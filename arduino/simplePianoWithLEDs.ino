#include <LEDFader.h>
// Frequencies (in Hz) of piano keys
#define KEY_C 262  // 261.6256 Hz (middle C)
#define KEY_D 294  // 293.6648 Hz
#define KEY_E 330  // 329.6276 Hz
#define KEY_F 350  // 349.2282 Hz
#define KEY_G 392  // 391.9954 Hz
// input buttons are laid out with lowest frequency on the left (pin 2)
// and frequencies increase as you go to the right
const int INPUT_BUTTON_C_PIN = 2;
const int INPUT_BUTTON_D_PIN = 4;
const int INPUT_BUTTON_E_PIN = 7;
const int INPUT_BUTTON_F_PIN = 8;
const int INPUT_BUTTON_G_PIN = 12;
const int OUTPUT_PIEZO_PIN = 9; // pin that supplies variable voltage to piezo
const int OUTPUT_LED_PIN = LED_BUILTIN; // when button is pressed built in LED turns on
const int BAUD_RATE = 9600;
// this defines the buttons to be using pull-up resistors (internal)
// if it is false then it would use pull-down resistor
const boolean buttonsAreActiveLow = true;
// Initialize fading LEDs
// used fading LED library from https://github.com/jgillick/arduino-LEDFader
LEDFader ledC = LEDFader(3);
LEDFader ledD = LEDFader(5);
LEDFader ledE = LEDFader(6);
LEDFader ledF = LEDFader(10);
LEDFader ledG = LEDFader(11);

void setup() {
  // set input and output pins and use INPUT_PULLUP to use an internal pull-up resistor
  pinMode(INPUT_BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_D_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_E_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_F_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  Serial.begin(BAUD_RATE);
}

void loop() {
  // update LEDs so they can change state in order to fade
  ledC.update();
  ledD.update();
  ledE.update();
  ledF.update();
  ledG.update();
  // if a button is pressed then play its corresponding tone on the 
  // piezo, and fade its corresponding LED.
  // If no button is pressed then use noTone
  if (buttonPressed(INPUT_BUTTON_C_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_C);
    ledC.fade(255, 30);
    ledC.fade(0, 1000);
  } else if(buttonPressed(INPUT_BUTTON_D_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_D);
    ledD.fade(255, 30);
    ledD.fade(0, 1000);
  } else if(buttonPressed(INPUT_BUTTON_E_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_E);
    ledE.fade(255, 30);
    ledE.fade(0, 1000);
  } else if(buttonPressed(INPUT_BUTTON_F_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_F);
    ledF.fade(255, 30);
    ledF.fade(0, 1000);
  } else if(buttonPressed(INPUT_BUTTON_G_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_G);
    ledG.fade(255, 30);
    ledG.fade(0, 1000);
  } else {
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }
}

boolean buttonPressed(int pinOfButton){
  int buttonValue = digitalRead(pinOfButton);
  // if there is a pull-up resistor and the buttons pin
  // has low input voltage, then it is pressed.
  // If there is a pull-down resistor and the buttons pin
  // has high input voltage, then it is pressed.
  // In any other case the button is not pressed.
  if(buttonsAreActiveLow && buttonValue == LOW){
    digitalWrite(OUTPUT_LED_PIN, HIGH);
    return true;
  }else if(!buttonsAreActiveLow && buttonValue == HIGH){
    digitalWrite(OUTPUT_LED_PIN, HIGH);
    return true;
  }
  // button not pressed
  return false;
}
