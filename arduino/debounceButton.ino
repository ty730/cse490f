const int INPUT_BUTTON_PIN = 2;
const int OUTPUT_LED_PIN = 3;
const int DEBOUNCE_WINDOW = 40; // time of debounce window in milliseconds
const int BAUD_RATE = 9600;

boolean debounceWindowFinished = true; // if we are not in a debounce window
int _savedbuttonValue = LOW; // starts low because using pull-down resistor
unsigned long debounceStartTime = 0; // time when debounce window starts
int buttonValue = LOW; // debounce start state of button

void setup() {
  // set input and output pins
  pinMode(INPUT_BUTTON_PIN, INPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  Serial.begin(BAUD_RATE); // for printing
}

void loop() {
  // if we are not in a debounce window we can check the button value
  // and start a new debounce window
  if (debounceWindowFinished) {
    // get button value to start debounce window. Assume pull-down resistor,
    // so its HIGH when pressed
    buttonValue = digitalRead(INPUT_BUTTON_PIN);
    debounceStartTime = millis(); // get the current time in milliseconds
    debounceWindowFinished = false; // we are now in a debounce window
    Serial.print("before: ");
    Serial.println(buttonValue);
  }
  unsigned long currentTime = millis(); // get current time
  // if the current time is a full debounce window or more greater
  // than the debounce start time then we should end the debounce window
  // and update the LED (if we are in a steady state).
  if (currentTime - debounceStartTime >= DEBOUNCE_WINDOW) {
    debounceWindowFinished = true; // end the debounce window
    // read the button value again
    int buttonValue2 = digitalRead(INPUT_BUTTON_PIN);
    // if buttonValue and buttonValue2 are the same, then we are in steady state.
    // update the _savedbuttonValue to match the steady state (if it's not the same)
    if(buttonValue == buttonValue2 && _savedbuttonValue != buttonValue){
      _savedbuttonValue = buttonValue;
    }
    Serial.print("after: ");
    Serial.println(buttonValue2);
    // output to the LED the current state of the button (HIGH or LOW)
    digitalWrite(OUTPUT_LED_PIN, _savedbuttonValue);
  }
}
