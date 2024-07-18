#ifndef perifericos_h
#define perifericos_h

//#include "Arduino.h"

/*************************************************
  Código de periféricos: LEDs, vibrador, pantalla, buzzer
  DONE: 
  Pantalla OLED 1.3" modelo SH1106
  * Pantalla de inicio (Nombre del dispositivo)
  * NO2 (ppm), CO (ppm), O2 (%)
  * Pantalla 1: Calentando sensores
  * Ícono de batería baja
  * Código de tira LED: rutinas de color
  * Código de alarma:

  TODO:
  * Pantalla de carga completa
  * Interrupciones del botón para activar la pantalla
  * 
  ************************************************/

class OLED
{
  public:
    OLED(uint8_t enablePinOLED, uint8_t interruptPin);
    void begin();
    // Displays individuales
    void displayO2(float measurement);
    void displayCO(float measurement);
    void displayNO2(float measurement);
    void alertaO2(float measurement);
    void alertaCO(float measurement);
    void alertaNO2(float measurement);

    void calentandoScreen(); // Calentamiento de sensores

    void lowBattery(); 
    void wakeUp();
    void displayLecturas(float measurementO2, float measurementCO, float measurementNO2);
    char updateDisplay(char lastMode);
    char selectMode();
    
  private:
    uint8_t _enableOLED;
    uint8_t _interruptPin;
};

class indicadores
{
  public: 
    indicadores(uint8_t ledEnablePin, uint8_t vibEnablePin, uint8_t buzzerEnablePin);
    void begin();
    uint32_t color_intensity(uint32_t color, uint8_t scale);
    void patron_inicio();
    void lectura_alta();
    void lectura_moderada();
    void lectura_normal();
    void buzz_on();
    void buzz_off();

  private:
    uint8_t _enableLeds;
    uint8_t _enableVibrador;
    uint8_t _enableBuzzer;
};

#endif 