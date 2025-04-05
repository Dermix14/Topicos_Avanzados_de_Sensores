#include <Adafruit_NeoPixel.h>  // Biblioteca para controlar LEDs NeoPixel

const uint8_t nLEDs = 6,        // Número de LEDs en la tira (6 en este caso)
              LEDPin = 0;       // Pin de conexión de los LEDs (GPIO 0)

Adafruit_NeoPixel strip(nLEDs, LEDPin, NEO_GRB + NEO_KHZ800);

uint32_t red = strip.Color(255, 0, 0),     // Rojo puro
         blue = strip.Color(0, 0, 255),     // Azul puro
         green = strip.Color(0, 255, 0),    // Verde puro
         purple = strip.Color(100, 255, 0), // Morado
         yellow = strip.Color(127, 0, 127), // Amarillo
         orange = strip.Color(255, 100, 0), // Naranja
         offColor = strip.Color(0, 0, 0);   // Apagado

void stripBegin(){
    strip.begin();           // Inicializa la tira de LEDs
    strip.setBrightness(255); // Establece brillo máximo (0-255)
}

void stripSetColor(uint16_t n, uint32_t color){
    strip.setPixelColor(n, color);  // Establece el color para el LED n
    strip.show();                   // Actualiza los LEDs con los nuevos colores
}
