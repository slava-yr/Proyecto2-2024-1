const unsigned long measurementInterval = 120000; // 2 minutos en milisegundos
const int stelPeriod = 15; // Periodo de 15 minutos para STEL
const int maxStelMeasurements = stelPeriod / 2; // 15 minutos / 2 minutos = 7.5 -> redondeamos a 8
float gasMeasurements[maxStelMeasurements]; // Array para almacenar las mediciones de STEL
int measurementCount = 0; // Contador de mediciones de gas

const int twaPeriod = 8 * 60; // Periodo de 8 horas en minutos
const int twaInterval = 2; // 2 minutos para almacenar un valor de TWA
const int maxTwaMeasurements = twaPeriod / twaInterval; // 8 horas / 2 minutos = 240
float twaMeasurements[maxTwaMeasurements]; // Array para almacenar los valores de TWA
int twaCount = 0; // Contador de mediciones de TWA
int stelCount = 0; // Contador de mediciones de STEL para TWA

float valorPico = 0.0; // Variable para almacenar el valor pico

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
    if (measurementCount < maxStelMeasurements) {
      gasMeasurements[measurementCount] = newGasData;
      measurementCount++;
      Serial.println("Todavía no pasan 15 minutos para el cálculo de STEL");
    } else {
      for (int i = 0; i < maxStelMeasurements - 1; i++) {
        gasMeasurements[i] = gasMeasurements[i + 1];
      }
      gasMeasurements[maxStelMeasurements - 1] = newGasData;

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

      // Actualizar las mediciones almacenadas para TWA
      if (twaCount < maxTwaMeasurements) {
        twaMeasurements[twaCount] = newGasData;
        twaCount++;
      } else {
        for (int i = 0; i < maxTwaMeasurements - 1; i++) {
          twaMeasurements[i] = twaMeasurements[i + 1];
        }
        twaMeasurements[maxTwaMeasurements - 1] = newGasData;
      }

      // Calcular el TWA
      float twa = 0.0;
      for (int i = 0; i < twaCount; i++) {
        twa += twaMeasurements[i];
      }
      if (twaCount > 0) {
        twa /= twaCount;
      }

      // Imprimir el valor de TWA
      Serial.print("TWA: ");
      Serial.print(twa, 2);
      Serial.println(" ppm");
    }
  }
}

float GasData() {
  // Simulación de una función que devuelve un valor de gas medido
  // En un caso real, esta función obtendría datos del sensor
  return random(10, 50); // Devuelve un valor aleatorio entre 10 y 50 ppm
}
