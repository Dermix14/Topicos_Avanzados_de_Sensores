// Incluye la biblioteca Adafruit_VL53L0X que permite interactuar con el sensor VL53L0X.
#include "Adafruit_VL53L0X.h"

// Crea una instancia del objeto Adafruit_VL53L0X llamada "lox".
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  // Inicia la comunicación serial a una velocidad de 115200 baudios.
  Serial.begin(115200);

  // Espera a que el puerto serial esté disponible (necesario para dispositivos USB nativos).
  while (! Serial) {
    delay(1);  // Espera 1 milisegundo antes de verificar nuevamente.
  }
  
  // Imprime un mensaje en el monitor serial indicando que se está iniciando la prueba del sensor.
  Serial.println("Adafruit VL53L0X test");

  // Intenta inicializar el sensor VL53L0X.
  if (!lox.begin()) {
    // Si la inicialización falla, imprime un mensaje de error y entra en un bucle infinito.
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);  // Bucle infinito que detiene la ejecución del programa.
  }

  // Si la inicialización es exitosa, imprime un mensaje indicando que el ejemplo de medición simple está listo.
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

void loop() {
  // Crea una estructura para almacenar los datos de medición del sensor.
  VL53L0X_RangingMeasurementData_t measure;
    
  // Imprime un mensaje indicando que se está realizando una medición.
  Serial.print("Reading a measurement... ");

  // Realiza una medición de distancia. El segundo parámetro 'false' indica que no se desea imprimir datos de depuración.
  lox.rangingTest(&measure, false); // Cambia a 'true' si deseas ver datos de depuración en el monitor serial.

  // Verifica si la medición es válida (RangeStatus != 4 indica que no hubo fallas en la medición).
  if (measure.RangeStatus != 4) {  // 4 indica que la medición está fuera de rango o hubo un error.
    // Si la medición es válida, imprime la distancia en milímetros.
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    // Si la medición no es válida, imprime un mensaje indicando que está fuera de rango.
    Serial.println(" out of range ");
  }
    
  // Espera 100 milisegundos antes de realizar la siguiente medición.
  delay(100);
}
