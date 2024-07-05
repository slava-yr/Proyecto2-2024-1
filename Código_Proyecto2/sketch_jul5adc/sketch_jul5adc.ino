// Pines de conexión
const int sensorPin = A0;  // Pin analógico para la lectura del sensor
const int heaterPin = 9;   // Pin digital para el control del calentador

// Variables de cálculo
float sensorResistance;   // Resistencia del sensor
float Rs_air = 10000;     // Resistencia del sensor en aire limpio (ohmios), ajustar según el datasheet o calibración
float Vout;               // Voltaje de salida del sensor
float ppmCO;              // Concentración de CO en ppm
const float Vcc = 5.0;    // Voltaje de alimentación
const float R_LOAD = 1000; // Resistor de carga en ohmios

void setup() {
  // Inicialización de pines
  pinMode(sensorPin, INPUT);
  pinMode(heaterPin, OUTPUT);
  
  // Inicialización del monitor serial
  Serial.begin(9600);

  // Activar el calentador del sensor
  digitalWrite(heaterPin, LOW);
}

void loop() {
  // Leer el valor del ADC
  int adcValue = analogRead(sensorPin);
  
  // Calcular el voltaje de salida del sensor
  Vout = (adcValue * Vcc) / 1023.0;
  
  // Calcular la resistencia del sensor
  sensorResistance = (Vcc / Vout - 1) * R_LOAD;
  
  // Calcular la concentración de CO en ppm usando la fórmula del datasheet
  ppmCO = 1000 * pow((sensorResistance / Rs_air), -1.4); // La constante y el exponente deben ajustarse según las curvas del datasheet

  // Mostrar las variables en el monitor serial
  Serial.print("ADC Value: ");
  Serial.println(adcValue);
  Serial.print("Vout: ");
  Serial.print(Vout);
  Serial.println(" V");
  Serial.print("Sensor Resistance: ");
  Serial.print(sensorResistance);
  Serial.println(" ohms");
  Serial.print("CO Concentration: ");
  Serial.print(ppmCO);
  Serial.println(" ppm");

  // Esperar antes de la próxima lectura
  delay(1000);
}
