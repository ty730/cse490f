const int VIBROMOTOR_OUTPUT_PIN = 3;

void setup() {
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
  delay(1000);
  digitalWrite(VIBROMOTOR_OUTPUT_PIN, LOW);
  delay(1000);
}
