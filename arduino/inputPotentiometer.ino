const int BAUD_RATE = 9600;

void setup() {
  Serial.begin(BAUD_RATE); // for printing values to console
}

void loop() {
  int potentiometerValue = analogRead(A0); // returns 0 - 1023 (due to 10 bit ADC)
  Serial.println(potentiometerValue); // print value
  delay(50); // read new values at ~20Hz
}
