#include <RGBConverter.h> // using library https://github.com/ratkins/RGBConverter
#include <PushButton.h> // using library https://github.com/kristianklein/PushButton
#include <Servo.h> // using library https://github.com/arduino-libraries/Servo

const int RED_PIN = 6; // pin for red part of RGB LED
const int GREEN_PIN  = 5; // pin for green part of RGB LED
const int BLUE_PIN  = 3; // pin for blue part of RGB LED
const int INPUT_PHOTOCELL_PIN = A0; // pin to connect photocell to
const int INPUT_FSR_PIN = A1; // pin to connect FSR to
const int INPUT_SAVECOLOR_BUTTON_PIN = 2; // pin to connect save color button to
const int NUM_OF_COLORS = 7; // number of different colors that can be selected
                             // in mode 2
const int SERVO_SIGNAL_PIN = 9; // pin to connect servo to
const int INPUT_POT_PIN = A2; // pin to connect potentiometer to
const int INPUT_MODE_BUTTON_PIN = 4; // pin to connect mode switching button to
const boolean COMMON_ANODE = false; // whether the RGB LED is common anode or common cathode
const int MAX_COLOR_VALUE = 255; // the maximum color value
const int INTERVAL = 40; // interval between changing colors in milliseconds
const int BAUD_RATE = 9600;

float _hue = 0; // hue varies between 0 - 1
float _step = 0.001f; // amount to change hue by

int redState = 255; // state of red part of RGB LED in mode 2
int greenState = LOW; // state of green part of RGB LED in mode 2
int blueState = LOW; // state of blue part of RGB LED in mode 2
boolean saveColor = false; // if the color is being saved or not

int pos = 0; // variable to store the servo position

int mode = 0; // current mode (will be moded by 3 to get number
              // 0, 1, or 2, where 0 is 1st mode, 1 is 2nd mode, and 2 is 3rd mode)

// use library to convert from HSL to RGB color space,
// found at https://github.com/ratkins/RGBConverter
RGBConverter _rgbConverter;
// initialize buttons to use debounced button library
// found at https://github.com/kristianklein/PushButton
PushButton saveColorButton(INPUT_SAVECOLOR_BUTTON_PIN);
PushButton modeButton(INPUT_MODE_BUTTON_PIN);
// create servo object to control the servo,
// library found at https://github.com/arduino-libraries/Servo
Servo myservo;  

void setup() {
  // set up input and output pins
  pinMode(INPUT_PHOTOCELL_PIN, INPUT);
  pinMode(INPUT_FSR_PIN, INPUT);
  pinMode(INPUT_SAVECOLOR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INPUT_MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INPUT_POT_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  myservo.attach(SERVO_SIGNAL_PIN);  // attaches the servo to the signal pin
  Serial.begin(BAUD_RATE); // for printing to console
  
  // configure buttons to be active low
  saveColorButton.setActiveLogic(LOW);
  modeButton.setActiveLogic(LOW);
}

void loop() {
  // update button so we can know if they are clicked
  saveColorButton.update();
  modeButton.update();
  // if the mode button is clicked go to the next mode
  if (modeButton.isClicked()) {
    mode += 1;
  }
  // check which mode we are in and do corresponding functionality
  if (mode % 3 == 0) { // mode 1
    int inputValue = analogRead(INPUT_PHOTOCELL_PIN); // returns 0-1023 (bc 10 bit ADC)
    float outputLightness = 0.5 - (float)inputValue / 2046.0; // convert from 0-1023 range to 0-0.5 range
    // print out the mapping of value from PHOTOCELL input to LED lightness
    Serial.print("photocell value -> LED lightness: ");
    Serial.print(inputValue);
    Serial.print(" -> ");
    Serial.println(outputLightness);
  
    // Convert current hue, saturation, and lightness to RGB
    // The library assumes hue, saturation, and lightness range from 0 - 1
    // and that RGB ranges from 0 - 255
    // If lightness is equal to 1, then the RGB LED will be white
    byte rgb[3];
    _rgbConverter.hslToRgb(_hue, 1, outputLightness, rgb);
    setColor(rgb[0], rgb[1], rgb[2]); 
    // update hue based on step size
    _hue += _step;
    // hue ranges between 0-1, so if > 1, reset to 0
    if(_hue > 1.0){
      _hue = 0;
    }
  } else if (mode % 3 == 1) { // mode 2
    int inputValue = analogRead(INPUT_FSR_PIN); // returns 0-1023 (bc 10 bit ADC)
    // if save color is clicked then toggle the save color state
    if (saveColorButton.isClicked()) {
      saveColor = !saveColor;
    }
    // if we are not saving the color then the color is chosen depending on
    // the FSRs input value
    if (!saveColor) {
      if (inputValue < 1023 / NUM_OF_COLORS) { // white
        redState = MAX_COLOR_VALUE;
        greenState = MAX_COLOR_VALUE;
        blueState = MAX_COLOR_VALUE;
      } else if (inputValue < 1023 / NUM_OF_COLORS * 2) { // green
        redState = LOW;
        greenState = MAX_COLOR_VALUE;
        blueState = LOW;
      } else if (inputValue < 1023 / NUM_OF_COLORS * 3) { // blue
        redState = LOW;
        greenState = LOW;
        blueState = MAX_COLOR_VALUE;
      } else if (inputValue < 1023 / NUM_OF_COLORS * 4) { // purple
        redState = MAX_COLOR_VALUE;
        greenState = LOW;
        blueState = MAX_COLOR_VALUE;
      } else if (inputValue < 1023 / NUM_OF_COLORS * 5) { // turqoise
        redState = LOW;
        greenState = MAX_COLOR_VALUE;
        blueState = MAX_COLOR_VALUE;
      } else if (inputValue < 1023 / NUM_OF_COLORS * 6) { // red
        redState = MAX_COLOR_VALUE;
        greenState = LOW;
        blueState = LOW;
      } else { // yellow
        redState = MAX_COLOR_VALUE;
        greenState = MAX_COLOR_VALUE;
        blueState = LOW;
      }
    }
    setColor(redState, greenState, blueState); // set the RGB LED to the chosen color
  } else { // mode 3
    int potentiometerValue = analogRead(INPUT_POT_PIN); // returns 0 - 1023 (due to 10 bit ADC)
    int servoPosition = map(potentiometerValue, 0, 1023, 0, 180); // convert from 0-1023 range to 0-180 range
    myservo.write(servoPosition); // write the current position to the servo,
                                  // which is determined by the potentiometer
  }
  delay(INTERVAL); // delay by the interval amount
}

/**
 * setColor has parameters red, green, and blue, which all represent
 * color values (between 0 and 255) for those respective LED colors.
 * A value of 0 is none of that color, and 255 is maximum of that color.
 */
void setColor(int red, int green, int blue) {
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
