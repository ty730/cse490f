const int INPUT_FSR_PIN = A0;
const int OUTPUT_PIEZO_PIN = 3;
const int DELAY = 30;
const int BAUD_RATE = 9600;


void setup() {
  // set up input and output pins
  pinMode(INPUT_FSR_PIN, INPUT);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  Serial.begin(BAUD_RATE); // for printing
}

void loop() {
  int fsrValue = analogRead(INPUT_FSR_PIN); // returns 0-1023 (bc 10 bit ADC)
  int piezoOutputValue = map(fsrValue, 0, 1023, 50, 1500); // convert from 0-1023 range to 50- 1500 range
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
  delay(DELAY); // delay
}
