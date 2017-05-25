cmake_minimum_required(VERSION 3.6)
project(pi)

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )

set(SOURCE_FILES main.cpp rs232.c rs232.h serial.cpp serial.h server.cpp server.h ConstructorErrException.h ConstructorErrException.h MPU6050.cpp MPU6050.h I2Cdev.cpp I2Cdev.h MPU6050_6Axis_MotionApps20.h helper_3dmath.h)

add_executable(pi ${SOURCE_FILES})

target_link_libraries(pi ${OpenCV_LIBS} pthread m)
