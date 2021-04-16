const int LED_OUTPUT_PIN = 3;
const int MAX_ANALOG_OUT = 255; // the max analog output is 255
const int INTERVAL = 20;

int fadeAmount = 5;      // amount of fade on each loop
int curBrightness = 0;   // brightness of LED
unsigned long prevTime = 0; // track the last time the LED was updated

void setup() {
  // set the LED pin to as an output
  pinMode(LED_OUTPUT_PIN, OUTPUT);
  Serial.begin(9600); // for printing with serial
}

void loop() {

  // get the current time
  unsigned long currentTime = millis();

  // if the difference between the current time
  // and the previous time we updated exceeds or equals the
  // interval length then update brightnesses
  if (currentTime - prevTime >= INTERVAL) {
    // set the brightness of the LED pin
    analogWrite(LED_OUTPUT_PIN, curBrightness);

    // make the previous time the current time
    prevTime = currentTime;
  
    // increase brightness by fade amount
    curBrightness = curBrightness + fadeAmount;
  
    // change direction of brightness once reached max
    if (curBrightness <= 0 || curBrightness >= MAX_ANALOG_OUT) {
      fadeAmount = -fadeAmount;
    }
  }
}
