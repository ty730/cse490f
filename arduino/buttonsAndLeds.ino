const int INPUT_BUTTON_PIN1 = 2;
const int INPUT_BUTTON_PIN2 = 6;
const int INPUT_BUTTON_PIN3 = 10;
const int OUTPUT_LED_PIN1 = 4;
const int OUTPUT_LED_PIN2 = 8;
const int OUTPUT_LED_PIN3 = 12;
const int DELAY = 30;
const int BAUD_RATE = 9600;

void setup() {
  // set button pins to input
  pinMode(INPUT_BUTTON_PIN1, INPUT);
  pinMode(INPUT_BUTTON_PIN2, INPUT);
  pinMode(INPUT_BUTTON_PIN3, INPUT);
  // set LED pins to output
  pinMode(OUTPUT_LED_PIN1, OUTPUT);
  pinMode(OUTPUT_LED_PIN2, OUTPUT);
  pinMode(OUTPUT_LED_PIN3, OUTPUT);
  Serial.begin(BAUD_RATE); // for printing to console
}

void loop() {
  // get current values of button input pins
  int buttonVal1 = digitalRead(INPUT_BUTTON_PIN1); // returns 0 (LOW) or 1 (HIGH)
  int buttonVal2 = digitalRead(INPUT_BUTTON_PIN2); // returns 0 (LOW) or 1 (HIGH)
  int buttonVal3 = digitalRead(INPUT_BUTTON_PIN3); // returns 0 (LOW) or 1 (HIGH)
  // print out values of button input pins
  Serial.print("Buttons 1,2,3: ");
  Serial.print(buttonVal1);
  Serial.print(",");
  Serial.print(buttonVal2);
  Serial.print(",");
  Serial.println(buttonVal3);
  // set LED output pins to values of button input pins
  digitalWrite(OUTPUT_LED_PIN1, buttonVal1);
  digitalWrite(OUTPUT_LED_PIN2, buttonVal2);
  digitalWrite(OUTPUT_LED_PIN3, buttonVal3);
  delay(DELAY); // delay
}
