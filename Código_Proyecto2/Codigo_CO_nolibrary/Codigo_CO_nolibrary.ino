#define ADC_PIN A0
#define POWER_PIN 10

const float Vcc = 5.0;           // Supply voltage
const float Rl = 0.82;           // Load resistance in kOhms (820 Ω from the datasheet)
const float Vadc_max = 1023.0;   // ADC max value (10-bit ADC)
const float CO_Rs0 = 100.0;      // Typical sensor resistance in clean air for CO (value from the datasheet)

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
  float ppm = pow(10, ((log10(ratio) - 0.54) / (-0.47))); // Adjust based on actual datasheet curve
  return ppm;
}

void setup() {
  Serial.begin(115200);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH); // Power on the sensor
  delay(60000); // Allow the sensor to warm up for 1 minute
}

void loop() {
  float Vrl = readSensorVoltage(ADC_PIN);
  float Rs = calculateSensorResistance(Vrl);
  float CO_ppm = calculateCOPPM(Rs);
  
  Serial.print("CO Concentration: ");
  Serial.print(CO_ppm);
  Serial.println(" PPM");

  delay(1000);
}
