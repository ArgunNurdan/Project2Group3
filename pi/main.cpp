#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <math.h>
#include "serial.h"
#include "server.h"
#include "ConstructorErrException.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#ifdef GYRO_ENABLE
#error ???
#else
#define GYRO_ENABLE
#endif

using namespace cv;
using namespace cv::ml;
using namespace std;

struct StickMan {
    uint32_t x;
    uint32_t y;
    uint16_t angle;
    direction d;
    uint16_t x_l;
    uint16_t y_l;
    uint16_t length;
};

//bool foundStickMan = false;
StickMan myStickMan = {0,  0, 0,  none};
sem_t sem_counter;

#ifdef GYRO_ENABLE
MPU6050 mpu;
// MPU control/status vars
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
#endif

void exit_routine();
void handler(int sig);
void* socket_thread(void* arg);
void readData(Mat& out);
void mergeFrames(Mat* frames, uint16_t size, Mat& out);
void findAli(Mat& img);
inline void waitTillCameraIsSteady();
direction findDirection(double angle);
double findAngle(Point p1, Point p2);
Point getOrientation(vector<Point> &pts, Mat &img);
cv::Mat cropImg(cv::Mat img, int left, int up, int right, int down);
#ifdef GYRO_ENABLE
void gyro_setup();
#endif

int main()
{
    signal(SIGINT, handler);
#ifndef MERGE_TEST
    atexit(exit_routine);

    if(!init_serial())
        return 1;

    if(sem_init(&sem_counter, 0, 0) == -1)
        perror("sem_init");
#ifdef GYRO_ENABLE
    gyro_setup();
#endif
    try {
        Server server;
		cout << "client connected" << endl;
//		sleep(8);
#endif
#ifndef MERGE_TEST
        pthread_t thread;
        pthread_create(&thread, NULL, socket_thread, (void*)&server);
#endif
        Mat out;
        readData(out);
        resize(out, out, Size(840, 594));
        findAli(out);
//         cout << "redtfyghujıokX: " << myStickMan.x << endl << "Y: " << myStickMan.y << endl << "Rotation:" << myStickMan.angle << endl <<
//                 "x_l: " << myStickMan.x_l << endl << "y_l:" << myStickMan.y_l << endl << "length: " << myStickMan.length << endl
//                 << "D: " << (int)myStickMan.d << endl;


//        foundStickMan = true;
#ifndef MERGE_TEST
        sem_post(&sem_counter);
        pthread_join(thread, NULL);
        waitKey();
    }catch(ConstructorErrException e) {
        perror("");
        return 1;
    }
#endif
    return 0;
}

void handler(int sig)
{
    exit(0);
}

void* socket_thread(void* arg){

    Server& server = *(Server*)arg;

    for(int i=0;; ++i) {
        sem_wait(&sem_counter); //wait

        if(i<15)
            server.sendNextStep();

        else {
            server.sendCoordinate(myStickMan.x, myStickMan.y, myStickMan.d,
                                  myStickMan.x_l, myStickMan.y_l, myStickMan.length, myStickMan.angle);



            break;
        }
    }

    return NULL;
}

#ifndef MERGE_TEST
void readData(Mat &out)
{
    int numSteps = 16;
    Mat frames[numSteps - 1];

    VideoCapture camera;

    camera.open(0);

    if(!camera.isOpened()){
        perror("Camera cannot opened: ");
		exit(1);
	}

    for (int i = 0; i < numSteps - 1; ++i) {

        if(i != 0)
            sleep(1);

        waitTillCameraIsSteady();

        for(int j=0; j<10; ++j)
            camera >> frames[i];

//        char buf[10];
//        sprintf(buf, "%d.jpg", i + 1);
//        imwrite(buf, frames[i]);

        sem_post(&sem_counter); //notify thread
        usleep(250000); //reading frame
        sendNextStep();
    }

    sendNextStep();

    mergeFrames(frames, numSteps-1, out);

    camera.release();
}
#else
void readData(Mat &out){

    int numSteps = 16;
    Mat frames[numSteps - 1];

    for(int i = 0; i<15; ++i) {
        char buf[24];
       // sprintf(buf, "%d.jpg", i+1);
        //frames[i] = imread(buf);
    }

    mergeFrames(frames, numSteps-1, out);
}
#endif

void waitTillCameraIsSteady()
{
#ifndef GYRO_ENABLE
    sleep(1);
#else
    bool loop = true;
    mpu.resetFIFO(); //TODO timer
	cout << "Waiting data from gyro..." << endl;
    while(loop){
        float arr[3];
        float epsilon = 0.0001;

        for(int i=0; i<3; ++i) {

            while((mpu.getFIFOCount()) < 42)
                ;
            mpu.getFIFOBytes(fifoBuffer, packetSize);
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            arr[i] = q.w;
            printf("w: %7.2f\n", q.w);
        }

        if((fabsf(arr[0] - arr[1]) < epsilon) && (fabsf(arr[1] - arr[2]) < epsilon) && (fabsf(arr[0]-arr[2]) < epsilon)){
            loop = false;
        }
    }
#endif
}

void exit_routine()
{
    close_serial();
    sem_destroy(&sem_counter);
	cout << endl;
	cout << "Program finished. You can close the terminal." << endl;
}

void findAli(Mat &img)
{
    char key = 27;
    Scalar reference(0, 255, 0);
    Mat draw;
    RNG rng(12345);
    Mat src_gray;
    int dilation_elem = 0;
    int erosion_elem = 0;
    int erosion_size = 3;
    int dilation_size = 0;
    int const max_elem = 2;
    int const max_kernel_size = 21;
    Mat erosion_dst;
    int largest_area = 0;
    int largest_contour_index = 0;
    unsigned char ch;
    Rect bounding_rect, temp, temp2;
    Mat threshold_output_Temp;
    vector<vector<Point> > contours_poly_temp;
    vector<Rect> boundRectTemp;
    vector<Point2f>centerTemp;
    vector<float>radiusTemp;
    vector<vector<Point> > contoursTemp;
    int thresh = 100;
    int max_thresh = 255;


    if (!img.data){
        perror("");
        return;
    }

    cvtColor(img, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    int erosion_type;
    if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
    else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
    else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement(erosion_type,
                                        Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                        Point(erosion_size, erosion_size));

    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);

    erode(threshold_output, erosion_dst, element);
    erode(erosion_dst, threshold_output, element);
   // imshow("withErode", threshold_output);

    threshold_output_Temp = threshold_output;
    findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    contoursTemp = contours;
    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f>center(contours.size());
    vector<float>radius(contours.size());
    vector<RotatedRect> minRect(contours.size());
    RotatedRect cinAli;

    for (int i = 0; i < contours.size(); i++)
    {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
        minRect[i] = minAreaRect(Mat(contours_poly[i]));
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
        minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
    }

//    cout << "contours.size()" <<contours.size()<< endl;
    /**************************************************************************************/
    // For each object
    for (size_t i = 0; i < contours.size(); i++)
    {
        // Calculate its area
        double area = contourArea(contours[i]);

        // Ignore if too small or too large
        if (area < 600 || 1e5 < area) continue;

        // Draw the contour
        drawContours(img, contours, i, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0);

        Point p1 = getOrientation(contours[i], img);

        // Get the object orientation
        ////////////////////////////////////////////////////////////
        ++i;
        if (i >= contours.size())
            break;

        area = contourArea(contours[i]);

        // Ignore if too small or too large
        if (area < 100 || 1e5 < area) continue;

        // Draw the contour
        drawContours(img, contours, i, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0);
        vector<double> eigen_val_p2(2);

        Point p2 = getOrientation(contours[i], img);
//        cout << "i " << i  << endl;
//        cout << "p1 " << p1 << endl;
//        cout << "p2 " << p2<< endl;
        /* Founded Ali */
        double angle = findAngle(p1, p2);
        myStickMan.x = p1.x;
        myStickMan.y = p1.y;
        myStickMan.angle = (int)angle;
        myStickMan.d = findDirection(angle);
    }
    /**************************************************************************************/
    contours_poly_temp = contours_poly;
    boundRectTemp = boundRect;
    centerTemp = center;
    radiusTemp = radius;
    Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
    const char* source_window = "Trackbar";
    namedWindow(source_window, WINDOW_AUTOSIZE);
    imshow(source_window, img);
    createTrackbar(" Canny thresh:", source_window, &thresh, max_thresh);

    Point2i maxCenter;

    for (size_t i = 0; i< contours.size(); i++)  // iterate through each contour.
    {
        double area = boundRect[i].area();       //  Find the area of contour

        if (area > largest_area && area < 150000)
        {
            largest_area = area;
            largest_contour_index = i;               //Store the index of largest contour
            bounding_rect = boundRect[i];			 // Find the bounding rectangle for biggest contour
            maxCenter = center[i];
            cinAli = minRect[i];
        }
    }
    largest_area = 0;
    Mat drawing2 = Mat::zeros(threshold_output.size(), CV_8UC3);
    drawing2 = erosion_dst;
    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

    Point2f rect_points[4]; cinAli.points(rect_points);
    for (int j = 0; j < 4; j++)
        line(drawing2, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);

    Point center_of_rect = (bounding_rect.br() + bounding_rect.tl())*0.5;
//
//    cout << rect_points[0] << endl; // sol alt kose
//    cout << rect_points[1] << endl; // sol ust kose
//    cout << rect_points[2] << endl; // sag ust kose
//    cout << rect_points[3] << endl; // sag alt kose

    myStickMan.x_l = rect_points[0].x;
    myStickMan.y_l = rect_points[0].y;

    uint16_t uzunluk=0;

    switch (myStickMan.d) {
        case nw:
        case se:
        //kuzeybati ve guneydogu
            uzunluk = (uint16_t)sqrt(abs(rect_points[0].x - rect_points[2].x) * abs(rect_points[0].x - rect_points[2].x) +
                                     abs(rect_points[0].y - rect_points[2].y) * abs(rect_points[0].y - rect_points[2].y));

//            cout << uzunluk << endl;
            break;
        //kuzeydogu ve guneybatı
        case ne:
        case sw:
            uzunluk = (uint16_t)sqrt(abs(rect_points[1].x - rect_points[3].x) * abs(rect_points[1].x - rect_points[3].x) +
                                     abs(rect_points[1].y - rect_points[3].y) * abs(rect_points[1].y - rect_points[3].y));

//            cout << uzunluk << endl;
            break;
        case north:
        case south:
        //kuzey güney
            uzunluk = (uint16_t)sqrt(abs(rect_points[2].x - rect_points[3].x) * abs(rect_points[2].x - rect_points[3].x) +
                                     abs(rect_points[2].y - rect_points[3].y) * abs(rect_points[2].y - rect_points[3].y));

//            cout << uzunluk << endl;
            break;
        case west:
        case east:
        //dogu bati
            uzunluk = (uint16_t)sqrt(abs(rect_points[0].x - rect_points[3].x) * abs(rect_points[0].x - rect_points[3].x) +
                                     abs(rect_points[0].y - rect_points[3].y) * abs(rect_points[0].y - rect_points[3].y));

//            cout << uzunluk << endl;
            break;
    }

    myStickMan.length = uzunluk;

    // namedWindow("Contours2", CV_WINDOW_AUTOSIZE);
   // imshow("Contours2", drawing2);


}

double findAngle(Point p1, Point p2)
{
    double angle = atan2(p1.y - p2.y, p2.x - p1.x);
    angle = angle*180.0 / 3.1415;
    if (angle < 0) {
        angle += 360;
    }
    return angle;
}

direction findDirection(double angle) {
    direction direct;
  //  cout << "find direction   angle-> " << angle << endl;
    if (angle >= 340 && angle <= 360) {
    //    cout << "DOGU" << endl;
        direct = east;
    } else if (angle >= 0 && angle <= 50) {
      //  cout << "KUZEY DOGU" << endl;
        direct = ne;
    } else if (angle > 50 && angle < 100) {
       // cout << "KUZEY" << endl;
        direct = north;
    } else if (angle >= 100 && angle < 140) {
      //  cout << "KUZEY BATI" << endl;
        direct = nw;
    } else if (angle >= 140 && angle < 220) {
       // cout << "BATI" << endl;
        direct = west;
    } else if (angle >= 220 && angle < 260) {
      //  cout << "GUNEY BATI" << endl;
        direct = sw;
    } else if (angle >= 260 && angle < 280) {
      //  cout << "GUNEY" << endl;
        direct = south;
    } else if (angle >= 280 && angle <= 340) {
      //  cout << "GUNEY DOGU" << endl;
        direct = se;
    } else {
     //   cout << "NONE" << endl;
        direct = none;
    }

    return direct;
}

Point getOrientation(vector<Point> &pts, Mat &img)
{
    //Construct a buffer used by the pca analysis
    Mat data_pts = Mat(pts.size(), 2, CV_64FC1);
    for (int i = 0; i < data_pts.rows; ++i)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }

    //Perform PCA analysis
    PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);

    //Store the position of the object
    Point pos = Point(pca_analysis.mean.at<double>(0, 0),
                      pca_analysis.mean.at<double>(0, 1));

    //Store the eigenvalues and eigenvectors
    vector<Point2d> eigen_vecs(2);
    vector<double> eigen_val(2);
    for (int i = 0; i < 1; ++i)
    {
        eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
                                pca_analysis.eigenvectors.at<double>(i, 1));
        eigen_val[i] = pca_analysis.eigenvalues.at<double>(0, i);

    }

    // Draw the principal components
//    cout << pos.x << " - " << pos.y << endl;
//    cout << eigen_val[0] << " -- " << eigen_val[1] << endl;

    circle(img, pos, 3, CV_RGB(255, 0, 255), 2);
    line(img, pos, pos + 0.02 * Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]), CV_RGB(255, 255, 0));  // sari
    line(img, pos, pos + 0.02 * Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]), CV_RGB(0, 255, 255));  // mavi

    return pos;
}

/**
 * @param order 0 düz, 1 ters
 */
void merge_h(Mat *frames, uint8_t size, Mat& out, uint8_t order)
{
    if(order == 0) {
        out = frames[0];

        for(int i=1; i<size; ++i)
            hconcat(out, frames[i], out);

    }else {
        out = frames[size - 1];

        for(int i=size-2; i>=0; --i)
            hconcat(out, frames[i], out);
    }
}

void mergeFrames(Mat *frames, uint16_t size, Mat &out)
{
    Mat l1, l2, l3;
    int b1 = 120;
    int b2 = 120;
    int b3 = 120;
    int b4 = 120;
    int b5 = 120;


//    for(int i=0; i<15; ++i)
//        if(i != 4 && i != 5 && i != 14)
//            frames[i] = cropImg(frames[i], 0, 0, /*227*/256, 0);

    //blok 1
    frames[0] = cropImg(frames[0], 0, 0, b1, 0);
    frames[9] = cropImg(frames[9], 0, 0, b1, 0);
    frames[10] = cropImg(frames[10], 0, 0, b1, 0);

    //blok 2
    frames[1] = cropImg(frames[1], b2, 0, b2, 0);
    frames[8] = cropImg(frames[8], b2, 0, b2, 0);
    frames[11] = cropImg(frames[11], b2, 0, b2, 0);

    //blok 3
    frames[2] = cropImg(frames[2], b3, 0, b3, 0);
    frames[7] = cropImg(frames[7], b3, 0, b3, 0);
    frames[12] = cropImg(frames[12], b3, 0, b3, 0);

    //blok 4
    frames[3] = cropImg(frames[3], b4, 0, b4, 0);
    frames[6] = cropImg(frames[6], b4, 0, b4, 0);
    frames[13] = cropImg(frames[13], b4, 0, b4, 0);

    //blok 5
    frames[4] = cropImg(frames[4], b5, 0, b5, 0);
    frames[5] = cropImg(frames[5], b5, 0, b5, 0);
    frames[14] = cropImg(frames[14], b5, 0, b5, 0);


    merge_h(frames, 5, l1, 0);
    merge_h(frames + 5, 5, l2, 1);
    merge_h(frames + 10, 5, l3, 0);

    out = l3;
    vconcat(out, l2, out);
    vconcat(out, l1, out);
}

cv::Mat cropImg(cv::Mat img, int left, int up, int right, int down){
    cv::Rect roi;
    roi.x = left;
    roi.y = up;
    roi.width = img.size().width - left - right;
    roi.height = img.size().height - up - down;
    Mat crop = img(roi);
    return crop;
}

#ifdef GYRO_ENABLE
void gyro_setup() {
    // initialize device
    printf("Initializing I2C devices...\n");
    mpu.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(mpu.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");

    // load and configure the DMP
    printf("Initializing DMP...\n");
    devStatus = mpu.dmpInitialize();

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        printf("Enabling DMP...\n");
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        //attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        printf("DMP ready!\n");

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        printf("DMP Initialization failed (code %d)\n", devStatus);
    }
}
#endif
