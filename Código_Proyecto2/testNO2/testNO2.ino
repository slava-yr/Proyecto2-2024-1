#include "DFRobot_MICS.h"

#define CALIBRATION_TIME 3 // Tiempo de calibración predeterminado de tres minutos

// Definición de pines para los sensores
#define SENSOR1_ADC_PIN   A1
#define SENSOR1_POWER_PIN 10
#define SENSOR2_ADC_PIN   A2
#define SENSOR2_POWER_PIN 11

DFRobot_MICS_ADC mics1(SENSOR1_ADC_PIN, SENSOR1_POWER_PIN);
DFRobot_MICS_ADC mics2(SENSOR2_ADC_PIN, SENSOR2_POWER_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Inicialización del primer sensor
  pinMode(SENSOR1_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR1_POWER_PIN, HIGH);

  while (!mics1.begin()) {
    Serial.println("No Devices for Sensor 1!");
    delay(1000);
  }
  Serial.println("Sensor 1 connected successfully!");

  uint8_t mode1 = mics1.getPowerState();
  if (mode1 == SLEEP_MODE) {
    mics1.wakeUpMode();
    Serial.println("Wake up Sensor 1 success!");
  } else {
    Serial.println("Sensor 1 is in wake up mode");
  }

  // Inicialización del segundo sensor
  pinMode(SENSOR2_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR2_POWER_PIN, HIGH);

  while (!mics2.begin()) {
    Serial.println("No Devices for Sensor 2!");
    delay(1000);
  }
  Serial.println("Sensor 2 connected successfully!");

  uint8_t mode2 = mics2.getPowerState();
  if (mode2 == SLEEP_MODE) {
    mics2.wakeUpMode();
    Serial.println("Wake up Sensor 2 success!");
  } else {
    Serial.println("Sensor 2 is in wake up mode");
  }

  // Calentamiento de los sensores
  while (!mics1.warmUpTime(CALIBRATION_TIME)) {
    Serial.println("Please wait until the warm-up time for Sensor 1 is over!");
    delay(1000);
  }

  while (!mics2.warmUpTime(CALIBRATION_TIME)) {
    Serial.println("Please wait until the warm-up time for Sensor 2 is over!");
    delay(1000);
  }
}

void loop() {
  // Lectura de datos del primer sensor
  float gasdata1 = mics1.getGasData(C2H5OH);
  Serial.print("Sensor 1 Ethanol Value: ");
  Serial.print(gasdata1, 1);
  Serial.println(" PPM");

  // Lectura de datos del segundo sensor
  float gasdata2 = mics2.getGasData(C2H5OH);
  Serial.print("Sensor 2 Ethanol Value: ");
  Serial.print(gasdata2, 1);
  Serial.println(" PPM");

  delay(1000);
}
