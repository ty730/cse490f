const int RED_PIN = 6; // pin for red part of RGB LED
const int GREEN_PIN = 5; // pin for green part of RGB LED
const int BLUE_PIN = 3; // pin for blue part of RGB LED
const int BAUD_RATE = 9600; // rate of serial
const boolean IS_COMMON_ANODE = false; // if RBG LED is common anode or common cathode
const int DELAY = 500; // delay in milliseconds between color switch

void setup() {
  // Set the pins to output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Turn on Serial for printing
  Serial.begin(BAUD_RATE); 
}

void loop() {

  // change to red
  Serial.print("Red: ");
  setRgbLedColor(HIGH, LOW, LOW);
  delay(DELAY);
  
  // change to green
  Serial.print("Green: ");
  setRgbLedColor(LOW, HIGH, LOW);
  delay(DELAY);
  
  // change to blue
  Serial.print("Blue: ");
  setRgbLedColor(LOW, LOW, HIGH);
  delay(DELAY);
  
  // change to purple
  Serial.print("Purple: ");
  setRgbLedColor(HIGH, LOW, HIGH);
  delay(DELAY);
  
  // change to turqoise
  Serial.print("Turqoise: ");
  setRgbLedColor(LOW, HIGH, HIGH);
  delay(DELAY);
  
  // change to white
  Serial.print("White: ");
  setRgbLedColor(HIGH, HIGH, HIGH);
  delay(DELAY);
}


/**
 * setRgbLedColor has parameters red, green, and blue,
 * which signify a value (HIGH or LOW) to give the red, green
 * and blue componenents of the RGB LED.
 */
void setRgbLedColor(int red, int green, int blue)
{
  // If a common anode reverse values
  if(IS_COMMON_ANODE == true){
    red = !red;
    green = !green;
    blue = !blue;
  }

  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);
  
  digitalWrite(RED_PIN, red);
  digitalWrite(GREEN_PIN, green);
  digitalWrite(BLUE_PIN, blue);  
}
