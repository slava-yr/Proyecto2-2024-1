#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "perifericos.h"

//Pines para la tira led
#define ON_OFF_LED A3 //Pin para habilitar alimentación de los LED

//Pines de habilitación de alimentación
#define ON_OFF_BUZZER 4   //Pin habilitador de buzzer
#define ON_OFF_PANTALLA 6 //Pin habilitador de pantalla OLED
#define ON_OFF_SENSORES 7 //Pin que controla la alimentación de los sensores
#define ON_OFF_VIB 10     //Pin habilitador de vibrador

//Pines para habilitación de sensores (solo NO2 y CO)
#define EN_CO 8
#define EN_NO2 9  

//Pin de interrupción
// CAMBIAR A 2 DE NUEVO, SOLO ES PARA PROBAR
#define INT0 2// 2//Pin de interrupción

//Pines de lectura del ADC
#define BAT_VOLT A6 //Pin de lectura de la tensión en la batería
#define CO_OUT  A0  //Pin de lectura de valor de sensor CO
#define NO2_OUT  A1 //Pin de lectura de valor de sensor NO2
#define O2_OUT  A2  //Pin de lectura de valor de sensor O2


OLED pantalla(ON_OFF_PANTALLA, INT0); // Crea un oled_display
indicadores indicadores(ON_OFF_LED, ON_OFF_VIB, ON_OFF_BUZZER); // Crea los indicadores

char selected_mode;

void setup() {
  pinMode(INT0, INPUT);
  pinMode(EN_CO, OUTPUT);
  pinMode(EN_NO2, OUTPUT);

  Serial.begin(9600);
  pantalla.begin(); 
  indicadores.begin();
  indicadores.patron_inicio();
  // selected_mode = pantalla.selectMode(); // El usuario selecciona TWA o STEL
}

void loop() {
  /******** BOSQUEJO DE CÓDIGO PRINCIPAL ***********/ 
  // TOMAR MEDICIONES DE LOS SENSORES Y GUARDARLAS. GUARDAR VALOR PICO
  // EN las mediciones de arriba también hay que incluir la medición de batería.
  // CALCULAR STEL/TWA (SEGÚN SELECCIÓN DEL USUARIO)
  // VERIFICAR SI SUPERA VALORES LÍMITES (revisar valores límite):
    // Si supera, iniciar alarma (2 modos: moderado y alto)
  // REVISAR LA BANDERA DE INTERRUPCIÓN POR BOTÓN
  // Si está alta, imprimir en la pantalla las lecturas actuales y el valor de STEL/TWA 
  // 
  indicadores.patron_inicio();
  // digitalWrite(ON_OFF_LED, HIGH);
}
