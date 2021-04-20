const int OUTPUT_LED_PIN = LED_BUILTIN; // LED to output to
const int INPUT_FSR_PIN = A0; // pin to connect FSR to
const int DELAY = 30; // delay interval
const int BAUD_RATE = 9600;

void setup() {
  // set up input and output pins
  pinMode(INPUT_FSR_PIN, INPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  Serial.begin(BAUD_RATE); // for printing to console
}

void loop() {
  int inputValue = analogRead(INPUT_FSR_PIN); // returns 0-1023 (bc 10 bit ADC)
  int outputValue = map(inputValue, 0, 1023, 0, 255); // convert from 0-1023 range to 0-255 range
  // print out the mapping of value from FSR input to LED output
  Serial.print("FSR value -> LED value: ");
  Serial.print(inputValue);
  Serial.print(" -> ");
  Serial.println(outputValue);
  // output the mapped value to the LED
  analogWrite(OUTPUT_LED_PIN, outputValue);
  delay(DELAY); // delay
}
