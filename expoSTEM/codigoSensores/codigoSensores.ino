#define CO_OUT  A0  // Pin de lectura de valor de sensor CO
#define NO2_OUT  A1 // Pin de lectura de valor de sensor NO2
#define O2_OUT  A2  // Pin de lectura de valor de sensor O2

const float Vcc = 5.0;           // Voltaje de suministro
const float Vadc_max = 1023.0;   // Valor máximo del ADC (ADC de 10 bits)
const float VRefer = 3.3;       // Voltaje de referencia del ADC

const float Rl_CO = 0.82;           // Resistencia de carga en kOhms (820 Ω del datasheet)
const float Rl_NO2 = 10.0;           // Resistencia de carga en kOhms (820 Ω del datasheet)
const float CO_Rs0 = 100.0;      // Resistencia típica del sensor en aire limpio para CO (valor del datasheet)
const float NO2_Rs0 = 10.0;      // Resistencia típica del sensor en aire limpio para NO2 (valor del datasheet)

//FUNCIONES GENERAL 
float readSensorVoltage(int pin) {
  int sensorValue = analogRead(pin);
  float voltage = sensorValue * (Vcc / Vadc_max);
  return voltage;
}

// Función para calcular la resistencia del sensor
float calculateSensorResistance(float Vrl, float Rl) {
  float Rs = ((Vcc - Vrl) / Vrl) * Rl;
  return Rs;
}


//FUNCIONES CO 
// Función para calcular CO PPM usando la fórmula correcta
float calculateCOPPM(float Rs) {
  float ratio = Rs / CO_Rs0;
  float ppm = pow((3.5 / ratio), 1.179); // Usando la fórmula del datasheet
  return ppm;
}

// Función para obtener PPM de CO
float getCOPPM() {
  float sensorVoltage = readSensorVoltage(CO_OUT);
  float sensorResistance = calculateSensorResistance(sensorVoltage, Rl_CO);
  float coPPM = calculateCOPPM(sensorResistance);
  return coPPM;
}

//FUNCIONES NO2 
// Función para calcular NO2 PPM usando la curva característica del datasheet
float calculateNO2PPM(float Rs) {
  // Usando la relación proporcionada en el datasheet
  float ratio = Rs / NO2_Rs0;
  float ppm = (3.0 / 20.0) * ratio; // Ajustado basado en la curva actual del datasheet
  return ppm;
}

// Función para obtener PPM de NO2
float getNO2PPM() {
  float sensorVoltage = readSensorVoltage(NO2_OUT);
  float sensorResistance = calculateSensorResistance(sensorVoltage, Rl_NO2);
  float no2PPM = calculateNO2PPM(sensorResistance);
  return no2PPM;
}

//FUNCIONES O2
// Función para leer el voltaje de salida del sensor O2
float readO2Vout() {
    long sum = 0;
    for (int i = 0; i < 32; i++) {
        sum += analogRead(O2_OUT);
    }
    sum >>= 5;
    float MeasuredVout = sum * (VRefer / 1023.0);
    return MeasuredVout;
}

// Función para obtener la concentración de O2 en porcentaje
float getO2Concentration() {
  float MeasuredVout = readO2Vout();
  float Concentration_Percentage = (MeasuredVout * 0.21 / 2.0) * 100;
  return Concentration_Percentage;
}

void setup()
{

}

void loop()
{

}