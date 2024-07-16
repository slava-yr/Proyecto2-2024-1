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
#define NUM_GASES 3 // Solo para debugear, quitar después

OLED pantalla(ON_OFF_PANTALLA, INT0); // Crea un oled_display
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER); // Crea los indicadores

volatile bool buttonPressed = false; // Botón de interrupción presionado

// Estructura GasData
struct Gas 
{
  float valorPicoRegistrado;
  const float valorPicoPermitido;
  float stelCalculado;
  const float stelMax;
  float gasMeasurements[maxStelMeasurements];

  // Constructor para inicializar los miembros const
  Gas(float vpp, float sm)
    : valorPicoPermitido(vpp), stelMax(sm) {
      // Inicializa los demás miembros a 0
      valorPicoRegistrado = 0.0;
      stelCalculado = 0.0;
      // Inicializa los arrays a 0.0
      for (int i = 0; i < maxStelMeasurements; i++) {
        gasMeasurements[i] = 0.0;
      }
    }
};

float newGasData[3];
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

  indicadores.begin(); // Inicializa los indicadores
  digitalWrite(ON_OFF_BUZZER, HIGH);
  digitalWrite(ON_OFF_LED, HIGH);
  digitalWrite(ON_OFF_VIB, HIGH);
  indicadores.patron_inicio(); // Prender los leds, vibrador y buzzer en un patrón 
  pantalla.begin(); // Inicia la pantalla (imprime logo PUCP y subterrasafe)
}


void loop() {
  /* NOTAS:
  Al parecer si la memoria dinámica ocupa 40% o más, la pantalla deja de funcionar.
  No estoy seguro del límite, parece estar alrededor de 38%
  Con 34% funciona
  Buscar formas de reducir el consumo de memoria.
  */

  
  uint8_t program_counter = 0; // PARA DEBUG. BORRAR 
  uint8_t stelIndex = 0; // Índice circular para STEL
  // uint8_t twaCount = 0; // Conteo de mediciones para TWA

  unsigned long int lastMeasurementTime = 0;
  // bool isFirstTwaMeasurement = true; // Indicador para la primera medición TWA
  // const float alpha = 2.0 / (240 + 1); // Factor de suavizado para el EMA

  char selected_mode = pantalla.selectMode();
  // Gases en este orden: CO, NO2, O2
  Gas gases[3] = {
    Gas(50.0, 100.0),
    Gas(60.0, 110.0),
    Gas(70.0, 120.0)
  };
  attachInterrupt(digitalPinToInterrupt(INT0), onButtonPress, RISING);
  while(1)
  {
    unsigned long int currentTime = millis();
    if (currentTime - lastMeasurementTime >= measurementInterval) { // Cada 2 minutos
      // program_counter+=1;
      lastMeasurementTime = currentTime; // Actualiza el tiempo 
      // Obtener una nueva medición de cada gas
      for (uint8_t i = 0; i < NUM_GASES; i++)
      {
        newGasData[i] = GasData();
      }
      // Actualizar el valor pico (de cada gas)
      for (uint8_t i = 0; i < NUM_GASES; i++)
      {
        if (newGasData[i] > gases[i].valorPicoRegistrado) {
          gases[i].valorPicoRegistrado = newGasData[i];
        }    
      }
      if (selected_mode == 'S') // STEL seleccionado
      {
        for (uint8_t j = 0; j < NUM_GASES; j++) // Realiza el cálculo de stel para los 3 gases
        {
          gases[j].gasMeasurements[stelIndex] = newGasData[j];
        
          gases[j].stelCalculado = 0.0;
          for (int i = 0; i < maxStelMeasurements; i++) {
            gases[j].stelCalculado += gases[j].gasMeasurements[i];
          }
          gases[j].stelCalculado /= maxStelMeasurements;

          // Activación de alarma
          if (gases[j].stelCalculado > gases[j].stelMax) {
            // ACTIVAR ALARMA
          }
        }
        // Actualiza el índice 
        stelIndex = (stelIndex + 1) % maxStelMeasurements; // Índice circular
      }
      
      else // TWA seleccionado
      {
        /*
        //Serial.println("TWA");
        for (uint8_t j = 0; j < NUM_GASES; j++) // Calcula TWA para los 3 gases
        {
          gases[j].twaSum += newGasData[j];
          
          if (twaCount > maxTwaMeasurements)
          {
            gases[j].twaSum -= gases[j].twaAvg; 
          }

          gases[j].twaAvg = gases[j].twaSum / min(twaCount, maxTwaMeasurements);
          // Actualizar TWA usando el promedio móvil exponencial (EMA)
          if (isFirstTwaMeasurement) {
            gases[j].twaEMA = newGasData[j];
            if (j == 2) isFirstTwaMeasurement = false; // Solo actualiza cuando estén los 3 gases
          } 
          else {
            gases[j].twaEMA = alpha * newGasData[j] + (1 - alpha) * gases[j].twaEMA;
          }
          gases[j].twaValue = (twaCount <= maxTwaMeasurements) ? gases[j].twaAvg : gases[j].twaEMA;
          
          //Serial.print("TWA: ");
          //Serial.print(gases[j].twaValue, 2);
          //Serial.println(" ppm");

          // ACTIVACIÓN DE ALARMA
          if (gases[j].twaValue > gases[j].twaMax) {
            //Serial.println("Activar alarma: TWA excedido!");
          }
          
        }
        twaCount++; // Aumenta la cuenta del twa
        */
      }
    }
  }
    
    if (buttonPressed == true) // Se presionó el botón 
    {
      pantalla.displayLecturas(gases[0].gasMeasurements[stelIndex-1], gases[1].gasMeasurements[stelIndex-1], gases[2].gasMeasurements[stelIndex-1]); // Mostrar las lecturas
      buttonPressed = false;
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