#include <PushButton.h>
// Frequencies (in Hz) of piano keys
#define KEY_C 262  // 261.6256 Hz (middle C)
#define KEY_D 294  // 293.6648 Hz
#define KEY_E 330  // 329.6276 Hz
#define KEY_F 350  // 349.2282 Hz
#define KEY_G 392  // 391.9954 Hz
// input buttons are laid out with lowest frequency on the left (pin 2)
// and frequencies increase as you go to the right
const int INPUT_BUTTON_C_PIN = 2;
const int INPUT_BUTTON_D_PIN = 3;
const int INPUT_BUTTON_E_PIN = 4;
const int INPUT_BUTTON_F_PIN = 5;
const int INPUT_BUTTON_G_PIN = 6;
// initialize all buttons to use debounced button library
// found at https://github.com/kristianklein/PushButton
PushButton btnC(2);
PushButton btnD(3);
PushButton btnE(4);
PushButton btnF(5);
PushButton btnG(6);
const int OUTPUT_PIEZO_PIN = 9; // pin that supplies variable voltage to piezo
const int OUTPUT_LED_PIN = LED_BUILTIN; // when button is pressed built in LED turns on

void setup() {
  // set input and output pins and use INPUT_PULLUP to use an internal pull-up resistor
  pinMode(INPUT_BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_D_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_E_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_F_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  // configure all buttons to be active low
  btnC.setActiveLogic(LOW);
  btnD.setActiveLogic(LOW);
  btnE.setActiveLogic(LOW);
  btnF.setActiveLogic(LOW);
  btnG.setActiveLogic(LOW);
}

void loop() {
  btnC.update();
  btnD.update();
  btnE.update();
  btnF.update();
  btnG.update();
  // if a button is pressed then play its corresponding tone on the piezo.
  // If no button is pressed then use noTone
  if (btnC.isClicked()) {
    tone(OUTPUT_PIEZO_PIN, KEY_C);
    digitalWrite(OUTPUT_LED_PIN, HIGH);
  } else if(btnD.isClicked()) {
    tone(OUTPUT_PIEZO_PIN, KEY_D);
    digitalWrite(OUTPUT_LED_PIN, HIGH);
  } else if(btnE.isClicked()) {
    tone(OUTPUT_PIEZO_PIN, KEY_E);
    digitalWrite(OUTPUT_LED_PIN, HIGH);
  } else if(btnF.isClicked()) {
    tone(OUTPUT_PIEZO_PIN, KEY_F);
    digitalWrite(OUTPUT_LED_PIN, HIGH);
  } else if(btnG.isClicked()) {
    tone(OUTPUT_PIEZO_PIN, KEY_G);
    digitalWrite(OUTPUT_LED_PIN, HIGH);
  } else {
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }
}
