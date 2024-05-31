#include "perifericos.h"

OLED oled_display; // Crea un oled_display

void setup() {
  // clock_prescale_set(clock_div_128); // Set clock frequency to 32kHz
  Serial.begin(9600);
  oled_display.begin(); 
  
  
}

void loop()
{
  // oled_display.lowBattery();
  // oled_display.calentandoScreen();
  // oled_display.displayO2(80.0);
  // oled_display.displayNO2(420.0);
  // oled_display.displayCO(111.1);
}












