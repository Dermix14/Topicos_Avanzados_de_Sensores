#include <mpu.h>          // Header para el sensor MPU (acelerómetro/giroscopio)

#ifdef ARDUINO_ESP32C3_DEV  // Condicional para placas ESP32-C3
  HWCDC SerialUSB;        // Objeto para comunicación serial USB en ESP32-C3
#define SerialUSB_port SerialUSB  // Alias para el puerto serial
#endif

float masa = 0.05f; //Masa medida del conjunto en kg

void setup() {
  SerialUSB.begin(115200); // Inicializando monitor serial a 115200 baudios
  mpuInit(); //Inicializando mpu
}

void loop() {
  // Calcular fuerza en X (F = m * a)
  float ax_mss = getAccelX() * 9.81f; //Obteniendo la aceleración en m/s^2
  float force_x = masa * ax_mss * 1000.0f; //Obteniendo la fuerza por segunda ley de Newton en mN
  
  // Enviar datos al serial (para graficar)
  SerialUSB.println(force_x);
  
  delay(10); // Tiempo en el que se toma cada muestra
}
