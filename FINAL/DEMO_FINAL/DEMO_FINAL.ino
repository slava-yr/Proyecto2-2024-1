/*----------------------------------------
DEMO FINAL PARA LA PRESENTACIÓN
- Mostrar selección de modo
- Interrupción por botón
- Imprimir "mediciones" actuales
- Alarma cuando se superen los valores pico (ACTUALIZARLOS)
- Alarma auditiva, vibratoria y luminosa
-----------------------------------------*/
#include "perifericos.h"

// Pines para la tira led
#define ON_OFF_LED A3 // Pin para habilitar alimentación de los LED
#define COLOR_LED 3 // Pin pwm para el color de los leds

// Pines de habilitación de alimentación
#define ON_OFF_BUZZER 4   // Pin habilitador de buzzer
#define ON_OFF_PANTALLA 6 // Pin habilitador de pantalla OLED
#define ON_OFF_SENSORES 7 // Pin que controla la alimentación de los sensores
#define ON_OFF_VIB 10     // Pin habilitador de vibrador

// Pin de interrupción
#define INT0 2

// Pines de lectura del ADC
#define BAT_VOLT A6 // Pin de lectura de la tensión en la batería
#define CO_OUT  A0  // Pin de lectura de valor de sensor CO
#define NO2_OUT  A1 // Pin de lectura de valor de sensor NO2
#define O2_OUT  A2  // Pin de lectura de valor de sensor O2

#define measurementInterval 6000 // 120000 // 2 minutos en ms
#define NUM_GASES 3 // Solo para debugear, quitar después

OLED pantalla(ON_OFF_PANTALLA, INT0); // Crea un oled_display
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER); // Crea los indicadores

volatile bool buttonPressed = false; // Botón de interrupción presionado

// Estructura GasData
struct Gas 
{
  float newGasData;
  const float picoMax;
  float picoRegistrado;

  // Constructor para inicializar los miembros const
    Gas(float max) : picoMax(max) {
      // Inicializa los demás miembros a 0
      newGasData = 0.0;
      picoRegistrado = 0.0;
    }
};

void setup() {
  // put your setup code here, to run once:
  pinMode(COLOR_LED, OUTPUT);
  pinMode(INT0, INPUT);
  
  indicadores.begin(); // Inicializa los indicadores
  digitalWrite(ON_OFF_BUZZER, HIGH);
  digitalWrite(ON_OFF_LED, HIGH);
  digitalWrite(ON_OFF_VIB, HIGH);
  indicadores.patron_inicio(); // Prender los leds, vibrador y buzzer en un patrón 
  pantalla.begin(); // Inicia la pantalla (imprime logo PUCP y subterrasafe)
}

uint8_t i;
void loop() {
  char selected_mode = pantalla.selectMode(); // Selecciona modo de operación
  pantalla.calentandoScreen(); // Dos veces
  
  // uint8_t stelIndex = 0; // Índice circular para STEL
  unsigned long int lastMeasurementTime = 0;
  
  // Gases en este orden: CO, NO2, O2
  Gas gases[NUM_GASES] = {
    Gas(200.0),   // CO
    Gas(5.0),   // NO2
    Gas(20.0)    // O2
  };

  attachInterrupt(digitalPinToInterrupt(INT0), onButtonPress, RISING); // Interrupciones por pulsador
  while(1)
  {
    unsigned long int currentTime = millis();
    
    if (currentTime - lastMeasurementTime >= measurementInterval) { // Cada 2 minutos
      
      lastMeasurementTime = currentTime; // Actualiza el tiempo 


      gases[0].newGasData = GasData(10, 200); // CO;
      gases[1].newGasData = GasData(1, 5); // NO2
      gases[2].newGasData = GasData(10, 22); // O2  
      
      for (i = 0; i < NUM_GASES; i++)
      {
        // Actualizar valores pico
        if (gases[i].newGasData > gases[i].picoRegistrado)
        {
          gases[i].picoRegistrado = gases[i].newGasData;
        }

        // Comprobación de límites
        if (gases[i].picoRegistrado > gases[i].picoMax) // Si supera el límite
        {
          // ALARMA
          indicadores.alarma();
          // Imprimir en pantalla el gas que supera
        }
      }
    }
    if (buttonPressed == true) // Se presionó el botón 
    {
      pantalla.displayLecturas(gases[0].newGasData, gases[1].newGasData, gases[2].newGasData); // Mostrar las lecturas
      // Mostrar valores pico
      pantalla.displayPicos(gases[0].picoRegistrado, gases[1].picoRegistrado, gases[2].picoRegistrado);
      buttonPressed = false;
    }
  }
}

float GasData(uint8_t min, uint8_t max) {
  return random(min*10, max*10)/10.0; // Devuelve un valor aleatorio entre 10 y 50 ppm
}

void onButtonPress()
{
  buttonPressed = true;
}
