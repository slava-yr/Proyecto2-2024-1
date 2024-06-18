#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

// Button pin configuration
const int buttonPin = 7;

// Sensor power pin configuration (adjust as needed)
const int sensorPowerPin = 8;

// Function prototypes
void updateValue();
void showDisplay();
void onButtonPress();
void sleep();

// Variable to store gas value
volatile int gasValue = 0;
volatile bool buttonPressed = false;
volatile int wakeUpCount = 0;
const int wakeUpLimit = 15; // 15 * 8 seconds = 120 seconds (2 minutes)

// Variable to store the number of watchdog timer wake-ups needed for 2 minutes
//const int watchdogWakeUps = 15;

void setup() {
  // Initialize the button pin as input with pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize the sensor power pin as output and turn it off initially
  pinMode(sensorPowerPin, OUTPUT);
  digitalWrite(sensorPowerPin, LOW);

  // Attach interrupt to the button pin
  attachInterrupt(digitalPinToInterrupt(buttonPin), onButtonPress, FALLING);

  // Turn off ADC before sleep to save power
  ADCSRA &= ~(1 << ADEN);

  // Initial message indicating readiness (using Serial for demonstration)
  Serial.begin(9600);
  delay(1000); // Allow time for Serial to stabilize
  Serial.println("Gas Detector Ready");

  // Update value initially
  updateValue();
}

void loop() {
  // Enter power down state with ADC and BOD disabled
  sleep();

  // Re-initialize Serial communication after waking up
  Serial.begin(9600);
  delay(1000); // Allow time for Serial to stabilize
  Serial.println("Woke up!");

  // Wake-up routine
  if (buttonPressed) {
    showDisplay();
    buttonPressed = false;
  }

  updateValue();
}

void updateValue() {
  // Turn on the sensor
  digitalWrite(sensorPowerPin, HIGH);

  // Allow some time for the sensor to stabilize if necessary (adjust as needed)
  delay(10000);

  // Enable ADC to read the sensor value
  ADCSRA |= (1 << ADEN);
  gasValue = analogRead(A0);  // Assuming gas sensor is connected to A0
  ADCSRA &= ~(1 << ADEN);  // Disable ADC to save power

  // Turn off the sensor
  digitalWrite(sensorPowerPin, LOW);

  // Print the new value to Serial Monitor for debugging
  Serial.print(".....Updated Gas Value: ");
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
  // Reset wake-up count
  wakeUpCount = 0;

  // Configure watchdog timer for 8-second intervals
  wdt_enable(WDTO_8S);
  WDTCSR |= (1 << WDIE);

  while (wakeUpCount < wakeUpLimit) {
    // Disable ADC
    ADCSRA &= ~(1 << ADEN);

    // Set sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    // Disable BOD (Brown-out Detector) to save power
    sleep_bod_disable();

    // Disable interrupts while setting sleep bits
    noInterrupts();
    sleep_cpu();  // The device will sleep here until an interrupt occurs
    interrupts();

    // The device will continue from here after it wakes up
    sleep_disable(); // Disable sleep mode after waking up

    // Re-enable ADC
    ADCSRA |= (1 << ADEN);

    // Increment wake-up count
    wakeUpCount++;

    // Exit loop if button is pressed
    if (buttonPressed) {
      break;
    }
  }

  // Disable watchdog timer after the required sleep duration
  wdt_disable();
}

ISR(WDT_vect) {
  // Watchdog timer interrupt service routine (ISR)
  // Increment wake-up count
  wakeUpCount++;
}
