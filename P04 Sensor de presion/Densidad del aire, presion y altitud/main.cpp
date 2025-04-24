#include <Wire.h>              // Comunicación I2C
#include <Adafruit_Sensor.h>  // Base para sensores Adafruit
#include <Adafruit_BME280.h>  // Driver para el sensor BME280
#include <U8g2lib.h>          // Controlador para pantallas OLED

#ifdef ARDUINO_ESP32C3_DEV
  HWCDC SerialUSB;           // Configuración especial para ESP32-C3
#define SerialUSB_Port SerialUSB
#endif

U8G2_SH1106_72X40_WISE_1_HW_I2C myScreen(U8G2_R0, U8X8_PIN_NONE, 6, 5);
#define SEALEVELPRESSURE_HPA (1013.25)  // Presión estándar a nivel del mar

Adafruit_BME280 bme; // Objeto para el sensor BME280
float Rd = 287.058f; // Constante del gas para aire seco (J/(kg·K))

uint8_t center(const char *strToCenter) {
  uint8_t x = ((myScreen.getDisplayWidth() - (myScreen.getUTF8Width(strToCenter))) / 2);
  if (x < 0 || x > myScreen.getDisplayWidth()) x = 0;
  return x;
}

void drawData(char* which, double data, char* units){
  char strData[10] = {};
  dtostrf(data, 1, 4, strData);  // Convierte float/double a string
  
  myScreen.firstPage();
  do {
    myScreen.drawStr(center(which), 20, which);  // Título centrado
    myScreen.drawStr(center(strData), 30, strData); // Valor centrado
    myScreen.drawStr(center(units), 40, units);  // Unidades centradas
  } while (myScreen.nextPage());
}

void airDensity() {
  float temp = bme.readTemperature() + 273.15f; // Convertir a Kelvin
  float pressure = bme.readPressure(); // Presión en Pascales
  float rho = pressure / (Rd * temp); // Fórmula densidad del aire
  
  SerialUSB.printf("Densidad del aire: %f kg/m^3\n", rho);
  drawData("Densidad del aire:", rho, "kg/m^3");
}

void altitude(){ //Función para visualizar la altitud actual medida
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  SerialUSB.printf("Altitud: %f m\n", altitude);
  drawData("Altitud :", altitude, "m");
}

void pressure(){ //Función para visualizar la presión atmosférica actual medida
  float pressure = bme.readPressure()/100.0f; // Convertir a milibares
  SerialUSB.printf("Presión atmosférica: %f mb\n", pressure);
  drawData("Presion atm: ", pressure, "mb");
}

void temp(){ //Función para visualizar la temperatura actual medida
  float temp = bme.readTemperature();
  SerialUSB.printf("Temperatura: %f °C\n", temp);
  drawData("Temperatura: ", temp, "C");
}

void setup() {
  //Inicializando monitor serial y pantalla OLED
  SerialUSB.begin(115200);
  myScreen.begin();
  myScreen.setFont(u8g2_font_ncenB08_tr);
  myScreen.setContrast(255);
  
  while(!SerialUSB); // Espera conexión serial
  SerialUSB.println(F("BME280 test"));

  unsigned status = bme.begin(BME280_ADDRESS_ALTERNATE);
  
  while(!status){
    SerialUSB.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
  }
}

void loop() {
  readCommand(); // Espera comandos por serial
}
