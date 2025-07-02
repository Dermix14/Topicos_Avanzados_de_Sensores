#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

MPU6050 mpu;
Quaternion q; 
VectorFloat gravity; 
VectorInt16 accel;
float ypr[3];   
float yawAngle, pitchAngle, rollAngle;
uint8_t fifoBuffer[64]; 

void mpuInit(uint8_t SDA_pin = SDA, uint8_t SCL_pin = SCL){
    Wire.begin(SDA_pin, SCL_pin);
    mpu.initialize();
    
    if(mpu.dmpInitialize() == 0){
        mpu.setDMPEnabled(true);
    }
    else{
        while(1);
    }
}

void getAngle(){
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {  // Check for new DMP data
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    }
}

void getAccel(){
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {  // Check for new DMP data
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetAccel(&accel, fifoBuffer);
    }
}

float getYawAngle(){
    // Output roll (ypr[0]) angle in degrees
    getAngle();
    yawAngle = ypr[0] * 180.0/M_PI;
    return yawAngle;
}

float getPitchAngle(){
    // Output Pitch (ypr[1]) angle in degrees
    getAngle();
    pitchAngle = ypr[1] * 180.0/M_PI;
    return pitchAngle;
}

float getRollAngle(){
    // Output roll (ypr[2]) angle in degrees
    getAngle();
    rollAngle = ypr[2] * 180.0/M_PI;
    return rollAngle;
}

float getAccelX(){
    getAccel();
    float ax = accel.x / 8192.0f;
    return ax;
}

float getAccelY(){
    getAccel();
    float ay = accel.y / 8192.0f;
    return ay;
}

float getAccelZ(){
    getAccel();
    float az = accel.z / 8192.0f;
    return az;
}