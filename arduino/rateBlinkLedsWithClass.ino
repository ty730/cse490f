#include "Blinker.h"

Blinker led1Blinker(2, 200);  // specify pin and blink interval (200ms)
Blinker led2Blinker(5, 400);  // specify pin and blink interval (333ms)
Blinker led3Blinker(9, 1200); // specify pin and blink interval (1111ms)

void setup() {
}

void loop() {
  // update each LED blinker in the loop, the blinker class
  // will handle if the state of the LED will change or not
  led1Blinker.update();
  led2Blinker.update();
  led3Blinker.update();
}
