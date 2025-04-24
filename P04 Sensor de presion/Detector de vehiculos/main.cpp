#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <U8g2lib.h>

#ifdef ARDUINO_ESP32C3_DEV
  HWCDC SerialUSB;
#define SerialUSB_Port SerialUSB
#endif

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

// Variables para detección
const float UMBRAL_DETECCION = 0.3; // hPa de cambio para detectar vehículo
const int TIEMPO_ENTRE_DETECCIONES = 2000; // ms
unsigned long ultimaDeteccion = 0;

float presionBase;
bool vehiculoDetectado = false;

void detectarVehiculo(float diferencia) {
  vehiculoDetectado = true;
  
  SerialUSB.print("Vehículo detectado! Cambio de presión: ");
  SerialUSB.print(diferencia);
  SerialUSB.println(" hPa");
  
  // Clasificación básica por magnitud del cambio
  if (abs(diferencia) > 0.8) {
    SerialUSB.println("Tipo: Vehículo grande (camión/SUV)");
  } else if (abs(diferencia) > 0.5) {
    SerialUSB.println("Tipo: Automóvil");
  } else {
    SerialUSB.println("Tipo: Motocicleta o vehículo pequeño");
  }
  
}

void setup() {
  SerialUSB.begin(115200);
  while (!SerialUSB);
  
  while(!bme.begin(BME280_ADDRESS_ALTERNATE)) { 
    SerialUSB.println("No se encontró el sensor BME280!");
  }
  
  // Establecer presión base (promedio de 10 lecturas)
  SerialUSB.println("Calibrando...");
  float suma = 0;
  for (int i = 0; i < 10; i++) {
    suma += bme.readPressure() / 100.0F;
    delay(100);
  }
  presionBase = suma / 10;
  
  SerialUSB.println("Sistema listo");
  SerialUSB.print("Presión base: "); SerialUSB.print(presionBase); SerialUSB.println(" hPa");
  SerialUSB.print("Vehículo detectado! Cambio de presión: ");
  SerialUSB.print(diferencia);
  SerialUSB.println(" hPa");
  
  // Clasificación básica por magnitud del cambio
  if (abs(diferencia) > 0.8) {
    SerialUSB.println("Tipo: Vehículo grande (camión/SUV)");
  } else if (abs(diferencia) > 0.5) {
    SerialUSB.println("Tipo: Automóvil");
  } else {
    SerialUSB.println("Tipo: Motocicleta o vehículo pequeño");
  }
}

void loop() {
  float presionActual = bme.readPressure() / 100.0F;
  float diferencia = presionActual - presionBase;
  
  // Detectar vehículos solo si ha pasado el tiempo mínimo entre detecciones
  if (millis() - ultimaDeteccion > TIEMPO_ENTRE_DETECCIONES) {
    if (abs(diferencia) > UMBRAL_DETECCION) {
      detectarVehiculo(diferencia);
      ultimaDeteccion = millis();
    }
  }
  
  // Ajuste gradual de la presión base
  presionBase = presionBase * 0.99 + presionActual * 0.01;
  
  delay(50); // Pequeña pausa entre lecturas
}
