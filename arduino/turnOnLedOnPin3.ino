const int LED_OUTPUT_PIN = 3;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_OUTPUT_PIN, HIGH); // turn LED on (output 5V)
  delay(500); // wait half a second
  digitalWrite(LED_OUTPUT_PIN, LOW); // turn LED off (output 0V)
  delay(500); // wait half a second
}
