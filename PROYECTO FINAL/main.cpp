// Inclusión de librerías necesarias
#include <mpu.h>                  // Para el sensor MPU (acelerómetro/giroscopio)
#include <images.h>               // Contiene imágenes para la pantalla OLED
#include <U8G2lib.h>              // Librería para controlar la pantalla OLED
#include <TinyGPS++.h>            // Para procesamiento de datos GPS
#include <HardwareSerial.h>       // Para comunicación serial con hardware
#include <BluetoothSerial.h>      // Para comunicación Bluetooth

// Constante para el tamaño de la ventana de análisis de movimiento
const int WINDOW_SIZE = 100;

// Inicialización de objetos para los componentes hardware
U8G2_SH1107_PIMORONI_128X128_1_HW_I2C myScreen(U8G2_R0);  // Pantalla OLED
BluetoothSerial SerialBT;         // Objeto para Bluetooth
HardwareSerial SerialGPS(2);      // UART2 para GPS (RX=16, TX=17)
TinyGPSPlus gps;                  // Objeto para procesar datos GPS

// Función para centrar texto en la pantalla
uint8_t center(const char *strToCenter) {
  // Calcula la posición x para centrar el texto
  uint8_t x = ((myScreen.getDisplayWidth() - (myScreen.getUTF8Width(strToCenter))) / 2);
  
  // Asegura que la posición esté dentro de los límites de la pantalla
  if (x < 0 || x > myScreen.getDisplayWidth()) x = 0;

  return x;
}

// Función para dibujar en la pantalla OLED
void drawScreen(const char* strToDraw, unsigned char* bmpToDraw) {
  myScreen.firstPage();
  do {
    myScreen.setColorIndex(1);  // Establece color blanco (1) para dibujar
    
    // Dibuja la imagen BMP en posición (36,64) con tamaño 56x56
    myScreen.drawXBMP(36, 64, 56, 56, bmpToDraw);
    
    // Dibuja el texto "ESTADO:" centrado en Y=22
    myScreen.drawStr(center("ESTADO:"), 22, "ESTADO:");
    
    // Dibuja el texto del estado centrado en Y=52
    myScreen.drawStr(center(strToDraw), 52, strToDraw);
  } while (myScreen.nextPage());
}

// Configuración inicial del sistema
void setup() {
  Serial.begin(115200);          // Inicia comunicación serial para depuración
  SerialBT.begin("Alex");        // Inicia Bluetooth con nombre "Alex"
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17); // Inicia GPS con UART2
  
  // Mensajes de inicialización
  Serial.println("GPS initialized");
  Serial.println("Format: latitud;longitud");
  
  Wire.begin();                  // Inicia comunicación I2C
  mpuInit();                    // Inicializa el sensor MPU
  myScreen.begin();             // Inicia la pantalla OLED
  myScreen.setFont(u8g2_font_disrespectfulteenager_tu); // Establece fuente
}

// Bucle principal del programa
void loop() {
  // Variables para análisis de movimiento
  float smv_sum = 0, smv_sq_sum = 0;
  float smv[WINDOW_SIZE];

  // Procesamiento de datos GPS
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {  // Decodifica los datos GPS
      if (gps.location.isValid()) {      // Si la ubicación es válida
        // Imprime latitud y longitud con 6 decimales
        Serial.print(gps.location.lat(), 6);
        Serial.print(";");
        Serial.println(gps.location.lng(), 6);
        
        // Envía los mismos datos por Bluetooth
        SerialBT.print(gps.location.lat(), 6);
        SerialBT.print(";");
        SerialBT.print(gps.location.lng(), 6);
        SerialBT.println(";");
      }
    }
  }

  // Detección de errores del GPS
  if (millis() > 8000 && gps.charsProcessed() < 10) {
    Serial.println("ERROR;No hay señal GPS");
    delay(3000);
  }

  // Recolección de datos del acelerómetro para análisis de movimiento
  for (int i = 0; i < WINDOW_SIZE; i++) {
    // Calcula la magnitud del vector de aceleración (SMV)
    float smv_val = sqrt(getAccelX()*getAccelX() + getAccelY()*getAccelY() + getAccelZ()*getAccelZ());
    smv[i] = smv_val;
    smv_sum += smv_val;          // Suma para calcular la media
    smv_sq_sum += smv_val * smv_val; // Suma de cuadrados para varianza
    delay(10);                   // Pequeña pausa para tasa de muestreo
  }

  // Cálculo de estadísticas para detección de movimiento
  float mean = smv_sum / WINDOW_SIZE;               // Media
  float variance = (smv_sq_sum / WINDOW_SIZE) - (mean * mean); // Varianza

  // Determinación del estado de movimiento basado en la varianza
  if (variance < 0.09) {
    Serial.println("State: REST");
    drawScreen("REPOSO", rest_bits);          // Muestra estado de reposo
  } else if (variance >= 0.08 && variance < 0.4) {
    Serial.println("State: WALKING");
    drawScreen("CAMINANDO", walking_bits);   // Muestra estado caminando
  } else {
    Serial.println("State: RUNNING");
    drawScreen("CORRIENDO", running_bits);   // Muestra estado corriendo
  }
}
