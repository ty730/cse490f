const int LED1_OUTPUT_PIN = 2;
const int LED1_INTERVAL = 200; // time in ms between blinks for LED1
unsigned long led1prevTime = 0; // the last time that LED1 was updated
int led1State = LOW; // the state of LED1 (HIGH or LOW)

const int LED2_OUTPUT_PIN = 5;
const int LED2_INTERVAL = 400; // time in ms between blinks for LED2
unsigned long led2prevTime = 0; // the last time that LED1 was updated
int led2State = LOW; // the state of LED2 (HIGH or LOW)

const int LED3_OUTPUT_PIN = 9;
const int LED3_INTERVAL = 1200; // time in ms between blinks for LED3
unsigned long led3prevTime = 0; // the last time that LED1 was updated
int led3State = LOW; // the state of LED3 (HIGH or LOW)

void setup() {
  // set output pins for LEDs
  pinMode(LED1_OUTPUT_PIN, OUTPUT);
  pinMode(LED2_OUTPUT_PIN, OUTPUT);
  pinMode(LED3_OUTPUT_PIN, OUTPUT);
}

void loop() {
  unsigned long currentTime = millis(); // get the current time

  // if the last time we updated LED1 was longer than the
  // blinking interval for LED1, then update it now
  if (currentTime - led1prevTime >= LED1_INTERVAL) {
    led1prevTime = currentTime;
    led1State = !led1State;
    digitalWrite(LED1_OUTPUT_PIN, led1State);
  }

  // if the last time we updated LED2 was longer than the
  // blinking interval for LED2, then update it now
  if (currentTime - led2prevTime >= LED2_INTERVAL) {
    led2prevTime = currentTime;
    led2State = !led2State;
    digitalWrite(LED2_OUTPUT_PIN, led2State);
  }

  // if the last time we updated LED3 was longer than the
  // blinking interval for LED3, then update it now
  if (currentTime - led3prevTime >= LED3_INTERVAL) {
    led3prevTime = currentTime;
    led3State = !led3State;
    digitalWrite(LED3_OUTPUT_PIN, led3State);
  }
}
