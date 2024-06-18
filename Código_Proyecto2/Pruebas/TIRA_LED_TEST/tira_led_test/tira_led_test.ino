#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

//Designamos nuestro pin de datos
#define PIN 3
//Designamos cuantos pixeles tenemos en nuestra cinta led RGB
#define NUMPIXELS      8

//Definimos el número de pixeles de la cinta y el pin de datos
//   Parámetro 1 = número de pixeles de la cinta
//   Parámetro 2 = número de pin de datos del Arduino
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANTE: Para reducir el riesgo de dañar los pixeles de la cinta, añada un condensador de 1000 uF a través de
// los cables de alimentación de la cinta, agregue una resistencia de 300-500 Ohm en la entrada de datos del primer pixel
// Y reduzca la distancia entre Arduino y el primera pixel. 
// Evite realizar conexiones en el circuito vivo si lo va a hacer, conecte GND primero.

void setup() {
// Inicializamos nuestra cinta led RGB
  pixels.begin(); 
  
}

void alarma() {
  int delayval = 100; // Pausa de cien milisegundos

  for(int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(30, 0, 0)); // Brillo moderado en rojo
  }
  pixels.show(); // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
  delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).

  for(int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 30)); // Brillo moderado en azul
  }
  pixels.show(); // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
  delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
}

void loop() {
  alarma();    
  
}
