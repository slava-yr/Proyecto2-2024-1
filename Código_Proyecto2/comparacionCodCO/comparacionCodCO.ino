#include "DFRobot_MICS.h"

#define ADC_PIN A0
#define POWER_PIN 10
#define CALIBRATION_TIME 1 // Calibration time in minutes

const float Vcc = 5.0;           // Supply voltage
const float Rl = 10;           // Load resistance in kOhms (820 Ω from the datasheet)
const float Vadc_max = 1023.0;   // ADC max value (10-bit ADC)
const float CO_Rs0 = 100.0;      // Typical sensor resistance in clean air for CO (value from the datasheet)

// Initialize the library object for the ADC version of the sensor
DFRobot_MICS_ADC mics(ADC_PIN, POWER_PIN);

// Function to read the analog voltage
float readSensorVoltage(int pin) {
  int sensorValue = analogRead(pin);
  float voltage = sensorValue * (Vcc / Vadc_max);
  return voltage;
}

// Function to calculate the sensor resistance
float calculateSensorResistance(float Vrl) {
  float Rs = ((Vcc - Vrl) / Vrl) * Rl;
  return Rs;
}

// Function to calculate CO PPM using the datasheet's characteristic curve
float calculateCOPPM(float Rs) {
  // Using the logarithmic relationship provided in the datasheet
  float ratio = Rs / CO_Rs0;
  float ppm = pow(10, ((log10(ratio) - 0.54) / (-0.47))); // Example formula, adjust based on actual datasheet curve
  return ppm;
}

void setup() {
  Serial.begin(115200);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW); // Power on the sensor

  while(!Serial);
  
  // Initialize the library
  while(!mics.begin()){
    Serial.println("No devices found!");
    delay(1000);
  }
  Serial.println("Device connected successfully!");

  uint8_t mode = mics.getPowerState();
  if(mode == SLEEP_MODE){
    mics.wakeUpMode();
    Serial.println("Wake up sensor success!");
  } else {
    Serial.println("The sensor is already in wake up mode");
  }

  // Wait for the sensor to warm up
  while(!mics.warmUpTime(CALIBRATION_TIME)){
    Serial.println("Please wait until the warm-up time is over!");
    delay(1000);
  }
}

void loop() {
  // Get CO concentration using custom code
  float Vrl = readSensorVoltage(ADC_PIN);
  float Rs = calculateSensorResistance(Vrl);
  float CO_ppm_custom = calculateCOPPM(Rs);

  // Get CO concentration using the library
  float CO_ppm_library = mics.getGasData(CO);

  // Print both results
  Serial.print("CO Concentration (Custom): ");
  Serial.print(CO_ppm_custom);
  Serial.print(" PPM, CO Concentration (Library): ");
  Serial.print(CO_ppm_library);
  Serial.println(" PPM");

  delay(1000);
}
