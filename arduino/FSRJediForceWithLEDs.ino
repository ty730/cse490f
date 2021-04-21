const int INPUT_FSR_PIN = A0;
const int OUTPUT_PIEZO_PIN = 3;
const int OUTPUT_LED_PIN1 = 5;
const int OUTPUT_LED_PIN2 = 6;
const int OUTPUT_LED_PIN3 = 9;
const int OUTPUT_LED_PIN4 = 10;
const int OUTPUT_LED_PIN5 = 11;
const int DELAY = 30;
const int BAUD_RATE = 9600;

void setup() {
  // set up input and output pins
  pinMode(INPUT_FSR_PIN, INPUT);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN1, OUTPUT);
  pinMode(OUTPUT_LED_PIN2, OUTPUT);
  pinMode(OUTPUT_LED_PIN3, OUTPUT);
  pinMode(OUTPUT_LED_PIN4, OUTPUT);
  pinMode(OUTPUT_LED_PIN5, OUTPUT);
  Serial.begin(BAUD_RATE); // for printing
}

void loop() {
  int fsrValue = analogRead(INPUT_FSR_PIN); // returns 0-1023 (bc 10 bit ADC)
  int piezoOutputValue = map(fsrValue, 0, 1023, 50, 1500); // convert from 0-1023 range to 50- 1500 range
  int ledValue = map(fsrValue, 0, 1023, 0, 255);
  // print out the mapping of value from FSR input to piezo output
  Serial.print("FSR value -> piezo value: ");
  Serial.print(fsrValue);
  Serial.print(" -> ");
  Serial.println(piezoOutputValue);
  // if FSR is not being pressed (fsrValue is zero),
  // then don't make a noise, otherwise have the piezo
  // output the corresponding tone
  if (fsrValue == 0) {
    noTone(OUTPUT_PIEZO_PIN);
  } else {
    tone(OUTPUT_PIEZO_PIN, piezoOutputValue);
  }
  int state1 = LOW;
  int state2 = LOW;
  int state3 = LOW;
  int state4 = LOW;
  int state5 = LOW;
  if (ledValue < 51) {
    state1 = HIGH;
  } else if (ledValue < 102) {
    state2 = HIGH;
  } else if (ledValue < 153) {
    state3 = HIGH;
  } else if (ledValue < 204) {
    state4 = HIGH;
  } else {
    state5 = HIGH;
  }
  digitalWrite(OUTPUT_LED_PIN1, state1);
  digitalWrite(OUTPUT_LED_PIN2, state2);
  digitalWrite(OUTPUT_LED_PIN3, state3);
  digitalWrite(OUTPUT_LED_PIN4, state4);
  digitalWrite(OUTPUT_LED_PIN5, state5);
  delay(DELAY); // delay
}
