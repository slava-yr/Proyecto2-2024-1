#ifndef perifericos_h
#define perifericos_h

#include "Arduino.h"

/*************************************************
  Código de pantalla OLED 1.3" modelo SH1106
  DONE: 
  * Pantalla de inicio (Nombre del dispositivo)
  * NO2 (ppm), CO (ppm), O2 (%)
  * Pantalla 1: Calentando sensores
  * Ícono de batería baja

  TODO:
  ************************************************/

class OLED
{
  public:
    OLED();
    void begin();
    void displayO2(float measurement);
    void displayCO(float measurement);
    void displayNO2(float measurement);
    void calentandoScreen();
    void lowBattery();
    
  private:

};

#endif 