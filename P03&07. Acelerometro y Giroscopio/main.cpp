#include <mpu.h>          // Header para el sensor MPU (acelerómetro/giroscopio)
#include <colors.h>       // Header que define los colores (red, green, etc.)

#ifdef ARDUINO_ESP32C3_DEV  // Condicional para placas ESP32-C3
  HWCDC SerialUSB;        // Objeto para comunicación serial USB en ESP32-C3
#define SerialUSB_port SerialUSB  // Alias para el puerto serial
#endif

void offOthers(uint8_t active){
  for(int i = 0; i < 6; i++){  // Itera sobre las 6 caras
    if(i != active) stripSetColor(i, offColor);  // Apaga todas las caras excepto la activa
  }
}

void faceLights(){
  // Dependiendo de la orientación (eje Z positivo), enciende la cara 0 (rojo)
  if(getAccelZ() >= 0.85f && getAccelZ() <= 1.1f){
    stripSetColor(0, red);
    offOthers(0);
  }
  // Eje Z negativo - cara 4 (verde)
  else if(getAccelZ() <= -0.85f && getAccelZ() >= -1.1f){
    stripSetColor(4, green);
    offOthers(4);
  }
  // Eje X positivo - cara 1 (azul)
  else if(getAccelX() >= 0.85f && getAccelX() <= 1.1f){
    stripSetColor(1, blue);
    offOthers(1);
  }
  // Eje X negativo - cara 3 (naranja)
  else if(getAccelX() <= -0.85f && getAccelX() >= -1.1f){
    stripSetColor(3, orange);
    offOthers(3);
  }
  // Eje Y positivo - cara 5 (morado)
  else if(getAccelY() >= 0.85f && getAccelY() <= 1.1f){
    stripSetColor(5, purple);
    offOthers(5);
  }
  // Eje Y negativo - cara 2 (amarillo)
  else if(getAccelY() <= -0.85f && getAccelY() >= -1.1f){
    stripSetColor(2, yellow);
    offOthers(2);
  }
}

void setup() {
    SerialUSB.begin(115200);  // Inicia comunicación serial
    mpuInit();               // Inicializa el sensor MPU
    stripBegin();            // Inicializa la tira de LEDs
}

void loop() {
  faceLights();  // Actualiza las luces según la orientación
  
  // Imprime los valores del acelerómetro para depuración
  SerialUSB.print(getAccelX());
  SerialUSB.print("\t");
  SerialUSB.print(getAccelY());
  SerialUSB.print("\t");
  SerialUSB.println(getAccelZ());
}
