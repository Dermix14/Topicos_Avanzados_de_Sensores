// Inclusión de librerías necesarias
#include <Arduino.h>          // Librería básica de Arduino
#include <Wire.h>             // Para comunicación I2C
#include <Adafruit_Sensor.h>  // Librería base para sensores Adafruit
#include <Adafruit_BME280.h>  // Para el sensor BME280 (temperatura, humedad, presión)

// Configuración especial para placas ESP32-C3
#ifdef ARDUINO_ESP32C3_DEV
  HWCDC SerialUSB;           // Objeto serial para ESP32-C3
#define SerialUSB_Port SerialUSB
#endif

Adafruit_BME280 bme; // Crear objeto BME280 usando I2C

// Definición de las etapas fenológicas con sus unidades calor umbral (en grados-día)
const float EMERGENCIA = 12.89f;       // Umbral para etapa de emergencia
const float FORMACION_GUIAS = 64.41f;   // Umbral para formación de guías
const float FLORACION = 91.33f;        // Umbral para floración
const float FORMACION_VAINA = 99.31f;  // Umbral para formación de vaina

// Variables para el cálculo de grados-día
float maxTemp = -100.0f;   // Inicializar con valor muy bajo para asegurar que cualquier temperatura será mayor
float minTemp = 100.0f;    // Inicializar con valor muy alto para asegurar que cualquier temperatura será menor
float unidadesCalorAcumuladas = 0.0f;  // Acumulador de grados-día
unsigned int day = 1;       // Contador de días
unsigned long lastUpdate = 0; // Para controlar el tiempo entre lecturas
const long interval = 3600000; // Intervalo de 1 hora entre lecturas (en milisegundos)

// Pines para LEDs indicadores de cada etapa fenológica
const uint8_t ledEmergencia = 3;
const uint8_t ledGuias = 4;
const uint8_t ledFloracion = 5;
const uint8_t ledVaina = 6;

// Función para determinar y mostrar la etapa fenológica actual
void phenologicStage() {
  // Apagar todos los LEDs primero (para evitar múltiples LEDs encendidos)
  digitalWrite(ledEmergencia, LOW);
  digitalWrite(ledGuias, LOW);
  digitalWrite(ledFloracion, LOW);
  digitalWrite(ledVaina, LOW);
  
  // Determinar etapa actual basada en las unidades de calor acumuladas
  if (unidadesCalorAcumuladas < EMERGENCIA) {
    SerialUSB.println("Etapa: Pre-emergencia");
  } 
  else if (unidadesCalorAcumuladas < FORMACION_GUIAS) {
    SerialUSB.println("Etapa: Emergencia");
    digitalWrite(ledEmergencia, HIGH);  // Encender LED correspondiente
  } 
  else if (unidadesCalorAcumuladas < FLORACION) {
    SerialUSB.println("Etapa: Formacion de guias");
    digitalWrite(ledGuias, HIGH);
  } 
  else if (unidadesCalorAcumuladas < FORMACION_VAINA) {
    SerialUSB.println("Etapa: Floracion");
    digitalWrite(ledFloracion, HIGH);
  } 
  else {
    SerialUSB.println("Etapa: Formacion de vaina");
    digitalWrite(ledVaina, HIGH);
  }
  
  SerialUSB.println("---------------------");
}

// Función para calcular las unidades de calor (grados-día) acumuladas
void calcularUnidadesCalor() {
  // Calcular unidades calor del día (promedio de temperatura máxima y mínima)
  float unidadesCalorDia = (maxTemp + minTemp) / 2;
  unidadesCalorAcumuladas += unidadesCalorDia;
  
  // Mostrar resumen del día por el puerto serial
  SerialUSB.println("\n--- Resumen del dia " + String(day) + " ---");
  SerialUSB.print("Temperatura maxima: "); SerialUSB.print(maxTemp); SerialUSB.println("°C");
  SerialUSB.print("Temperatura minima: "); SerialUSB.print(minTemp); SerialUSB.println("°C");
  SerialUSB.print("Unidades calor del dia: "); SerialUSB.println(unidadesCalorDia);
  SerialUSB.print("Unidades calor acumuladas: "); SerialUSB.println(unidadesCalorAcumuladas);
  
  // Determinar y mostrar la etapa fenológica actual
  phenologicStage();
  
  // Reiniciar valores para el nuevo día
  maxTemp = -100.0f;
  minTemp = 100.0f;
}

// Configuración inicial del sistema
void setup() {
  SerialUSB.begin(115200);  // Iniciar comunicación serial
  bme.begin();              // Inicializar sensor BME280
  
  // Configurar pines de los LEDs como salida
  pinMode(ledEmergencia, OUTPUT);
  pinMode(ledGuias, OUTPUT);
  pinMode(ledFloracion, OUTPUT);
  pinMode(ledVaina, OUTPUT);
  
  // Mensaje inicial
  SerialUSB.println("Sistema de monitoreo fenologico del frijol con sensor DHT22");
  SerialUSB.println("--------------------------------------------------------");
}

// Bucle principal del programa
void loop() {
  unsigned long currentTime = millis();  // Obtener tiempo actual
  
  // Tomar lectura cada hora (según el intervalo definido)
  if (currentTime - lastUpdate >= interval) {
    lastUpdate = currentTime;
    
    // Leer temperatura actual del sensor
    float currentTemp = bme.readTemperature();
    
    // Actualizar temperaturas máxima y mínima del día
    if (currentTemp > maxTemp) {
      maxTemp = currentTemp;
    }
    if (currentTemp < minTemp) {
      minTemp = currentTemp;
    }
    
    // Mostrar lectura actual
    SerialUSB.print("Hora: ");
    SerialUSB.print(currentTime / 3600000);
    SerialUSB.print(" - Temperatura actual: ");
    SerialUSB.print(currentTemp);
    SerialUSB.println("°C");
  }
  
  // Al final del día (cada 24 horas), calcular unidades de calor
  if (currentTime >= day * 86400000) { // 86400000 ms = 24 horas
    calcularUnidadesCalor();
    day++;  // Incrementar contador de días
  }
}
