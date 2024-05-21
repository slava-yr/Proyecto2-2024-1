#include <DFRobot_OxygenSensor.h>

#include <Wire.h>
#include <DFRobot_OxygenSensor.h>
#include <DFRobot_MICS.h>

// Create sensor objects
DFRobot_MICS_ADC mics5524(A0, 10);
DFRobot_MICS_ADC mics2714(A1, 11);
//DFRobot_MICS_ADC micsMEO2(A2, 12);
DFRobot_OxygenSensor o2Sensor;


void setup() {
  Serial.begin(115200);

  // Initialize sensors
  mics5524.begin();
  mics2714.begin();
 // micsMEO2.begin();
  o2Sensor.begin(0x70);

  // Calibrate sensors
 // mics5524.calibrate();
//  mics2714.calibrate();
//  meo2.calibrate(20.9);
}

void loop() {
  // Read sensor values
  float mics5524_value = mics5524.readGas();
  float mics2714_value = mics2714.readGas();
  float meo2_value = micsMEO2.readGas();

  // Display sensor values
  Serial.print("MiCS5524: ");
  Serial.print(mics5524_value);
  Serial.print(" ppm\t");
  Serial.print("MiCS2714: ");
  Serial.print(mics2714_value);
  Serial.print(" ppm\t");
  Serial.print("MeO2: ");
  Serial.println(meo2_value);

  // Turn off sensors
  mics5524.sleep();
  mics2714.sleep();
  micsMEO2.sleep();

  // Wait for a while before taking the next reading
  delay(1000);

  // Turn on sensors
  mics5524.wakeup();
  mics2714.wakeup();
  micsMEO2.wakeup();
}
