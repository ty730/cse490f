const int INPUT_POTMETER_PIN = A0;
const int OUTPUT_LED_PIN = 3;
const int BAUD_RATE = 9600;

void setup() {
  pinMode(INPUT_POTMETER_PIN, INPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  Serial.begin(BAUD_RATE); // for printing values to console
}

void loop() {
  int potentiometerValue = analogRead(A0); // returns 0 - 1023 (due to 10 bit ADC)
  int outputValue = map(potentiometerValue, 0, 1023, 0, 255); // convert from 0-1023 range to 0-255 range
  // print out the mapping of value from FSR input to LED output
  Serial.print("Potentiometer value -> LED value: ");
  Serial.print(potentiometerValue);
  Serial.print(" -> ");
  Serial.println(outputValue);
  analogWrite(OUTPUT_LED_PIN, outputValue);
  delay(50); // read new values at ~20Hz
}
