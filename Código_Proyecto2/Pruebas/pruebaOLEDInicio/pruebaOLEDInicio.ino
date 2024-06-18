// Código de selección de modo
// Pantalla 1: TWA 
#include "perifericos.h"

#define INT0 8 // botón (el pin 2 no funciona)
#define ON_OFF_PANTALLA 6 //Pin habilitador de pantalla OLED

int buttonState = 0;
int lastButtonState = 1; // previous state of the button
int startPressed = 0;    // the moment the button was pressed
int endPressed = 0;      // the moment the button was released
unsigned long int holdTime = 0;        // how long the button was hold

char lastMode = 'T';
char finalMode;

OLED pantalla(ON_OFF_PANTALLA, INT0);

void setup() {
  pinMode(INT0, INPUT_PULLUP);
  pantalla.begin();
  Serial.begin(9600);
  pantalla.wakeUp();
  Serial.println("Listo!");
  lastMode = pantalla.updateDisplay(lastMode);
}

void loop() {
  
  buttonState = digitalRead(INT0); // read the button input
  if (buttonState != lastButtonState) { // button state changed
    updateState();
  }
  lastButtonState = buttonState;        // save state for next loop

  if (100 < holdTime && holdTime < 500) // Change screen
  {
    lastMode = pantalla.updateDisplay(lastMode);
    holdTime = 0;
  }

  if (holdTime > 1200)
  {
    finalMode = lastMode;
    Serial.println("Seleccionó ");
    if (finalMode == 'T')
    {
      Serial.println("TWA");
    }
    else
    {
      Serial.println("STEL");
    }
    while(1); // Pasar al programa principal
  }
}

void updateState() {
  if (buttonState == LOW) // the button has been just pressed 
  {
    startPressed = millis(); 
  }  
  else // the button has been just released
  {
    endPressed = millis();
    holdTime = endPressed - startPressed;
    // if (holdTime > 100) Serial.println(holdTime);
  }
}