#include <mpu.h>
#include <colors.h>

#ifdef ARDUINO_ESP32C3_DEV
  HWCDC SerialUSB;
#define SerialUSB_port SerialUSB
#endif

void offOthers(uint8_t active){
  for(int i = 0; i < 6; i++){
    if(i != active) stripSetColor(i, offColor);
  }
}

void faceLights(){
  if(getAccelZ() >= 0.85f && getAccelZ() <= 1.1f){
    stripSetColor(0, red);
    offOthers(0);
  }
  else if(getAccelZ() <= -0.85f && getAccelZ() >= -1.1f){
    stripSetColor(4, green);
    offOthers(4);
  }
  else if(getAccelX() >= 0.85f && getAccelX() <= 1.1f){
    stripSetColor(1, blue);
    offOthers(1);
  }
  else if(getAccelX() <= -0.85f && getAccelX() >= -1.1f){
    stripSetColor(3, orange);
    offOthers(3);
  }
  else if(getAccelY() >= 0.85f && getAccelY() <= 1.1f){
    stripSetColor(5, purple);
    offOthers(5);
  }
  else if(getAccelY() <= -0.85f && getAccelY() >= -1.1f){
    stripSetColor(2, yellow);
    offOthers(2);
  }
}

void setup() {
    SerialUSB.begin(115200);
    mpuInit();
    stripBegin();
}

void loop() {
  faceLights();
  SerialUSB.print(getAccelX());
  SerialUSB.print("\t");
  SerialUSB.print(getAccelY());
  SerialUSB.print("\t");
  SerialUSB.println(getAccelZ());
}
