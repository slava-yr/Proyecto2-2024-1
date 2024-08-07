const unsigned long measurementInterval = 120000; // 2 minutos en milisegundos
const int stelPeriod = 15; // Periodo de 15 minutos para STEL
const int maxStelMeasurements = stelPeriod / 2; // 15 minutos / 2 minutos = 7.5 -> redondeamos a 8
float gasMeasurements[maxStelMeasurements]; // Array para almacenar las mediciones de STEL
int stelIndex = 0; // Índice circular para STEL

const int twaPeriod = 8 * 60; // Periodo de 8 horas en minutos
const int twaInterval = 2; // 2 minutos para almacenar un valor de TWA
const int maxTwaMeasurements = twaPeriod / twaInterval; // 8 horas / 2 minutos = 240
float twaMeasurements[maxTwaMeasurements]; // Array para almacenar los valores de TWA
int twaIndex = 0; // Índice circular para TWA

float valorPico = 0.0; // Variable para almacenar el valor pico

const float STELmax = 30.0; // Valor máximo permitido para STEL en ppm
const float TWAmax = 20.0; // Valor máximo permitido para TWA en ppm

unsigned long lastMeasurementTime = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < maxStelMeasurements; i++) {
    gasMeasurements[i] = 0.0;
  }
  for (int i = 0; i < maxTwaMeasurements; i++) {
    twaMeasurements[i] = 0.0;
  }
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastMeasurementTime >= measurementInterval) {
    lastMeasurementTime = currentTime;

    // Obtener una nueva medición de gas
    float newGasData = GasData();

    // Actualizar el valor pico
    if (newGasData > valorPico) {
      valorPico = newGasData;
    }

    // Imprimir el valor pico
    Serial.print("Valor Pico: ");
    Serial.print(valorPico, 2);
    Serial.println(" ppm");

    // Actualizar las mediciones almacenadas para STEL
    gasMeasurements[stelIndex] = newGasData;
    stelIndex = (stelIndex + 1) % maxStelMeasurements; // Índice circular

    // Calcular el STEL
    float stel = 0.0;
    for (int i = 0; i < maxStelMeasurements; i++) {
      stel += gasMeasurements[i];
    }
    stel /= maxStelMeasurements;

    // Imprimir el valor de STEL
    Serial.print("STEL: ");
    Serial.print(stel, 2);
    Serial.println(" ppm");

    // Comparar con STELmax
    if (stel > STELmax) {
      Serial.println("Activar alarma: STEL excedido!");
    }

    // Actualizar las mediciones almacenadas para TWA
    twaMeasurements[twaIndex] = newGasData;
    twaIndex = (twaIndex + 1) % maxTwaMeasurements; // Índice circular

    // Calcular el TWA
    float twa = 0.0;
    for (int i = 0; i < maxTwaMeasurements; i++) {
      twa += twaMeasurements[i];
    }
    twa /= maxTwaMeasurements;

    // Imprimir el valor de TWA
    Serial.print("TWA: ");
    Serial.print(twa, 2);
    Serial.println(" ppm");

    // Comparar con TWAmax
    if (twa > TWAmax) {
      Serial.println("Activar alarma: TWA excedido!");
    }
  }
}

float GasData() {
  // Simulación de una función que devuelve un valor de gas medido
  // En un caso real, esta función obtendría datos del sensor
  return random(10, 50); // Devuelve un valor aleatorio entre 10 y 50 ppm
}
