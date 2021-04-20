const int INPUT_BUTTON_PIN = 2;
const int OUTPUT_LED_PIN = LED_BUILTIN;
const int BAUD_RATE = 9600;
const int DELAY = 30;

void setup() {
  // set input and output pins
  pinMode(INPUT_BUTTON_PIN, INPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  Serial.begin(BAUD_RATE); // for printing
}

void loop() {
  int buttonValue = digitalRead(INPUT_BUTTON_PIN); // get current button value (HIGH or LOW)
  Serial.println(buttonValue); // print button value
  digitalWrite(OUTPUT_LED_PIN, buttonValue); // output the value to the LED
  delay(DELAY); // delay
}
