#include <LowPower.h>
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

// Variable to store gas value
volatile int gasValue = 0;
volatile bool buttonPressed = false;
volatile int wakeUpCount = 0;
const int wakeUpLimit = 2; // Update every 16 seconds (2 * 8 seconds)

void setup() {
  // Reduce the clock frequency to 4 MHz (assuming original frequency is 8 MHz)
  clock_prescale_set(clock_div_2);

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
  while (!Serial); // Wait for serial port to connect. Needed for native USB
  Serial.println("Gas Detector Ready");

  // Update value initially
  updateValue();
}

void loop() {
  // Enter power down state with ADC and BOD disabled
  sleep();

  // Re-initialize Serial communication after waking up
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect. Needed for native USB
  delay(100); // Allow time for Serial to stabilize

  // Wake-up routine
  if (buttonPressed) {
    showDisplay();
    buttonPressed = false;
  }

  if (++wakeUpCount >= wakeUpLimit) {
    wakeUpCount = 0;
    updateValue();
  }
}

void updateValue() {
  // Turn on the sensor
  digitalWrite(sensorPowerPin, HIGH);

  // Allow some time for the sensor to stabilize if necessary (adjust as needed)
  delay(2000); // Adjusted for 4 MHz clock frequency

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
  delay(600); // Adjusted for 4 MHz clock frequency
}

void sleep() {
  // Sleep for 8 seconds
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
