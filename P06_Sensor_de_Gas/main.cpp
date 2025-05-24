const int mq2Pin = 34; // Usamos GPIO34, un pin ADC de solo entrada

void setup() {
  Serial.begin(115200); // Velocidad recomendada para ESP32
}

void loop() {
  int sensorValue = analogRead(mq2Pin); // Leer el valor del MQ-2 en GPIO34
  float voltage = sensorValue * (3.3 / 4095.0); // ESP32 usa ADC de 12 bits (0 a 4095)

  Serial.print(voltage, 2); //Imprimiendo el valor leído de voltaje con 2 decimales de precisión
  Serial.println(" ");

  delay(1000);
}
