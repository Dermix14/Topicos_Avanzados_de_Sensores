#include <Arduino.h>
#include <SoftwareSerial.h> //Inclusión de la librería para la creación de un puerto serial por software

SoftwareSerial GPS(4, 3); //Pines digitales utilizados para la comunicación serial (RX, TX)

char data = ' '; // Variable tipo char para obtener los datos char por char en el serial

void setup() {
  Serial.begin(115200); //Inicialización del monitor serial del arduino
  GPS.begin(9600); //Inicialización de la comuniación serial con el módulo GPS
}

void loop() {
  if(GPS.available()){ //Al detectar datos en el serial
    data = GPS.read(); //La variable char toma el valor del dato actual
    Serial.print(data); //El dato actual es impreso en el monitor serial
  }
}
