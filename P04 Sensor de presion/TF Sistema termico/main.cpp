// Inclusión de librerías necesarias
#include <Arduino.h>          // Librería básica de Arduino (funciones principales)
#include <Wire.h>             // Para comunicación I2C (protocolo para sensores)
#include <Adafruit_Sensor.h>  // Librería base para sensores Adafruit (interfaz común)
#include <Adafruit_BME280.h>  // Para el sensor BME280 (temperatura, humedad, presión)

// Configuración especial para placas ESP32-C3
#ifdef ARDUINO_ESP32C3_DEV
  HWCDC SerialUSB;           // Objeto serial para ESP32-C3 (comunicación USB)
#define SerialUSB_Port SerialUSB  // Definir alias para el puerto serial
#endif

Adafruit_BME280 bme; // Crear objeto BME280 usando comunicación I2C

// Configuración del sensor LM35 (sensor de temperatura analógico)
const uint8_t LM35 = 0; // Pin analógico donde está conectado el LM35
unsigned long t0;       // Variable para almacenar tiempo inicial

// Configuración de lectura para LM35
uint8_t resolutionBits = 12; // Resolución de 12 bits para la lectura analógica

// Función para leer temperatura del LM35
float readLM35(){
  // Leer valor analógico y convertirlo a voltaje (rango 0-3.3V)
  float voltageLM35 = float((analogRead(LM35) * 3.3f) / pow(2, resolutionBits));
  
  // Convertir voltaje a temperatura (LM35 produce 10mV por °C)
  float tempLM35 = float(voltageLM35 * 100.0f);

  return tempLM35; // Devolver temperatura en °C
}

// Configuración inicial del sistema
void setup() {
  SerialUSB.begin(115200);  // Iniciar comunicación serial a 115200 baudios
  while(!SerialUSB);        // Esperar a que el puerto serial esté listo (solo para algunas placas)

  // Intentar inicializar el sensor BME280 con dirección alternativa
  unsigned status;
  status = bme.begin(BME280_ADDRESS_ALTERNATE);  

  // Si no se encuentra el sensor, mostrar mensaje de error
  while(!status){
    SerialUSB.println("No se encontró un sensor BME válido");
    // Nota: Este bucle es infinito si no se encuentra el sensor
  }     

  t0 = millis(); // Guardar tiempo de inicio para referencia temporal
}

// Bucle principal del programa
void loop() {
  // Imprimir datos en formato tabulado:
  // 1. Temperatura del LM35
  // 2. Temperatura del BME280
  // 3. Tiempo transcurrido desde el inicio (ms)
  SerialUSB.printf("%f\t%f\t%d\n", readLM35(), bme.readTemperature(), millis()-t0);
  
  delay(100); // Pequeña pausa entre lecturas (100ms)
}
