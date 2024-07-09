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

OLED pantalla(ON_OFF_PANTALLA, INT0); // Crea un oled_display
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER); // Crea los indicadores

char selected_mode;

const unsigned long measurementInterval = 120000; // 2 minutos en milisegundos
const int stelPeriod = 15; // Periodo de 15 minutos para STEL
const int maxStelMeasurements = stelPeriod / 2; // 15 minutos / 2 minutos = 7.5 -> redondeamos a 8
float gasMeasurements[maxStelMeasurements]; // Array para almacenar las mediciones de STEL
int stelIndex = 0; // Índice circular para STEL

const int twaPeriod = 8 * 60; // Periodo de 8 horas en minutos
const int twaInterval = 2; // 2 minutos para almacenar un valor de TWA
const int maxTwaMeasurements = twaPeriod / twaInterval; // 8 horas / 2 minutos = 240
float twaMeasurements[maxTwaMeasurements]; // Array para almacenar los valores de TWA
int twaIndex = 0; // Índice circular para TWA

float valorPico = 0.0; // Variable para almacenar el valor pico

const float STELmax = 30.0; // Valor máximo permitido para STEL en ppm
const float TWAmax = 20.0; // Valor máximo permitido para TWA en ppm

unsigned long lastMeasurementTime = 0;

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

  unsigned long int startTime = millis();
  indicadores.begin();
  pantalla.begin();
  indicadores.patron_inicio();
  selected_mode = pantalla.selectMode(); // El usuario selecciona TWA o STEL
  
  for (int i = 0; i < maxStelMeasurements; i++) {
    gasMeasurements[i] = 0.0;
  }
  for (int i = 0; i < maxTwaMeasurements; i++) {
    twaMeasurements[i] = 0.0;
  }
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastMeasurementTime >= measurementInterval) {
    lastMeasurementTime = currentTime;

    // Obtener una nueva medición de gas
    float newGasData = GasData();

    // Actualizar el valor pico
    if (newGasData > valorPico) {
      valorPico = newGasData;
    }

    // Imprimir el valor pico
    Serial.print("Valor Pico: ");
    Serial.print(valorPico, 2);
    Serial.println(" ppm");

    // Actualizar las mediciones almacenadas para STEL
    gasMeasurements[stelIndex] = newGasData;
    stelIndex = (stelIndex + 1) % maxStelMeasurements; // Índice circular

    // Calcular el STEL
    float stel = 0.0;
    for (int i = 0; i < maxStelMeasurements; i++) {
      stel += gasMeasurements[i];
    }
    stel /= maxStelMeasurements;

    // Imprimir el valor de STEL
    Serial.print("STEL: ");
    Serial.print(stel, 2);
    Serial.println(" ppm");

    // Comparar con STELmax
    if (stel > STELmax) {
      Serial.println("Activar alarma: STEL excedido!");
    }

    // Actualizar las mediciones almacenadas para TWA
    twaMeasurements[twaIndex] = newGasData;
    twaIndex = (twaIndex + 1) % maxTwaMeasurements; // Índice circular

    // Calcular el TWA
    float twa = 0.0;
    for (int i = 0; i < maxTwaMeasurements; i++) {
      twa += twaMeasurements[i];
    }
    twa /= maxTwaMeasurements;

    // Imprimir el valor de TWA
    Serial.print("TWA: ");
    Serial.print(twa, 2);
    Serial.println(" ppm");

    // Comparar con TWAmax
    if (twa > TWAmax) {
      Serial.println("Activar alarma: TWA excedido!");
    }
  }
}

float GasData() {
  // Simulación de una función que devuelve un valor de gas medido
  // En un caso real, esta función obtendría datos del sensor
  return random(10, 50); // Devuelve un valor aleatorio entre 10 y 50 ppm
}
