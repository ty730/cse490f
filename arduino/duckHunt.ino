
// #include <SPI.h> // Comment out when using i2c
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ParallaxJoystick.hpp>;

// 'bullet', 33x60px
const unsigned char bulletBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 
  0x38, 0x0e, 0x00, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x80, 0x00, 0x00, 0xc0, 
  0x01, 0xc0, 0x00, 0x01, 0x80, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 
  0x20, 0x00, 0x06, 0x00, 0x00, 0x30, 0x00, 0x06, 0x00, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x00, 0x18, 
  0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 
  0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x0f, 
  0xff, 0xff, 0xf8, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 
  0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 
  0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 
  0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 
  0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 
  0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 
  0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 
  0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 
  0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 
  0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x06, 
  0x00, 0x00, 0x30, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xff, 
  0xff, 0xf8, 0x00, 0x0c, 0x00, 0x00, 0x18, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xff, 0xff, 
  0xf8, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char STR_TITLE[] = "OUT OF AMMO";
const int DELAY_MS = 5;
const long BAUD_RATE = 115200;
const int OUTPUT_PIEZO_PIN = 9;
const int JOYSTICK_UPDOWN_PIN = A1;
const int JOYSTICK_LEFTRIGHT_PIN = A0;

const int MAX_ANALOG_VAL = 1023;
const enum JoystickYDirection JOYSTICK_Y_DIR = RIGHT;

// Analog joystick for left player
ParallaxJoystick _analogJoystick(JOYSTICK_UPDOWN_PIN, JOYSTICK_LEFTRIGHT_PIN, MAX_ANALOG_VAL, JOYSTICK_Y_DIR);

int _bullets = 3;
int _prevJoystickVal = 500;

void setup() {
  Serial.begin(BAUD_RATE);

  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  _display.clearDisplay();
  _display.setTextSize(1);      // Normal 1:1 pixel scale
  _display.setTextColor(SSD1306_WHITE); // Draw white text
  _display.setCursor(0, 0);     // Start at top-left corner
  _display.print("Waiting to receive\ndata from serial...");
  _display.display();
}

void loop() {
  // Check to see if there is any incoming serial data
  if(Serial.available() > 0){
    // If we're here, then serial data has been received
    // Read data off the serial port until we get to the endline delimeter ('\n')
    // Store all of this data into a string
    String rcvdSerialData = Serial.readStringUntil('\n'); 
    int numBullets = rcvdSerialData.toInt();
    // Set the number of bullets to be the data recieved from the Web.
    if (numBullets != 0) {
      _bullets = numBullets;
    }
  }

  _display.clearDisplay();

  // Read analog joystick
  _analogJoystick.read();
  int leftRightVal = _analogJoystick.getLeftRightVal();
  // if joystick is right (up towards OLED) then it means to shoot.
  // if joystick is left (down away from OLED) then it means to start game.
  if (leftRightVal > 600 && _prevJoystickVal < 600) {
    _bullets -= 1;
    Serial.println("shoot");
    tone(OUTPUT_PIEZO_PIN, 200);
  } else if (leftRightVal < 400 && _prevJoystickVal > 400) {
    Serial.println("start");
    noTone(OUTPUT_PIEZO_PIN);
  } else {
    noTone(OUTPUT_PIEZO_PIN);
  }
  _prevJoystickVal = leftRightVal; // track previous value so we don't send more than once per user action
  
  if (_bullets <= 0) { // if number of bullets is zero or less draw "OUT OF AMMO"
    drawTitleText();
  }
  if (_bullets > 0) { // if number of bullets is 1 or more then draw first bullet
    _display.drawBitmap(7, 2,  bulletBitmap, 33, 60, WHITE);
  }
  if (_bullets > 1) { // if number of bullets is 2 or more then draw second bullet
    _display.drawBitmap(47, 2,  bulletBitmap, 33, 60, WHITE);
  }
  if (_bullets > 2) { // if number of bullets is 3 or more then draw third bullet
    _display.drawBitmap(87, 2,  bulletBitmap, 33, 60, WHITE);
  }
  _display.display();
}

/**
 * Draws the text "OUT OF AMMO" centered on the screen
 * 
 * See: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#characters-and-text-2002798-24
 */
void drawTitleText() {
  int16_t x, y;
  uint16_t textWidth, textHeight;

  // Setup text rendering parameters
  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);

  // Measure the text with those parameters
  _display.getTextBounds(STR_TITLE, 0, 0, &x, &y, &textWidth, &textHeight);

  // Center the text on the display
  _display.setCursor(_display.width() / 2 - textWidth / 2, _display.height() / 2 - textHeight / 2);

  // Print out the string
  _display.print(STR_TITLE);
}
