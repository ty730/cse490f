#include <PushButton.h>
const int VIBROMOTOR_OUTPUT_PIN = 3;
const int BUTTON_INPUT_PIN = 2;
const int DELAY_FAST = 200;
const int DELAY_MEDIUM = 500;
const int DELAY_SLOW = 600;

enum RGB{
  CALM,
  REGULAR,
  URGENT
};

int _mode = CALM;
int _vibroState = 0;
unsigned long prevTime = 0; // last time the colors were updated

// initialize button to use debounced button library
// found at https://github.com/kristianklein/PushButton
PushButton btn(BUTTON_INPUT_PIN);

void setup() {
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
  pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  btn.setActiveLogic(LOW);
}

void loop() {
  btn.update();
  int buttonValue = digitalRead(BUTTON_INPUT_PIN);
  if (btn.isClicked()) {
    if (_mode == URGENT) {
      _mode = CALM;
    } else {
      _mode += 1;
    }
  }
  Serial.print("Mode: ");
  if (_mode == 0) {
    Serial.println("CALM");
  } else if (_mode == 1) {
    Serial.println("REGULAR");
  } else {
    Serial.println("URGENT");
  }

  unsigned long currentTime = millis(); // get the current time in ms
  
  if (_mode % 3 == 0) { // calm alert
    if (currentTime - prevTime >= DELAY_SLOW) {
      prevTime = currentTime;
      if (_vibroState > 0) {
        _vibroState = 0;
      } else {
        _vibroState = 50;
      } 
    }
  } else if (_mode % 3 == 1) { // regular alert
    if (currentTime - prevTime >= DELAY_MEDIUM) {
      prevTime = currentTime;
      if (_vibroState > 0) {
        _vibroState = 0;
      } else {
        _vibroState = 100;
      } 
    }
  } else { // urgent alert
    if (currentTime - prevTime >= DELAY_FAST) {
      prevTime = currentTime;
      if (_vibroState > 0) {
        _vibroState = 0;
      } else {
        _vibroState = 255;
      } 
    }
  }
  analogWrite(VIBROMOTOR_OUTPUT_PIN, _vibroState);
}
