#ifndef perifericos_h
#define perifericos_h

#include "Arduino.h"

/*************************************************
  Código de periféricos: LEDs, vibrador, pantalla, buzzer
  DONE: 
  Pantalla OLED 1.3" modelo SH1106
  * Pantalla de inicio (Nombre del dispositivo)
  * NO2 (ppm), CO (ppm), O2 (%)
  * Pantalla 1: Calentando sensores
  * Ícono de batería baja

  TODO:
  * Pantalla de carga completa
  * Código de tira LED: rutinas de color
  * Código de alarma:
    * Buzzer
    * Vibrador 
    * Luces intermitentes 
  ************************************************/

class OLED
{
  public:
    OLED(uint8_t enablePinOLED);
    void begin();
    void displayO2(float measurement);
    void displayCO(float measurement);
    void displayNO2(float measurement);
    void calentandoScreen();
    void lowBattery();
    void wakeUp();
    void displayLecturas(float measurementO2, float measurementCO, float measurementNO2);
  private:
    uint8_t _enableOLED;

};

class leds
{
  public: 
    leds(uint8_t ledEnablePin);
    void begin();
    void Indicator();
  private:
    uint8_t _enableLeds;
};

class alarma
{
  public:
    alarma();
    void begin();
    void alarmaON();
  
  private:
    uint8_t _pinVibrador;
    uint8_t _pinBuzzer;

};

#endif 