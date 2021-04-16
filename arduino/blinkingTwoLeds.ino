const int LED_OUTPUT_PIN1 = 3; // anode output pin
const int LED_OUTPUT_PIN2 = 4; // cathode output pin
const int INTERVAL = 500; // time in milliseconds between switching output states (HIGH and LOW)

int ledState = LOW; // current state of the output LEDs
unsigned long prevTime = 0; // last recorded time of LED states being updated in milliseconds

void setup() {
  pinMode(LED_OUTPUT_PIN1, OUTPUT); // set pin 3 as the anode output pin
  pinMode(LED_OUTPUT_PIN2, OUTPUT); // set pin 4 as the cathode output pin
}

void loop() {

  // get the current time in milliseconds
  unsigned long currentTime = millis();

  // if its been INTERVAL amount of time since our last update
  // then do another update
  if (currentTime - prevTime >= INTERVAL) {
    prevTime = currentTime; // the last update is the one we are doing right now!
    // change the state to the opposite of its current state
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    // set the output pins as the current state
    digitalWrite(LED_OUTPUT_PIN1, ledState);
    digitalWrite(LED_OUTPUT_PIN2, ledState);
  }
}
