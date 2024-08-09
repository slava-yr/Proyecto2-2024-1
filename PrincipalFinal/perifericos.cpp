#include "Arduino.h"
#include "perifericos.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>

// Para el OLED
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define STARTUP_DELAY 500 // Time for the startup screens
#define DISPLAY_DELAY 5000 // Time for each measurement


// Para los leds
#define NUMLEDS 7
#define COLOR_LED 3 // Pin pwm para el color de los leds
#define LEDS_DELAY 100
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMLEDS, COLOR_LED, NEO_GRB + NEO_KHZ800);

// Calentamiento de sensores
#define TIEMPO_CALENTADO 120000 // 2 minutos en ms

const unsigned char pucplogo [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x03, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xfb, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xfb, 0xff, 0xf0, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xf9, 0xff, 0xfe, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xc0, 0x7f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xc0, 0x7f, 0xe0, 0x0f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf9, 0xff, 0x80, 0x03, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf9, 0xff, 0x00, 0x01, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xf9, 0xfe, 0x00, 0x03, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xf9, 0xfc, 0x01, 0x03, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xf9, 0xf8, 0x01, 0x87, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xf0, 0x01, 0x07, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xf0, 0x01, 0x07, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xf0, 0x01, 0x07, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xf0, 0x01, 0x07, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xf0, 0x01, 0x07, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xf3, 0x01, 0x07, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xe1, 0x87, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xfc, 0x39, 0x87, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xf6, 0x0f, 0xe7, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xf6, 0x07, 0xf3, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xf2, 0x01, 0xfb, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x20, 0x0f, 0xf2, 0x00, 0x7f, 0x7c, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x40, 0xf9, 0x00, 0x1c, 0x7c, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x1c, 0x78, 0x38, 0x00, 0x00, 0x78, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x1f, 0x08, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xcf, 0xc0, 0x00, 0x00, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xe7, 0xe3, 0x00, 0x00, 0x11, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xf3, 0xf9, 0xf0, 0x01, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xf9, 0xfc, 0x7f, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xfc, 0x7f, 0x1f, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xfe, 0x3f, 0xc3, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0x9f, 0xf0, 0x7e, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xcf, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xe3, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xfc, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x01, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const unsigned char subterralogo [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0xc0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x01, 0xb0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x01, 0x1f, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x01, 0x86, 0x40, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x03, 0xc4, 0x00, 0x1f, 0xc6, 0x72, 0x18, 0x10, 0x41, 0x83, 0xfd, 0xfc, 0x7c, 0xe0, 0x83, 
	0xf0, 0x0c, 0x6c, 0xc0, 0x1f, 0x9e, 0x72, 0x0e, 0x31, 0xc0, 0x81, 0xf9, 0xf9, 0x7c, 0xe3, 0x8f, 
	0xf0, 0x14, 0x7c, 0x40, 0x1f, 0x9e, 0x76, 0x4e, 0x73, 0xcc, 0x99, 0xf8, 0xf9, 0xf8, 0xe7, 0x9f, 
	0xf0, 0x10, 0x7c, 0x40, 0x1f, 0x9e, 0x76, 0x5e, 0x71, 0xcc, 0x99, 0xf6, 0xf8, 0xf8, 0x63, 0x8f, 
	0xf0, 0x1f, 0xef, 0xe0, 0x1f, 0xce, 0x76, 0x1e, 0x70, 0x49, 0x83, 0xf5, 0x7c, 0x78, 0x60, 0x83, 
	0xf0, 0x0e, 0xfe, 0x10, 0x1f, 0xe6, 0x76, 0x0e, 0x71, 0xc3, 0x83, 0xe9, 0x7e, 0x7a, 0x63, 0x8f, 
	0xf0, 0x04, 0x7c, 0x10, 0x1f, 0xf2, 0x76, 0x4e, 0x73, 0xc3, 0x83, 0xe9, 0xbf, 0x32, 0x67, 0x9f, 
	0xf0, 0x04, 0x6c, 0x60, 0x1f, 0xf2, 0x66, 0x4e, 0x73, 0xc9, 0x93, 0xd3, 0xbf, 0x30, 0x67, 0x9f, 
	0xf0, 0x02, 0x46, 0xc0, 0x1f, 0xb3, 0x06, 0x0e, 0x70, 0x49, 0x99, 0xd4, 0x9b, 0x30, 0x27, 0x83, 
	0xf0, 0x00, 0xc3, 0x80, 0x1f, 0x87, 0x06, 0x1e, 0x70, 0x4c, 0x99, 0x80, 0x18, 0x33, 0x27, 0x83, 
	0xf0, 0x05, 0xc2, 0x00, 0x1f, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xdd, 0x80, 0x1c, 0xff, 0xff, 0xff, 
	0xf0, 0x03, 0x32, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x1e, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf8, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


/* **********************************************************
 **********CÓDIGO DE CLASE OLED******************************
 ***********************************************************/

OLED::OLED(uint8_t enablePinOLED, uint8_t interruptPin)
{
  _enableOLED = enablePinOLED; // Set the pin for enabling the OLED displays
  _interruptPin = interruptPin; // Pin attached to the button
}

void OLED::begin() // First time 
// Se apaga después del begin. Debes llamar el wakeup y luego lo que quieras hacer.
{
  pinMode(_enableOLED, OUTPUT); // Control pin for OLED
  //pinMode(_interruptPin, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(_interruptPin), handleButtonPress, RISING); // Interrupt
  digitalWrite(_enableOLED, HIGH); // Enable OLED 
  delay(STARTUP_DELAY/2); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.clearDisplay();
  display.drawBitmap(0, 0, pucplogo, 128, 64, 1); // Draw pucp logo
  display.display(); // Display it
  delay(STARTUP_DELAY/2); 
  display.clearDisplay();
  display.drawBitmap(0, 0, subterralogo, 128, 64, 1); // Draw subterra logo
  display.display(); // Display it
  delay(STARTUP_DELAY); 
  display.setTextColor(SH110X_WHITE); // Set text color for writing 
  
  display.clearDisplay();
  // CALENTAR POR 2 MINUTOS
  // unsigned long int startTime = millis()  
}

void OLED::wakeUp() // Inicializa tras estar apagado
{
  digitalWrite(_enableOLED, HIGH);
  delay(STARTUP_DELAY/2); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.clearDisplay(); 
  display.setTextColor(SH110X_WHITE);
}

void OLED::displayO2(float measurement)
{
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("% O2\n");
  display.setTextSize(4); 
  display.print(measurement);
  display.println(" %");
  display.display();
  delay(DISPLAY_DELAY);
  display.clearDisplay();
  display.display();
}

void OLED::displayCO(float measurement)
{
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("CO (ppm)\n");
  display.setTextSize(4); 
  display.println(measurement);
  display.display();
  delay(DISPLAY_DELAY);
  display.clearDisplay();
  display.display();
}

void OLED::displayNO2(float measurement)
{
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("NO2 (ppm)\n");
  display.setTextSize(4); 
  display.println(measurement);
  display.display();
  delay(DISPLAY_DELAY);
  display.clearDisplay();
  display.display();
}

void OLED::calentandoScreen() // Aprox 9 segundos
{
  long unsigned int startTime = millis();
  long unsigned int currentTime = startTime;

  if (currentTime - startTime < TIEMPO_CALENTADO)
  {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("Calentando sensores");
    for (int i = 0; i < 10; i++)
    {
      display.print(".");
      delay(DISPLAY_DELAY/5);
      display.display();
    }
    currentTime = millis(); // Actualiza el tiempo actual
    // TODO: Tiempo de estimado de calentar 
  }
  
}

void OLED::displayLecturas(float measurementO2, float measurementCO, float measurementNO2)
{
  displayO2(measurementO2);
  displayCO(measurementCO);
  displayNO2(measurementNO2);
  digitalWrite(_enableOLED, LOW); // Apaga la pantalla tras mostrar las lecturas 
}

/*
void OLED::lowBattery() // Pantalla de batería baja
{
  display.drawBitmap(0, 0, lowBatteryIcon, 128, 64, 1);
  display.display();
  delay(STARTUP_DELAY); 
  display.clearDisplay();
  display.display();
  delay(STARTUP_DELAY); 
}*/

char OLED::selectMode() // Selecciona el modo: TWA o STEL
{
  int buttonState = 0; // Current state of the button
  int lastButtonState = 1; // previous state of the button
  int startPressed = 0;    // the moment the button was pressed
  int endPressed = 0;      // the moment the button was released
  unsigned long int holdTime = 0;        // how long the button was hold
  char lastMode = 'T'; // Initialize last mode
  char finalMode; // Return variable

  //wakeUp();
  lastMode = updateDisplay(lastMode);
  while(1) // Loop hasta que el usuario escoja una opción
  {
    buttonState = digitalRead(_interruptPin); // read the button input
    if (buttonState != lastButtonState) { // button state changed
      if (buttonState == LOW)
      {
        startPressed = millis(); // the button has been just pressed
      }  
      else // the button has been just released 
      {
        endPressed = millis();
        holdTime = endPressed - startPressed;
      }
    }
    lastButtonState = buttonState;        // save state for next loop

    if (100 < holdTime && holdTime < 500) // Quick press: Change screen
    {
      lastMode = updateDisplay(lastMode);
      // Serial.println(holdTime);
      holdTime = 0;
    }

    if (holdTime > 1200) // Confirmar opción
    {
      finalMode = lastMode;
      // Pantalla de modo seleccionado
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(5);  

      Serial.println("Seleccionó ");
      if (finalMode == 'T')
      {
        display.println("TWA");
        Serial.println("TWA");
      }
      else
      {
        Serial.println("STEL");
        display.println("STEL");
      }
      display.setTextSize(1);
      display.print("\nSeleccionado");
      display.display();
      delay(1000);
      return finalMode;
    }
  }
}

char OLED::updateDisplay(char lastMode)
{
  char newMode;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Seleccione el modo de medicion: \n");
  display.setTextSize(5);  
  
  if (lastMode == 'T') // TWA
  { 
    Serial.println("STEL");
    display.println("STEL"); 
    newMode = 'S'; // STEL
  }
  else if (lastMode == 'S')
  {
    Serial.println("TWA");
    display.println("TWA"); 
    newMode = 'T'; // TWA
  } 

  display.display();
  delay(600);
  return newMode;
}
/* **********************************************************
 **********CÓDIGO DE LOS INDICADORES*************************
 ************************************************************/

indicadores::indicadores(uint8_t ledEnablePin, uint8_t vibEnablePin, uint8_t buzzerEnablePin)
{
  _enableLeds = ledEnablePin; 
  _enableVibrador = vibEnablePin;
  _enableBuzzer = buzzerEnablePin;
}

void indicadores::begin()
{
  pinMode(_enableLeds, OUTPUT);
  pinMode(_enableVibrador, OUTPUT);
  pinMode(_enableBuzzer, OUTPUT);
  digitalWrite(_enableBuzzer, HIGH);
  delay(400);
  digitalWrite(_enableBuzzer, LOW);
}

  //controla la intensidad de cada color ingresado(0-100%)y devuelve el comando pixels con la intensidad adecuada
uint32_t indicadores::color_intensity(uint32_t color, uint8_t scale) {
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
void indicadores::patron_inicio() {
  digitalWrite(_enableLeds, HIGH);
  uint32_t colors[] = {pixels.Color(0, 0, 255), pixels.Color(0, 255, 0), pixels.Color(255, 0, 0)};
  int numColors = sizeof(colors) / sizeof(colors[0]);

  for(int j = 0; j < numColors; j++) {
    for(int brightness = 0; brightness <= 100; brightness++) {
      for(int i = 0; i < NUMLEDS; i++) {
        pixels.setPixelColor(i, color_intensity(colors[j], brightness));
      }
      pixels.show();
      delay(8);
    }

    if (j == 0){
      digitalWrite(_enableBuzzer, HIGH);
      digitalWrite(_enableVibrador, HIGH);
      delay(500);
    }
    
    delay(300);
    digitalWrite(_enableBuzzer, LOW);
    digitalWrite(_enableVibrador, LOW);

    /*(int brightness = 100; brightness >= 0; brightness--) {
      for(int i = 0; i < NUMLEDS; i++) {
        pixels.setPixelColor(i, color_intensity(colors[j], brightness));
      }
      //digitalWrite(ON_OFF_VIB, LOW);
      pixels.show();
      delay(8);
    }*/
  }

  for(int brightness = 100; brightness >= 0; brightness--) {
    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(brightness*255/100, brightness*255/100, brightness*255/100));
    }
    pixels.show();
    delay(8);
  }
  
  delay(500);
}

void indicadores::lectura_alta() {
  for (int i = 0; i < 10; i++) {
    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(255, 0, 0), 90));
    }
    pixels.show();
    digitalWrite(_enableVibrador, HIGH);
    digitalWrite(_enableBuzzer, HIGH);
    delay(200);

    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(0, 0, 255), 90));
    }
    pixels.show();
    digitalWrite(_enableVibrador, LOW);
    digitalWrite(_enableBuzzer, LOW);
    delay(200);
  }
  for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  delay(200);
}

void indicadores::lectura_moderada() {
  for (int i = 0; i < 10; i++) {
    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(255, 191, 0), 90));
    }
    pixels.show();
    digitalWrite(_enableVibrador, HIGH);
    digitalWrite(_enableBuzzer, HIGH);
    delay(300);

    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(0, 0, 0), 90));
    }
    pixels.show();
    digitalWrite(_enableVibrador, LOW);
    digitalWrite(_enableBuzzer, LOW);
    delay(300);
  }
  for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  delay(300);
}

void indicadores::lectura_normal() {
  for (int i = 0; i < 3; i++) {
    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(0, 255, 0), 90));
    }
    pixels.show();
    digitalWrite(_enableVibrador, HIGH);
    digitalWrite(_enableBuzzer, HIGH);
    delay(500);

    for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, color_intensity(pixels.Color(0, 0, 0), 90));
    }
    pixels.show();
    digitalWrite(_enableVibrador, LOW);
    digitalWrite(_enableBuzzer, LOW);
    delay(500);
  }
  for(int i = 0; i < NUMLEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  delay(500);
}



