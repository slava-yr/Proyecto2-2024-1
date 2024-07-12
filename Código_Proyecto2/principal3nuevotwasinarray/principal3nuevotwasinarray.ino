#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "perifericos.h"

#define ON_OFF_LED A3
#define COLOR_LED 3
#define ON_OFF_BUZZER 4
#define ON_OFF_PANTALLA 6
#define ON_OFF_SENSORES 7
#define ON_OFF_VIB 10
#define EN_CO 8
#define EN_NO2 9
#define INT0 2
#define BAT_VOLT A6
#define CO_OUT A0
#define NO2_OUT A1
#define O2_OUT A2

OLED pantalla(ON_OFF_PANTALLA, INT0);
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER);

char selected_mode;

const unsigned long measurementInterval = 120000; // 2 minutos en milisegundos
const int stelPeriod = 15; // Periodo de 15 minutos para STEL
const int maxStelMeasurements = stelPeriod / 2; // 15 minutos / 2 minutos = 7.5 -> redondeamos a 8
float gasMeasurements[maxStelMeasurements]; // Array para almacenar las mediciones de STEL
int stelIndex = 0; // Índice circular para STEL

const int maxTwaMeasurements = 240; // Máximo número de mediciones para TWA

float twaSum = 0; // Suma acumulativa para TWA
int twaCount = 0; // Conteo de mediciones para TWA
float twaAvg = 0; // Promedio de TWA

const float alpha = 2.0 / (240 + 1); // Factor de suavizado para el EMA
float twaEMA = 0; // Valor inicial del EMA
bool isFirstTwaMeasurement = true; // Indicador para la primera medición TWA

float valorPico = 0.0; // Variable para almacenar el valor pico

const float STELmax = 30.0; // Valor máximo permitido para STEL en ppm
const float TWAmax = 25.0; // Valor máximo permitido para TWA en ppm

unsigned long lastMeasurementTime = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < maxStelMeasurements; i++) {
    gasMeasurements[i] = 0.0;
  }
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastMeasurementTime >= measurementInterval) {
    lastMeasurementTime = currentTime;

    float newGasData = GasData();

    if (newGasData > valorPico) {
      valorPico = newGasData;
    }

    Serial.print("Valor Pico: ");
    Serial.print(valorPico, 2);
    Serial.println(" ppm");

    // Actualizar las mediciones almacenadas para STEL
    gasMeasurements[stelIndex] = newGasData;
    stelIndex = (stelIndex + 1) % maxStelMeasurements;

    float stel = 0.0;
    for (int i = 0; i < maxStelMeasurements; i++) {
      stel += gasMeasurements[i];
    }
    stel /= maxStelMeasurements;

    Serial.print("STEL: ");
    Serial.print(stel, 2);
    Serial.println(" ppm");

    if (stel > STELmax) {
      Serial.println("Activar alarma: STEL excedido!");
    }

    // Actualizar las mediciones almacenadas para TWA usando promedio móvil
    twaSum += newGasData;
    twaCount++;

    if (twaCount > maxTwaMeasurements) {
      twaSum -= twaAvg;
    }

    twaAvg = twaSum / min(twaCount, maxTwaMeasurements);

    // Actualizar TWA usando el promedio móvil exponencial (EMA)
    if (isFirstTwaMeasurement) {
      twaEMA = newGasData;
      isFirstTwaMeasurement = false;
    } else {
      twaEMA = alpha * newGasData + (1 - alpha) * twaEMA;
    }

    // Seleccionar el método de TWA a retornar
    float twaValue = (twaCount <= maxTwaMeasurements) ? twaAvg : twaEMA;

    Serial.print("TWA: ");
    Serial.print(twaValue, 2);
    Serial.println(" ppm");

    if (twaValue > TWAmax) {
      Serial.println("Activar alarma: TWA excedido!");
    }
  }
}

float GasData() {
  return random(10, 50);
}
