#include <Arduino.h>
#include <Wire.h>                       // Biblioteca para comunicación I2C
#include <MPU6050_6Axis_MotionApps20.h> // Biblioteca específica para MPU6050 con DMP

MPU6050 mpu;                            // Objeto principal del sensor
Quaternion q;                           // Almacena la orientación en cuaterniones
VectorFloat gravity;                    // Vector de gravedad calculado
VectorInt16 accel;                      // Datos de aceleración en enteros de 16 bits
float ypr[3];                          // Array para yaw, pitch, roll (ángulos Euler)
float yawAngle, pitchAngle, rollAngle;  // Variables para ángulos en grados
uint8_t fifoBuffer[64];                 // Buffer para datos FIFO del DMP

void mpuInit(uint8_t SDA_pin = SDA, uint8_t SCL_pin = SCL){
    Wire.begin(SDA_pin, SCL_pin);      // Inicia comunicación I2C con pines opcionales
    mpu.initialize();                  // Inicializa el sensor MPU6050
    
    if(mpu.dmpInitialize() == 0){      // Intenta inicializar el DMP
        mpu.setDMPEnabled(true);       // Habilita el DMP si la inicialización fue exitosa
    }
    else{
        while(1);                      // Bucle infinito si falla la inicialización (detiene el programa)
    }
}

void getAngle(){
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {  // Verifica si hay datos disponibles
        mpu.dmpGetQuaternion(&q, fifoBuffer);       // Obtiene cuaternión
        mpu.dmpGetGravity(&gravity, &q);            // Calcula vector gravedad
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);  // Convierte a ángulos Euler
    }
}

void getAccel(){
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {  // Verifica datos disponibles
        mpu.dmpGetQuaternion(&q, fifoBuffer);       // Obtiene cuaternión
        mpu.dmpGetGravity(&gravity, &q);            // Calcula gravedad
        mpu.dmpGetAccel(&accel, fifoBuffer);        // Obtiene datos de aceleración
    }
}

float getYawAngle(){
    getAngle();
    yawAngle = ypr[0] * 180.0/M_PI;  // Convierte de radianes a grados
    return yawAngle;
}

float getPitchAngle(){
    getAngle();
    pitchAngle = ypr[1] * 180.0/M_PI;
    return pitchAngle;
}

float getRollAngle(){
    getAngle();
    rollAngle = ypr[2] * 180.0/M_PI;
    return rollAngle;
}

float getAccelX(){
    getAccel();
    float ax = accel.x / 8192.0f;  // Escala según rango ±4g (8192 LSB/g)
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
