const int measurementInterval = 120000; // 2 minutos en milisegundos
const int stelPeriod = 15; // Periodo de 15 minutos para STEL
const int maxStelMeasurements = stelPeriod / 2; // 15 minutos / 2 minutos = 7.5 -> redondeamos a 8
float gasMeasurements[maxStelMeasurements]; // Array para almacenar las mediciones de STEL
int measurementCount = 0; // Contador de mediciones de gas

const int twaPeriod = 8 * 60; // Periodo de 8 horas en minutos
const int twaInterval = 15; // 15 minutos para almacenar un valor de STEL
const int maxTwaMeasurements = twaPeriod / twaInterval; // 8 horas / 15 minutos = 32
float twaMeasurements[maxTwaMeasurements]; // Array para almacenar los valores de STEL
int twaCount = 0; // Contador de mediciones de TWA
int stelCount = 0; // Contador de mediciones de STEL para TWA

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

      // Actualizar las mediciones almacenadas para TWA después de los primeros 8 STEL
      if (measurementCount >= maxStelMeasurements) {
        stelCount++;
        if (stelCount >= 4) { // Cada 4 mediciones de STEL (8 minutos)
          stelCount = 0;
          if (twaCount < maxTwaMeasurements) {
            twaMeasurements[twaCount] = stel;
            twaCount++;
          } else {
            for (int i = 0; i < maxTwaMeasurements - 1; i++) {
              twaMeasurements[i] = twaMeasurements[i + 1];
            }
            twaMeasurements[maxTwaMeasurements - 1] = stel;
          }

          // Calcular el TWA
          if (twaCount == maxTwaMeasurements) {
            float twa = 0.0;
            for (int i = 0; i < maxTwaMeasurements; i++) {
              twa += twaMeasurements[i];
            }
            twa /= maxTwaMeasurements;

            // Imprimir el valor de TWA
            Serial.print("TWA: ");
            Serial.print(twa, 2);
            Serial.println(" ppm");
          }
        }
      }
    }
  }
}

float GasData() {
  // Simulación de una función que devuelve un valor de gas medido
  // En un caso real, esta función obtendría datos del sensor
  return random(10, 50); // Devuelve un valor aleatorio entre 10 y 50 ppm
}
