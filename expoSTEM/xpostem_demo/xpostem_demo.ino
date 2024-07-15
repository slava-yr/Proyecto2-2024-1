#include <Adafruit_NeoPixel.h>
// #include <avr/power.h>
#include "perifericos.h"

// Pines para la tira led
#define ON_OFF_LED A3 // Pin para habilitar alimentación de los LED
#define COLOR_LED 3 // Pin pwm para el color de los leds

// Pines de habilitación de alimentación
#define ON_OFF_BUZZER 4   // Pin habilitador de buzzer
#define ON_OFF_PANTALLA 6 // Pin habilitador de pantalla OLED
#define ON_OFF_SENSORES 7 // Pin que controla la alimentación de los sensores
#define ON_OFF_VIB 10     // Pin habilitador de vibrador

// Pines para habilitación de sensores (solo NO2 y CO)
#define EN_CO 8
#define EN_NO2 9  

// Pin de interrupción
#define INT0 2

// Pines de lectura del ADC
#define BAT_VOLT A6 // Pin de lectura de la tensión en la batería
#define CO_OUT  A0  // Pin de lectura de valor de sensor CO
#define NO2_OUT  A1 // Pin de lectura de valor de sensor NO2
#define O2_OUT  A2  // Pin de lectura de valor de sensor O2

#define NUM_GASES 3 // Solo para debugear, quitar después
#define measurementInterval 3000 // 120000 // 2 minutos en ms

OLED pantalla(ON_OFF_PANTALLA, INT0); // Crea un oled_display
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER); // Crea los indicadores

volatile bool buttonPressed = false; // Botón de interrupción presionado

void setup() {
  Serial.begin(9600);
  pinMode(COLOR_LED, OUTPUT);
  pinMode(INT0, INPUT);
  pinMode(EN_CO, OUTPUT);
  pinMode(EN_NO2, OUTPUT);
  pinMode(ON_OFF_SENSORES, OUTPUT);

  // Alimentar sensores para calentar
  digitalWrite(ON_OFF_SENSORES, HIGH);
  digitalWrite(EN_CO, HIGH);
  digitalWrite(EN_NO2, HIGH);

  indicadores.begin();
  digitalWrite(ON_OFF_BUZZER, HIGH);
  digitalWrite(ON_OFF_LED, HIGH);
  digitalWrite(ON_OFF_VIB, HIGH);
  indicadores.patron_inicio();
  pantalla.begin();
}

void loop() {
  char selected_mode = pantalla.selectMode();  
  unsigned long int lastMeasurementTime = 0;
  // Calentando screen por 20 segundos
  pantalla.calentandoScreen(); // Dos veces

  attachInterrupt(digitalPinToInterrupt(INT0), onButtonPress, RISING); // Activa interrupciones
  float CO, NO2, O2;
  while(1)
  {
    unsigned long int currentTime = millis(); // Tiempo actual
    
    if (currentTime - lastMeasurementTime >= measurementInterval) // Cada 2 minutos
    {
      lastMeasurementTime = currentTime; // Actualiza el tiempo

      // Tomar lecturas  
      CO = GasData();
      NO2 = GasData();
      O2 = GasData();      

    }
    if (buttonPressed == true) // Si se presiona el botón
    {
      pantalla.displayLecturas(CO, NO2, O2); // Mostrar las lecturas
      buttonPressed = false;
    }
  }
}

float GasData() {
  return random(10, 50); // Devuelve un valor aleatorio entre 10 y 50 ppm
}

void onButtonPress() { // Cuando se presiona el botón
  buttonPressed = true; // Bandera de interrupción
}