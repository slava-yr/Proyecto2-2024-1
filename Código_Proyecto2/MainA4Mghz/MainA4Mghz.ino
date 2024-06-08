#include <LowPower.h>
#include <avr/sleep.h>
#include <avr/power.h>

// Button pin configuration
const int buttonPin = 7;

// Sensor power pin configuration (adjust as needed)
const int sensorPowerPin = 8;

// Function prototypes
void updateValue();
void showDisplay();
void onButtonPress();
void sleep();
void setClockFrequency(uint8_t prescaler);
void setupTimer1();
void setupTimer2();

// Variable to store gas value
volatile int gasValue = 0;
volatile bool buttonPressed = false;
volatile bool updateRequired = false;

void setup() {
  // Initialize the button pin as input with pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize the sensor power pin as output and turn it off initially
  pinMode(sensorPowerPin, OUTPUT);
  digitalWrite(sensorPowerPin, LOW);

  // Attach interrupt to the button pin
  attachInterrupt(digitalPinToInterrupt(buttonPin), onButtonPress, FALLING);

  // Set clock frequency to 4 MHz (prescaler 4)
  setClockFrequency(2);

  // Turn off ADC before sleep to save power
  ADCSRA &= ~(1 << ADEN);

  // Setup Timer1 for 2-minute intervals
  setupTimer1();

  // Setup Timer2 for button debounce
  setupTimer2();

  // Initial message indicating readiness (using Serial for demonstration)
  Serial.begin(9600);
  Serial.println("Gas Detector Ready");
}

void loop() {
  // Enter power down state with ADC and BOD disabled
  sleep();

  // Wake-up routine
  if (buttonPressed) {
    showDisplay();
    buttonPressed = false;
  }

  if (updateRequired) {
    updateRequired = false;
    updateValue();
  }
}

void updateValue() {
  // Turn on the sensor
  digitalWrite(sensorPowerPin, HIGH);

  // Allow some time for the sensor to stabilize if necessary (adjust as needed)
  delay(1000);

  // Enable ADC to read the sensor value
  ADCSRA |= (1 << ADEN);
  gasValue = analogRead(A0);  // Assuming gas sensor is connected to A0
  ADCSRA &= ~(1 << ADEN);  // Disable ADC to save power

  // Turn off the sensor
  digitalWrite(sensorPowerPin, LOW);

  // Print the new value to Serial Monitor for debugging
  Serial.print("Updated Gas Value: ");
  Serial.println(gasValue);
}

void onButtonPress() {
  buttonPressed = true;
}

void showDisplay() {
  // Display the gas value (using Serial for demonstration)
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // Wait a little bit to debounce button
  delay(300);
}

void sleep() {
  // Sleep for 8 seconds
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

void setClockFrequency(uint8_t prescaler) {
  // Set the clock prescaler (prescaler = 1 means no division, 2 means divide by 2, etc.)
  // This changes the system clock speed.
  CLKPR = (1 << CLKPCE); // Enable change of the clock prescaler
  CLKPR = prescaler;     // Set the clock prescaler
}

void setupTimer1() {
  // Timer1 for 2-minute intervals
  noInterrupts();           // Disable all interrupts
  TCCR1A = 0;               // Clear Timer/Counter Control Registers
  TCCR1B = 0;
  TCNT1 = 0;                // Initialize counter value to 0

  // Set compare match register for 2-minute increments
  OCR1A = 15624;            // = (16*10^6) / (1024*1) - 1 (must be <65536)

  // Turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  interrupts();             // Enable all interrupts
}

void setupTimer2() {
  // Timer2 for button debounce (if needed)
  noInterrupts();           // Disable all interrupts
  TCCR2A = 0;               // Clear Timer/Counter Control Registers
  TCCR2B = 0;
  TCNT2 = 0;                // Initialize counter value to 0

  // Set compare match register for debounce interval
  OCR2A = 124;              // 1ms debounce

  // Turn on CTC mode
  TCCR2B |= (1 << WGM21);
  // Set CS22, CS21 and CS20 bits for 1024 prescaler
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  // Enable Timer2 compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  interrupts();             // Enable all interrupts
}

ISR(TIMER1_COMPA_vect) {
  // Timer1 interrupt service routine
  updateRequired = true;
}

ISR(TIMER2_COMPA_vect) {
  // Timer2 interrupt service routine (if needed for debounce)
}
