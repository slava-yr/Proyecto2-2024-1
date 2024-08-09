#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
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
#define maxTwaMeasurements 240 
#define NUM_GASES 3 // Solo para debugear, quitar después

OLED pantalla(ON_OFF_PANTALLA, INT0); // Crea un oled_display
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER); // Crea los indicadores

int program_counter = 0; // PARA DEBUG. BORRAR 
char selected_mode; // TWA o STEL

uint8_t stelIndex = 0; // Índice circular para STEL
uint8_t twaCount = 0; // Conteo de mediciones para TWA

unsigned long int lastMeasurementTime = 0;
bool isFirstTwaMeasurement = true; // Indicador para la primera medición TWA
const float alpha = 2.0 / (240 + 1); // Factor de suavizado para el EMA
bool buttonPressed; // Botón de interrupción presionado

// Estructura GasData
struct GasData 
{
  float valorPicoRegistrado;
  const float valorPicoPermitido;
  float stelCalculado;
  float twaSum;
  float twaAvg; 
  float twaEMA; 
  float twaValue;
  const float stelMax;
  const float twaMax;
  float gasMeasurements[maxStelMeasurements];

  // Constructor para inicializar los miembros const
  GasData(float vpp, float sm, float tm)
    : valorPicoPermitido(vpp), stelMax(sm), twaMax(tm) {
      // Inicializa los demás miembros a 0
      valorPicoRegistrado = 0.0;
      stelCalculado = 0.0;
      twaSum = 0.0;
      twaAvg = 0.0;
      twaEMA = 0.0;
      twaValue = 0.0;
      // Inicializa los arrays a 0.0
      for (int i = 0; i < maxStelMeasurements; i++) {
        gasMeasurements[i] = 0.0;
      }
    }
};

GasData gases[] // Gases en este orden: CO, NO2, O2
// Inicializando las constantes
{
  GasData(200.0, 200.0, 25.0),  // CO (pico, stelmax, twamax)
  //GasData(5.0, 5.0, 3.0),      // NO2
 // GasData(22.5, 0.0, 19.5)     // O2
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

  
  indicadores.begin();
  digitalWrite(ON_OFF_BUZZER, HIGH);
  digitalWrite(ON_OFF_LED, HIGH);
  digitalWrite(ON_OFF_PANTALLA, HIGH);
  digitalWrite(ON_OFF_VIB, HIGH);
  indicadores.patron_inicio();
  pantalla.begin();
  
  selected_mode = pantalla.selectMode(); // El usuario selecciona TWA o STEL (para imprimir)
  // unsigned long int startTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastMeasurementTime >= measurementInterval) { // Cada 2 minutos
    program_counter+=1;
    lastMeasurementTime = currentTime; // Actualiza el tiempo 
    Serial.println(program_counter);
    // Obtener una nueva medición de cada gas
    for (uint8_t i = 0; i < NUM_GASES; i++)
    {
      newGasData[i] = GasData();
      Serial.print("GasData ");
      Serial.print(i);
      Serial.print(" : ");
      Serial.println(newGasData[i]);
    }
     
    

    // Actualizar el valor pico (de cada gas)
    for (uint8_t i = 0; i < NUM_GASES; i++)
    {
      if (newGasData[i] > gases[i].valorPicoRegistrado) {
        gases[i].valorPicoRegistrado = newGasData[i];
        // Imprimir el valor pico
        Serial.print("Valor Pico: ");
        Serial.print(gases[i].valorPicoRegistrado, 2);
        Serial.println(" ppm");
      }    
    }
    
    if (selected_mode == 'S') // STEL seleccionado
    {
      Serial.println("STEL");
      for (uint8_t j = 0; j < NUM_GASES; j++) // Realiza el cálculo de stel para los 3 gases
      {
        // Actualizar las mediciones almacenadas para STEL
        gases[j].gasMeasurements[stelIndex] = newGasData[j];
      
        // Calcular el STEL
        gases[j].stelCalculado = 0.0;
        for (int i = 0; i < maxStelMeasurements; i++) {
          gases[j].stelCalculado += gases[j].gasMeasurements[i];
        }
        gases[j].stelCalculado /= maxStelMeasurements;

        Serial.print("STEL: ");
        Serial.print(gases[j].stelCalculado, 2);
        Serial.println(" ppm");

        // Activación de alarma
        if (gases[j].stelCalculado > gases[j].stelMax) {
          Serial.println("Activar alarma: STEL excedido!");
        }
      }
      // Actualiza el índice 
      stelIndex = (stelIndex + 1) % maxStelMeasurements; // Índice circular
    }

    else // TWA seleccionado
    {
      Serial.println("TWA");
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
        
        Serial.print("TWA: ");
        Serial.print(gases[j].twaValue, 2);
        Serial.println(" ppm");

        // ACTIVACIÓN DE ALARMA
        if (gases[j].twaValue > gases[j].twaMax) {
          Serial.println("Activar alarma: TWA excedido!");
        }
      }
      twaCount++; // Aumenta la cuenta del twa
    } 
  }
  if (buttonPressed == true) // Se presionó el botón 
  {
    buttonPressed = false; // Actualiza el estado
    pantalla.displayLecturas(newGasData[0], newGasData[1], newGasData[2]);
  }


  // if de bandera de interrupción
  // Mostrar en la pantalla, etc.
}

float GasData() {
  // Simulación de una función que devuelve un valor de gas medido
  // En un caso real, esta función obtendría datos del sensor
  return random(10, 50); // Devuelve un valor aleatorio entre 10 y 50 ppm
}

// Función de interrupción
void buttonPressISR() {
  buttonPressed = true;
}