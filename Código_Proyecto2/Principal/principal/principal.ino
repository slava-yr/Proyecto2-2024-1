#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "perifericos.h"

//Pines para la tira led
#define COLOR_LED 3   //Pin PWM para el color de los LED
#define ON_OFF_LED A3 //Pin para habilitar alimentación de los LED
#define NUMLEDS 8     //Número de LEDs en la tira 

//Pines de habilitación de alimentación
#define ON_OFF_BUZZER 4   //Pin habilitador de buzzer
#define ON_OFF_PANTALLA 6 //Pin habilitador de pantalla OLED
#define ON_OFF_SENSORES 7 //Pin que controla la alimentación de los sensores
#define ON_OFF_VIB 10     //Pin habilitador de vibrador

//Pines para habilitación de sensores (solo NO2 y CO)
#define EN_CO 8
#define EN_NO2 9  

//Pin de interrupción
#define INT0 2 //Pin de interrupción

//Pines de lectura del ADC
#define BAT_VOLT A6 //Pin de lectura de la tensión en la batería
#define CO_OUT  A0  //Pin de lectura de valor de sensor CO
#define NO2_OUT  A1 //Pin de lectura de valor de sensor NO2
#define O2_OUT  A2  //Pin de lectura de valor de sensor O2


OLED pantalla(ON_OFF_PANTALLA, INT0); // Crea un oled_display
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER); // Crea los indicadores

void setup() {
  // put your setup code here, to run once:
  pinMode(INT0, INPUT);
  pinMode(EN_CO, OUTPUT);
  pinMode(EN_NO2, OUTPUT);

  pantalla.begin(); 
  indicadores.begin();
}

void loop() {

}
