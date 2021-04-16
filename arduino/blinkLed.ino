const int LED_OUTPUT_PIN = 3;
const int BAUD_RATE = 9600;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_OUTPUT_PIN, OUTPUT);
  Serial.begin(BAUD_RATE);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_OUTPUT_PIN, HIGH); // turn LED on (output 5V)
  Serial.println("Pin 3 is HIGH (5V)");
  delay(500); // wait half a second
  digitalWrite(LED_OUTPUT_PIN, LOW); // turn LED off (output 0V)
  Serial.println("Pin 3 is LOW (0V)");
  delay(500); // wait half a second
}
