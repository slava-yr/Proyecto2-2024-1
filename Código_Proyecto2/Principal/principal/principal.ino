#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

//Pines para la tira led
#define COLOR_LED 3   //Pin PWM para el color de los LED
#define ON_OFF_LED A3 //Pin para habilitar alimentación de los LED
#define NUMLEDS 8     //Número de LEDs en la tira 

//Pines de habilitación de alimentación
#define ON_OFF_BUZZER 4   //Pin habilitador de buzzer
#define ON_OFF_PANTALLA 6 //Pin habilitador de pantalla OLED
#define ON_OFF_SENSORES 7 //Pin que controla la alimentación de los sensores
#define ON_OFF_VIB 10     //Pin habilitador de vibrador

//Pines para habilitación de sensores (solo NO2 y CO)
#define EN_CO 8
#define EN_NO2 9  

//Pin de interrupción
#define INT0 2 //Pin de interrupción

//Pines de lectura del ADC
#define BAT_VOLT A6 //Pin de lectura de la tensión en la batería
#define CO_OUT  A0  //Pin de lectura de valor de sensor CO
#define NO2_OUT  A1 //Pin de lectura de valor de sensor NO2
#define O2_OUT  A2  //Pin de lectura de valor de sensor O2


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMLEDS, COLOR_LED, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pinMode(COLOR_LED, OUTPUT);
  pinMode(ON_OFF_BUZZER, OUTPUT);
  pinMode(ON_OFF_PANTALLA, OUTPUT);
  pinMode(ON_OFF_SENSORES, OUTPUT);
  pinMode(ON_OFF_VIB, OUTPUT);
  pinMode(INT0, INPUT);
  pinMode(ON_OFF_LED, OUTPUT);
  pinMode(EN_CO, OUTPUT);
  pinMode(EN_NO2, OUTPUT);

  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}

 //controla la intensidad de cada color ingresado(0-100%)y devuelve el comando pixels con la intensidad adecuada
uint32_t color_intensity(uint32_t color, uint8_t scale) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;
  r = (r * scale) / 100;
  g = (g * scale) / 100;
  b = (b * scale) / 100;
  return pixels.Color(r, g, b);
}

//patrón de luces de inicio del dispositivo
//enciende y apaga atenuando los colores del arreglo colors[]
void patron_inicio() {
  uint32_t colors[] = {pixels.Color(0, 0, 255), pixels.Color(0, 255, 0), pixels.Color(255, 0, 0)};
  int numColors = sizeof(colors) / sizeof(colors[0]);

  for(int j = 0; j < numColors; j++) {
    for(int brightness = 0; brightness <= 100; brightness++) {
      for(int i = 0; i < NUMLEDS; i++) {
        pixels.setPixelColor(i, color_intensity(colors[j], brightness));
      }
      digitalWrite(ON_OFF_VIB, HIGH);
      pixels.show();
      delay(8);
    }

    delay(300);

    for(int brightness = 100; brightness >= 0; brightness--) {
      for(int i = 0; i < NUMLEDS; i++) {
        pixels.setPixelColor(i, color_intensity(colors[j], brightness));
      }
      digitalWrite(ON_OFF_VIB, LOW);
      pixels.show();
      delay(8);
    }
  }

  for(int brightness = 100; brightness >= 0; brightness--) {
    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(brightness*255/100, brightness*255/100, brightness*255/100));
    }
    digitalWrite(ON_OFF_BUZZER, HIGH);
    pixels.show();
    delay(8);
  }
  digitalWrite(ON_OFF_BUZZER, LOW);
  delay(500);
}

void lectura_alta() {
  for (int i = 0; i < 10; i++) {
    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(255, 0, 0), 90));
    }
    pixels.show();
    digitalWrite(ON_OFF_VIB, HIGH);
    digitalWrite(ON_OFF_BUZZER, HIGH);
    delay(200);

    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(0, 0, 255), 90));
    }
    pixels.show();
    digitalWrite(ON_OFF_VIB, LOW);
    digitalWrite(ON_OFF_BUZZER, LOW);
    delay(200);
  }
  for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  delay(200);
}

void lectura_moderada() {
  for (int i = 0; i < 10; i++) {
    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(255, 191, 0), 90));
    }
    pixels.show();
    digitalWrite(ON_OFF_VIB, HIGH);
    digitalWrite(ON_OFF_BUZZER, HIGH);
    delay(300);

    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(0, 0, 0), 90));
    }
    pixels.show();
    digitalWrite(ON_OFF_VIB, LOW);
    digitalWrite(ON_OFF_BUZZER, LOW);
    delay(300);
  }
  for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  delay(300);
}

void lectura_normal() {
  for (int i = 0; i < 3; i++) {
    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(0, 255, 0), 90));
    }
    pixels.show();
    digitalWrite(ON_OFF_VIB, HIGH);
    digitalWrite(ON_OFF_BUZZER, HIGH);
    delay(500);

    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(0, 0, 0), 90));
    }
    pixels.show();
    digitalWrite(ON_OFF_VIB, LOW);
    digitalWrite(ON_OFF_BUZZER, LOW);
    delay(500);
  }
  for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  delay(500);
}







