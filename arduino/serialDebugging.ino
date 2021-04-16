const int BAUD_RATE = 9600;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE); // opens serial port, sets data rate to BAUD_RATE bps
  Serial.println("Hello world!");
}

void loop() {
  unsigned long currentTimeInMillis = millis(); // get current time in milliseconds
  Serial.print("Time since we started: ");
  Serial.print(currentTimeInMillis);
  Serial.println(" ms");
  delay(500);
}
