  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <avr/sleep.h>
  #include <avr/power.h>
  #define ANCHO 128
  #define ALTO 64
  #define OLED_RESET 4
  Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);
  const int Pin_boton = 7;
  const int Pin_BUZZ= 5;
  const int Pin_Sensores= 6;
  const int Pin_OLED= 13;
  const int Pin_inter= 2;
  volatile bool interruptFlag = false;
  float t;
  float t_co;
  float t_no2;
  int contador_0=0;
  int contador_timer2 = 0;
  int inicio_0=1;
  int contador_timer2_inicio=0;
  // Setup de los pines a utilizar
  void setup() {
    Serial.begin(9600);
    pinMode(Pin_boton,OUTPUT); 
    digitalWrite(Pin_boton,HIGH);
    pinMode(Pin_inter,INPUT); 
    digitalWrite(Pin_inter,LOW);
    pinMode(Pin_BUZZ,OUTPUT);
    pinMode(Pin_Sensores,OUTPUT);
    digitalWrite(Pin_Sensores,HIGH);
    pinMode(Pin_OLED,OUTPUT);
    digitalWrite(Pin_OLED,HIGH);
    delay(100);
    Wire.begin();
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    attachInterrupt(digitalPinToInterrupt(Pin_inter),handleInterrupt, FALLING);
    cli();//desactivar interrupciones globales
    //Configurar timer 2
    /*TCCR2A = 0x00; // Wave Form Generation Mode 0: Normal Mode, OC2A disconnected
    TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20); // prescaler = 1024
    TIMSK2 = (1<<TOIE2);*/
    sei();//activar interrupciones globales
  }

  void loop() { 
    if (interruptFlag) {
      Serial.println("iniciar oled");
      digitalWrite(Pin_OLED,HIGH);
      delay(100);
      Wire.begin();
      oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
      delay(500);
      handleInterruptAction();
      buzz_limite();
      interruptFlag = false; // Restablece la bandera de interrupción
    }
    //codigo timer2
    /*if(contador_timer2==500){
      TIMSK2 &= ~(1<<TOIE2);//desactivas interrupciones del timer
      digitalWrite(Pin_OLED,HIGH);
      oled.clearDisplay();
      oled.setTextColor(WHITE);
      oled.setCursor(0, 0);
      oled.setTextSize(1);
      oled.print("Entro al timer 2");
      oled.print(t);
      oled.display();
      delay(2000);
      handleInterruptAction();
      contador_timer2=0;
      inicio_0=1;
      contador_timer2_inicio=0;
    }*/
    //Codigo de medicion
    while((contador_0<=14)&&(inicio_0==1)){
      Serial.println("Inicio.");
      oled.clearDisplay();
      oled.setTextColor(WHITE);	
      oled.setCursor(0, 0);
      oled.setTextSize(2);
      oled.print("DETECTOR");
      oled.setCursor(0, 16);
      oled.print("DE GASES");
      oled.setCursor(0, 32);
      oled.print("NOCIVOS");
      oled.setCursor(0, 48);
      oled.print("PUCP");
      oled.display();
      buzz_inicio();
      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.print("Iniciando");
      oled.setCursor(0, 16);
      oled.print("sensores..");
      oled.display();
      delay(4000);
      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.print("Sensores ");
      oled.setCursor(0, 16);
      oled.print("listos...");
      oled.display();
      delay(2000);
      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.print("Tomando");
      oled.setCursor(0, 16);
      oled.print("muestras ");
      oled.setCursor(0, 32);
      oled.print("de aire...");
      oled.display();
      delay(4000);
      contador_0=15;}
    if(contador_0>14){
      oled.clearDisplay();
      oled.setTextColor(WHITE);
      oled.setCursor(0, 0);
      oled.setTextSize(2);
      oled.print("Se mostraran");
      oled.setCursor(0, 16);
      oled.print("las");
      oled.setCursor(0, 32);
      oled.print("mediciones");
      oled.display();
      delay(2000);
      contador_0=0;
      contador_timer2_inicio=1;
      inicio_0=0;
      handleInterruptAction();
      buzz_limite();
    }
    //Despues de que se muestran las mediciones, los pines usados pasan a LOW
    if(contador_timer2_inicio==1){
      digitalWrite(Pin_OLED,LOW);
      set_sleep_mode(SLEEP_MODE_PWR_SAVE);
      sleep_mode();
      contador_timer2++;
      Serial.println("desperte");
    }
  }
  void buzz_inicio()
  {
    digitalWrite(Pin_BUZZ, HIGH);   // Ponemos en alto(5V) el pin del buzzer
    delay(250);                     // Esperamos 1 segundo
    digitalWrite(Pin_BUZZ, LOW);    // Ponemos en bajo(0V) el pin del buzzer
    delay(250);                     // Esperamos 4 segundos
    digitalWrite(Pin_BUZZ, HIGH);   // Ponemos en alto(5V) el pin del buzzer
    delay(250);                     // Esperamos 1 segundo
    digitalWrite(Pin_BUZZ, LOW);    // Ponemos en bajo(0V) el pin del buzzer
    delay(250);         
    digitalWrite(Pin_BUZZ, HIGH);   // Ponemos en alto(5V) el pin del buzzer
    delay(250);                     // Esperamos 1 segundo
    digitalWrite(Pin_BUZZ, LOW);    // Ponemos en bajo(0V) el pin del buzzer
    delay(250);         
  }
  void buzz_limite()
  {
    digitalWrite(Pin_BUZZ, HIGH);   // Ponemos en alto(5V) el pin del buzzer
    delay(125);                     // Esperamos 1 segundo
    digitalWrite(Pin_BUZZ, LOW);    // Ponemos en bajo(0V) el pin del buzzer
    delay(125);                           // Esperamos 4 segundos
    digitalWrite(Pin_BUZZ, HIGH);   // Ponemos en alto(5V) el pin del buzzer
    delay(125);                          // Esperamos 1 segundo
    digitalWrite(Pin_BUZZ, LOW);    // Ponemos en bajo(0V) el pin del buzzer
    delay(125);               
    digitalWrite(Pin_BUZZ, HIGH);   // Ponemos en alto(5V) el pin del buzzer
    delay(125);                          // Esperamos 1 segundo
    digitalWrite(Pin_BUZZ, LOW);    // Ponemos en bajo(0V) el pin del buzzer
  }
  void handleInterrupt() {
    interruptFlag = true;
    Serial.println("Interrupción detectada.");
  }
  float read_o2()
  {
    float valorSensor;
    float voltajeSensor; 
    float Valor_O2;
    valorSensor = analogRead(A2);
    voltajeSensor =(valorSensor/1024)*5.0;
    Valor_O2 = voltajeSensor*20.94/2.89;
    return Valor_O2;
  }
  float read_co()
  {
    float valorSensor_co;
    float voltajeSensor_co; 
    float Valor_cO;
    valorSensor_co = analogRead(A0);
    voltajeSensor_co =(valorSensor_co/1024)*5.0;
    Valor_cO = voltajeSensor_co/5;
    return Valor_cO;
  }
  float read_no2()
  {
    float valorSensor_no2;
    float voltajeSensor_no2; 
    float Valor_nO2;
    valorSensor_no2 = analogRead(A1);
    voltajeSensor_no2 =(valorSensor_no2/1024)*5.0;
    Valor_nO2 = (4.92 - voltajeSensor_no2)/5;
    return Valor_nO2;
  }
  void handleInterruptAction() {
    t = read_o2(); 
    t_co = read_co();
    t_no2 = read_no2(); 
    
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.setTextSize(2);
    oled.print("O2:");
    oled.print(t);
    oled.print("%");
    oled.setCursor(0, 16);
    oled.print("C0:");
    oled.print(t_co );
    oled.print("%");
    oled.setCursor(0, 32);
    oled.print("NO2:");
    oled.print(t_no2);
    oled.print("%");
    oled.display();
    delay(5000); // Ajusta el tiempo de retardo según sea necesario
    oled.clearDisplay();
    if (t < 19.5){
      buzz_limite();
      oled.setCursor(0, 0);
      oled.setTextSize(2);
      oled.print("Bajo nivel de oxigeno");
      oled.display();
      delay(5000);
      oled.clearDisplay();
    }
    if (t_co > 25/10000){
      buzz_limite();
      oled.setCursor(0, 0);
      oled.setTextSize(2);
      oled.print("Alto nivel de CO");
      oled.display();
      delay(5000);
      oled.clearDisplay();
      
    }
    if (t_no2 > 0.5){
      buzz_limite();
      oled.setCursor(0, 0);
      oled.setTextSize(2);
      oled.print("Alto nivel de NO2");
      oled.display();
      delay(5000);
      oled.clearDisplay();
    }
    
  }

  ISR(TIMER2_OVF_vect) {
}
