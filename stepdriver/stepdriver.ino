#include <SoftwareSerial.h>

//defines pins numbers motor1 & motor2
const int stepPin = 4;
const int dirPin = 5;

//defines pins numbers motor3
const int stepPin2 = 9;
const int dirPin2 = 6;

int counter;
boolean flag = true;

#define RIGHTDIRPIN 1
#define LEFTDIRPIN 0
#define UPDIRPIN 1
#define DOWNDIRPIN 0

void setup() {
  
  Serial.begin(115200);
  while(!Serial)
  ;
 
   
  //Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  //Sets the two pins as Outputs for motor3
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);

}

//one step loop motor1&motor2
void step(const int direction) {

  digitalWrite(dirPin, direction); // Enables the motor to move in a particular direction
  //Makes 100 pulses for making two full cycle rotation
  for (int x = 0; x < 240 ; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}

void thirdStep(const int direction2) {

  //3. motorun pinleri için gerekli olan kod
  digitalWrite(dirPin2, direction2 ); // Enables the motor3 to move in a particular direction
  for (int x = 0; x < 190 ; x++) {
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }     // Makes 200 pulses for making two full cycle rotation
}

void rightMove(int direction) {

  thirdStep(direction);
}

void leftMove(int direction) {

  thirdStep(direction);
}

void upMove(int direction) {

  step(direction);
}

void downMove(int direction) {

  step(direction);
}

int x = 0;

void loop() {

  while(!Serial.available())
   ;
      
  Serial.read();  
 
  if (flag) {

    ++x;
    
    if(x<=4) 
      rightMove(RIGHTDIRPIN);
     
    else if(x==5)
      downMove(DOWNDIRPIN);
    
    else if(x<=9)  
      leftMove(LEFTDIRPIN);
    
    else if(x==10)
        downMove(DOWNDIRPIN);
    else if(x<=14)
        rightMove(RIGHTDIRPIN);    
    else { 
      upMove(UPDIRPIN);
      upMove(UPDIRPIN);
      delay(750); 
      for (int x = 0; x < 4 ; x++) {
        leftMove(LEFTDIRPIN);
      }
      
      flag = false;
    }
  }

}

