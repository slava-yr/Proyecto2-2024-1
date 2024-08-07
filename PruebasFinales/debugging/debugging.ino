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

#define measurementInterval 3000 // 120000 // 2 minutos en ms
#define maxStelMeasurements 8
// #define maxTwaMeasurements 240 
#define NUM_GASES 2 // Solo para debugear, quitar después

OLED pantalla(ON_OFF_PANTALLA, INT0); // Crea un oled_display
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER); // Crea los indicadores


//Constantes de gases
/*
#define CO_PICO
#define NO2_PICO
#define CO_STEL
#define NO2_STEL
*/

volatile bool buttonPressed = false; // Botón de interrupción presionado

// Estructura GasData
struct Gas 
{
  float newGasData;
  float stelCalculado;
  const float stelMax;
  float gasMeasurements[maxStelMeasurements];

  // Constructor para inicializar los miembros const
    Gas(float max) : stelMax(max) {
      // Inicializa los demás miembros a 0
      newGasData = 0.0;
      stelCalculado = 0.0;

      // Inicializa los arrays a 0.0
      for (uint8_t i = 0; i < maxStelMeasurements; i++) {
        gasMeasurements[i] = 0.0;
      }
    }
};


void setup() {
  // Serial.begin(9600);
  pinMode(COLOR_LED, OUTPUT);
  pinMode(INT0, INPUT);
  pinMode(EN_CO, OUTPUT);
  pinMode(EN_NO2, OUTPUT);
  pinMode(ON_OFF_SENSORES, OUTPUT);

  // Alimentar sensores para calentar
  digitalWrite(ON_OFF_SENSORES, HIGH);
  digitalWrite(EN_CO, HIGH);
  digitalWrite(EN_NO2, HIGH);

  
  indicadores.begin(); // Inicializa los indicadores
  digitalWrite(ON_OFF_BUZZER, HIGH);
  digitalWrite(ON_OFF_LED, HIGH);
  digitalWrite(ON_OFF_VIB, HIGH);
  indicadores.patron_inicio(); // Prender los leds, vibrador y buzzer en un patrón 
  pantalla.begin(); // Inicia la pantalla (imprime logo PUCP y subterrasafe)
}

uint8_t i = 0; // Variable para fors
void loop() {

  char selected_mode = pantalla.selectMode();
  pantalla.calentandoScreen(); // Dos veces
  
  uint8_t stelIndex = 0; // Índice circular para STEL

  unsigned long int lastMeasurementTime = 0;
  
  // Gases en este orden: CO, NO2, O2
  Gas gases[NUM_GASES] = {
    Gas(200.0),   // CO
    Gas(5.0),   // NO2
    // Gas(20.0)    // O2
  };
  
  attachInterrupt(digitalPinToInterrupt(INT0), onButtonPress, RISING);
  while(1)
  {
    unsigned long int currentTime = millis();
    
    if (currentTime - lastMeasurementTime >= measurementInterval) { // Cada 2 minutos
      
      lastMeasurementTime = currentTime; // Actualiza el tiempo 
      
      for (i = 0; i < NUM_GASES; i++)
      {
        gases[i].newGasData = GasData();
      }
            
      if (selected_mode == 'S') // STEL seleccionado
      {
        for (uint8_t j = 0; j < NUM_GASES; j++) // Realiza el cálculo de stel para los 3 gases
        {
          gases[j].gasMeasurements[stelIndex] = gases[j].newGasData;
        
          gases[j].stelCalculado = 0.0;
          for (i = 0; i < maxStelMeasurements; i++) {
            gases[j].stelCalculado += gases[j].gasMeasurements[i];
          }
          gases[j].stelCalculado /= maxStelMeasurements;

          // Activación de alarma
          
          if (gases[j].stelCalculado > gases[j].stelMax) {
            // indicadores.lectura_alta();
            indicadores.alarma();
          }
          
        }
        // Actualiza el índice 
        stelIndex = (stelIndex + 1) % maxStelMeasurements; // Índice circular
      }
      
    }
        
    if (buttonPressed == true) // Se presionó el botón 
    {
      // pantalla.displayLecturas(gases[0].newGasData, gases[1].newGasData, gases[2].newGasData); // Mostrar las lecturas
      // Display STEL
      pantalla.displayLecturas(gases[0].stelCalculado, gases[1].stelCalculado, gases[2].stelCalculado);
      buttonPressed = false;
    }
  }     
}


float GasData() {
  // Simulación de una función que devuelve un valor de gas medido
  // En un caso real, esta función obtendría datos del sensor
  return random(10, 50); // Devuelve un valor aleatorio entre 10 y 50 ppm
}

void onButtonPress()
{
  buttonPressed = true;
}