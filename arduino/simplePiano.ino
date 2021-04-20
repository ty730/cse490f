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
const int OUTPUT_PIEZO_PIN = 9; // pin that supplies variable voltage to piezo
const int OUTPUT_LED_PIN = LED_BUILTIN; // when button is pressed built in LED turns on
// this defines the buttons to be using pull-up resistors (internal)
// if it is false then it would use pull-down resistor
const boolean buttonsAreActiveLow = true; 

void setup() {
  // set input and output pins and use INPUT_PULLUP to use an internal pull-up resistor
  pinMode(INPUT_BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_D_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_E_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_F_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
}

void loop() {
  // if a button is pressed then play its corresponding tone on the piezo.
  // If no button is pressed then use noTone
  if (buttonPressed(INPUT_BUTTON_C_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_C);
  } else if(buttonPressed(INPUT_BUTTON_D_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_D);
  } else if(buttonPressed(INPUT_BUTTON_E_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_E);
  } else if(buttonPressed(INPUT_BUTTON_F_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_F);
  } else if(buttonPressed(INPUT_BUTTON_G_PIN)) {
    tone(OUTPUT_PIEZO_PIN, KEY_G);
  } else {
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }
}

boolean buttonPressed(int pinOfButton) {
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
