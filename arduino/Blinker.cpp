#include "Blinker.h"

/**
* This Blinker constructor takes parameters pin and blinkInterval,
* pin specifies the output pin, and blinkInterval specifies the time
* in between each blink of the LED
*/
Blinker::Blinker(const int pin, const unsigned long blinkInterval) :
  _pin(pin), _interval(blinkInterval) // initialize const like this in C++
{
  _state = LOW;
  _prevTime = 0;
  pinMode(_pin, OUTPUT);
}

/**
* Calculates whether to toggle output state based on the set interval
* Call this function once per loop()
*/
void Blinker::update() {
  unsigned long currentTime = millis();

  if (currentTime - _prevTime >= _interval) {
    _prevTime = currentTime;
    _state = !_state;
    digitalWrite(_pin, _state);
  }
}
