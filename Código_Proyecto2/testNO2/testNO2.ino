#include "DFRobot_MICS.h"

#define CALIBRATION_TIME 3 // Tiempo de calibración predeterminado de tres minutos

// Direcciones I2C para los sensores
#define MICS_I2C_ADDRESS_1 MICS_ADDRESS_0
#define MICS_I2C_ADDRESS_2 MICS_ADDRESS_1

DFRobot_MICS_I2C mics1(&Wire, MICS_I2C_ADDRESS_1);
DFRobot_MICS_I2C mics2(&Wire, MICS_I2C_ADDRESS_2);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Inicialización del primer sensor
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
  float gasdata1 = mics1.getGasData(NO2);
  Serial.print("Sensor 1 NO2 Value: ");
  Serial.print(gasdata1, 1);
  Serial.println(" PPM");

  // Lectura de datos del segundo sensor
  float gasdata2 = mics2.getGasData(NO2);
  Serial.print("Sensor 2 NO2 Value: ");
  Serial.print(gasdata2, 1);
  Serial.println(" PPM");

  delay(1000);
}
