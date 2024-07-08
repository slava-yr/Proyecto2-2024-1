#define ADC_PIN A0
#define POWER_PIN 10

const float Vcc = 5.0;           // Supply voltage
const float Rl = 0.82;           // Load resistance in kOhms (820 Ω from the datasheet)
const float Vadc_max = 1023.0;   // ADC max value (10-bit ADC)
const float NO2_Rs0 = 10.0;      // Typical sensor resistance in clean air for NO2 (value from the datasheet)

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

// Function to calculate NO2 PPM using the datasheet's characteristic curve
float calculateNO2PPM(float Rs) {
  // Using the logarithmic relationship provided in the datasheet
  float ratio = Rs / NO2_Rs0;
  float ppm = pow(10, ((log10(ratio) - 0.50) / (-0.31))); // Adjusted based on actual datasheet curve
  return ppm;
}

void setup() {
  Serial.begin(115200);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW); // Power on the sensor
  delay(200); // Allow the sensor to warm up for 1 minute
}

void loop() {
  float Vrl = readSensorVoltage(ADC_PIN);
  float Rs = calculateSensorResistance(Vrl);
  float NO2_ppm = calculateNO2PPM(Rs);
  
  Serial.print("NO2 Concentration: ");
  Serial.print(NO2_ppm);
  Serial.println(" PPM");

  delay(1000);
}
