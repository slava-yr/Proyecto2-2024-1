#include <Adafruit_NeoPixel.h>
#define BAT_VOLT A6

float porcentaje_bat() {
    float voltageMin = 2.8;
    float voltageMax = 4.2;
    int adcValue = analogRead(BAT_VOLT);  // Leer el valor del ADC en el pin A6
    float voltage = adcValue * (5.0 / 1023);  // Convertir el valor del ADC a tensión (ajusta 5.0 a tu tensión de referencia si es diferente)
    
    // Calcular el porcentaje de carga basado en la tensión medida
    float porcentaje = (voltage - voltageMin) * 100.0 / (voltageMax - voltageMin);
    
    // Asegurarse de que el porcentaje esté en el rango de 0% a 100%
    if (porcentaje < 0) {
        porcentaje = 0;
    } else if (porcentaje > 100) {
        porcentaje = 100;
    }
    return porcentaje;
}

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

}
