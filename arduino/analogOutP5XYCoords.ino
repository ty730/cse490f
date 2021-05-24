const int DELAY_MS = 5;

const int ANALOG_INPUT_PIN1 = A0;
const int ANALOG_INPUT_PIN2 = A1;
const int MAX_ANALOG_INPUT = 1023;

int _lastAnalogValX = -1;
int _lastAnalogValY = -1;

// If false, only sends new data when the new analog value does not
// equal the last analog value. If true, always sends the data
boolean _alwaysSendData = true; 

void setup() {
  Serial.begin(115200);
}

void loop() {

  // Get the new analog value for x coordinate
  int analogValX = analogRead(ANALOG_INPUT_PIN1);
  // Get the new analog value for y coordinate
  int analogValY = analogRead(ANALOG_INPUT_PIN2);

  // If the analog value has changed, send a new one over serial
  if(_alwaysSendData || _lastAnalogValX != analogValX ||  _lastAnalogValY != analogValY){
    float sizeFracX = analogValX / (float)MAX_ANALOG_INPUT;
    float sizeFracY = analogValY / (float)MAX_ANALOG_INPUT;
    Serial.print(sizeFracX, 4); // 4 decimal point precision
    Serial.print(",");
    Serial.println(sizeFracY, 4); // 4 decimal point precision
  }

  _lastAnalogValX = analogValX;
  _lastAnalogValY = analogValY;
  delay(DELAY_MS);
}
