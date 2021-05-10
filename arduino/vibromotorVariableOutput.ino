const int VIBROMOTOR_OUTPUT_PIN = 3;
const int POT_INPUT_PIN = A0;

void setup() {
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
  pinMode(POT_INPUT_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(POT_INPUT_PIN);
  int vibration = map(potValue, 0, 1023, 0, 255);
  analogWrite(VIBROMOTOR_OUTPUT_PIN, vibration);
  delay(30);
}
