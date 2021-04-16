const int RED_PIN = 6; // pin for red part of RGB LED
const int GREEN_PIN  = 5; // pin for green part of RGB LED
const int BLUE_PIN  = 3; // pin for blue part of RGB LED
const boolean COMMON_ANODE = false; // whether the RGB LED is common anode or common cathode
const int MAX_COLOR_VALUE = 255; // the maximum color value
const int BAUD_RATE = 9600; // rate of serial
const int INTERVAL = 40; // interval between changing colors in milliseconds

enum RGB{
  RED,
  GREEN,
  BLUE,
  NUM_COLORS
};

int rgbLedValues[] = {255, 0, 0}; // Red, Green, Blue
enum RGB curFadingUpColor = GREEN;
enum RGB curFadingDownColor = RED;
const int FADE_STEP = 5; 

unsigned long prevTime = 0; // last time the colors were updated

void setup() {
  // Set pins to output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Turn on Serial for printing
  Serial.begin(BAUD_RATE); 
  Serial.println("Red, Green, Blue");

  // Set initial color
  setColor(rgbLedValues[RED], rgbLedValues[GREEN], rgbLedValues[BLUE]);
}

void loop() {

  unsigned long currentTime = millis(); // get the current time in ms

  // if the difference between our current time and the
  // time we last updated is equal or greater than our time
  // interval then we should update again
  if (currentTime - prevTime >= INTERVAL) {
    prevTime = currentTime; // the last time we updated is now!

    // Increment and decrement the RGB LED values for the current
    // fade up color and the current fade down color
    rgbLedValues[curFadingUpColor] += FADE_STEP;
    rgbLedValues[curFadingDownColor] -= FADE_STEP;
  
    // If we reach the max color value for fading up, then set the
    // fading up color to the next color.
    // Note: the order of colors is given in enum
    if(rgbLedValues[curFadingUpColor] > MAX_COLOR_VALUE){
      rgbLedValues[curFadingUpColor] = MAX_COLOR_VALUE;
      curFadingUpColor = (RGB)((int)curFadingUpColor + 1);
  
      if(curFadingUpColor > (int)BLUE){
        curFadingUpColor = RED;
      }
    }
  
    // If we reach the max color value for fading down, then set the
    // fading down color to the next color.
    // Note: the order of colors is given in enum
    if(rgbLedValues[curFadingDownColor] < 0){
      rgbLedValues[curFadingDownColor] = 0;
      curFadingDownColor = (RGB)((int)curFadingDownColor + 1);
  
      if(curFadingDownColor > (int)BLUE){
        curFadingDownColor = RED;
      }
    }
  
    // Set the color and then wait
    setColor(rgbLedValues[RED], rgbLedValues[GREEN], rgbLedValues[BLUE]);
  }
}

/**
 * setColor has parameters red, green, and blue, which all represent
 * color values (between 0 and 255) for those respective LED colors.
 * A value of 0 is none of that color, and 255 is maximum of that color.
 */
void setColor(int red, int green, int blue)
{
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);

  // if LED is common anode reverse the color values
  if(COMMON_ANODE == true){
    red = MAX_COLOR_VALUE - red;
    green = MAX_COLOR_VALUE - green;
    blue = MAX_COLOR_VALUE - blue;
  }
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);  
}
