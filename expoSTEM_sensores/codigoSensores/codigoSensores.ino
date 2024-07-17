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

float getCOPPM() {
  // Leer el voltaje del sensor
  float sensorVoltage = readSensorVoltage(CO_OUT);
  
  // Calcular la resistencia del sensor
  float sensorResistance = calculateSensorResistance(sensorVoltage, Rl_CO);
  
  // Calcular el ratio de la resistencia del sensor respecto a Rs0
  float ratio = sensorResistance / CO_Rs0;
  
  // Calcular el PPM usando la fórmula del datasheet
  float coPPM = pow((3.5 / ratio), 1.179);
  
  return coPPM;
}

//FUNCIONES NO2 
// Función para calcular NO2 PPM usando la curva característica del datasheet
float getNO2PPM() {
  // Leer el voltaje del sensor
  float sensorVoltage = readSensorVoltage(NO2_OUT);
  
  // Calcular la resistencia del sensor
  float sensorResistance = calculateSensorResistance(sensorVoltage, Rl_NO2);
  
  // Calcular el ratio de la resistencia del sensor respecto a Rs0
  float ratio = sensorResistance / NO2_Rs0;
  
  // Calcular el PPM usando la fórmula del datasheet
  float no2PPM = (3.0 / 20.0) * ratio;
  
  return no2PPM;
}

//FUNCIONES O2
// Función para leer el voltaje de salida del sensor O2
float getO2Concentration() {
    long sum = 0;
    for (int i = 0; i < 32; i++) {
        sum += analogRead(O2_OUT);
    }
    sum >>= 5; // División por 32 utilizando bit shift
    float MeasuredVout = sum * (VRefer / 1023.0);
    float Concentration_Percentage = (MeasuredVout * 0.21 / 2.0) * 100;
    return Concentration_Percentage;
}

void setup()
{

}

void loop()
{

}