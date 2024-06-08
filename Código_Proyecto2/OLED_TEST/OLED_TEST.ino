#include "perifericos.h"
#define OLED_ENABLE 3 // enable pin OLED

const byte interruptPin = 2;
volatile byte buttonFlag = 0;

OLED oled_display(OLED_ENABLE); // Crea un oled_display

void setup() {
  // clock_prescale_set(clock_div_128); // Set clock frequency to 32kHz
  Serial.begin(9600);
  oled_display.begin(); 
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR_button, RISING);
  
}

void loop()
{
  if (buttonFlag == 1)
  {
    oled_display.calentandoScreen();
    buttonFlag = 0;
  }
  
  // oled_display.lowBattery();
  // oled_display.calentandoScreen();
  // oled_display.displayO2(80.0);
  // oled_display.displayNO2(420.0);
  // oled_display.displayCO(111.1);


}

void ISR_button()
{
  buttonFlag = 1;
}












