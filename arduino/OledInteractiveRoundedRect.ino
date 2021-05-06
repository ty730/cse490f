#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int MAX_ANALOG_INPUT = 1023;
const int SIZE_ANALOG_INPUT_PIN = A0;
const int ROUNDNESS_ANALOG_INPUT_PIN = A1;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  _display.clearDisplay();         
}

void loop() {
  // On each loop, we'll want to clear the display so we're not writing over
  // previously drawn data
  _display.clearDisplay(); 

  // Read the analog input values
  int sizeSensorVal = analogRead(SIZE_ANALOG_INPUT_PIN);
  int roundSensorVal = analogRead(ROUNDNESS_ANALOG_INPUT_PIN);

  // The maximum size side length is either the display width or height, whichever is smallest
  int maxSize = min(_display.width(), _display.height());

  // Now calculate the side length size based on the sensor val
  int sideLength = map(sizeSensorVal, 0, MAX_ANALOG_INPUT, 0, maxSize);
  // Now calculate the corner radius based on the sensor val
  int cornerRadius = map(roundSensorVal, 0, MAX_ANALOG_INPUT, 0, sideLength/2);

  // Center the rounded rectangle
  int xCenter = _display.width() / 2;
  int yCenter = _display.height() / 2;

  // Draw it on the screen
  _display.fillRoundRect(xCenter - sideLength/2, yCenter - sideLength/2, sideLength, sideLength, cornerRadius, SSD1306_WHITE);

  // Render the graphics buffer to screen
  _display.display(); 

  delay(50);
}
