#define ADC_PIN A0
#define POWER_PIN 10

const int Vcc = 5;           // Supply voltage
const float Rl = 0.82;           // Load resistance in kOhms (820 Ω from the datasheet)
const int Vadc_max = 1023;   // ADC max value (10-bit ADC)
const float CO_Rs0 = 100.0;      // Typical sensor resistance in clean air for CO (value from the datasheet)

// Function to read the analog voltage
float readSensorVoltage(int pin) {
  int sensorValue = analogRead(pin);
  float voltage = sensorValue * float(Vcc / Vadc_max);
  return voltage;
}

// Function to calculate the sensor resistance
float calculateSensorResistance(float Vrl) {
  float Rs = ((Vcc - Vrl) / Vrl) * Rl;
  return Rs;
}

// Function to calculate CO PPM using the correct formula
float calculateCOPPM(float Rs) {
  float ratio = Rs / CO_Rs0;
  float ppm = pow((3.5 / ratio), 1.179);
  return ppm;
}

void setup() {
  Serial.begin(115200);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW); // Power on the sensor
  // delay(); // Allow the sensor to warm up for 1 minute
}

void loop() {
  int sensorValue = analogRead(ADC_PIN);
  float Vrl = sensorValue * (Vcc / Vadc_max);
  float Rs = calculateSensorResistance(Vrl);
  float CO_ppm = calculateCOPPM(Rs);
  
  Serial.print("ADC Value: ");
  Serial.print(sensorValue);
  Serial.print(" | CO Concentration: ");
  Serial.print(CO_ppm);
  Serial.println(" PPM");

  delay(1000);
}
