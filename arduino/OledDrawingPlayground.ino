#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char STR_TITLE[] = "DRAWING SHAPES";
const int RECT_STEP = 2;

int _rectSideLength = 2;
int _triangleUp = 1;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // We need to randomly seed our random number generator, which we can
  // do with noise off of A5 (assuming nothing is attached to it)
  //
  // If we don't do this, then the random values will always be the same
  // after reboot
  randomSeed(analogRead(A5));

  _display.clearDisplay();
}

void loop() {
  // On each loop, we'll want to clear the display so we're not writing over
  // previously drawn data
  _display.clearDisplay();

  int numCircles = random(2, 6);
  drawRandomCircles(numCircles);

  drawGrowingRectangle();
  
  drawGrowingRoundedRectangles();

  drawFlippingTriangle();
  
  drawTitleText();

  // Render the graphics buffer to screen
  _display.display();

  delay(400);
}

/**
 * Draws the given number of circles with random radius
 * and random x,y location in the top left corner of the OLED
 * 
 * See: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#circles-2002788-14
 */
void drawRandomCircles(int numCircles){
  const int MIN_RADIUS = 2;
  const int MAX_RADIUS = 10;
  int x, y, radius;
  
  for(int i = 0; i < numCircles; i++){
    radius = random(MIN_RADIUS, MAX_RADIUS);
    x = random(radius/2, _display.width()/2 - radius/2);
    y = random(radius/2, _display.height()/2 - radius/2);
    
    // Returns a random number from 0 to 9, inclusive
    int randomFill = random(0, 10);

    if(randomFill < 5){
      _display.drawCircle(x, y, radius, SSD1306_WHITE);
    }else{
      _display.fillCircle(x, y, radius, SSD1306_WHITE);
    }
  }
}

/**
 * Draws one rectangle in the bottom left corner of the OLED
 * that grows in size until it reaches maximum size then jumps to the smallest size
 * and grows again.
 * 
 * See: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#rectangles-2002784-10
 */
void drawGrowingRectangle(){
  const int MIN_SIDE_LENGTH = 2;
  const int MAX_SIDE_LENGTH = 32;
  
  int x, y, width, height;
  
  x = _display.width() / 4 - (_rectSideLength / 2);
  y = _display.height() * 3 / 4 - (_rectSideLength / 2);
  width = _rectSideLength;
  height = _rectSideLength;
  _display.drawRect(x, y, width, height, SSD1306_WHITE);

  if (_rectSideLength < MAX_SIDE_LENGTH) {
    _rectSideLength += RECT_STEP;
  } else {
    _rectSideLength = MIN_SIDE_LENGTH;
  }
}

/**
 * Draws a filled triangle in the top right corner that flips between down and up
 * 
 * See: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#rectangles-2002784-10
 */
void drawFlippingTriangle(){
  const int MAX_SIZE = 30;
  int x0, y0, x1, y1, x2, y2, centerX, centerY;
  
  centerX = _display.width() * 3 / 4;
  centerY = _display.height() / 4;
  x0 = centerX;
  y0 = centerY + (10 * _triangleUp);

  x1 = centerX - 10;
  y1 = centerY - (10 * _triangleUp);

  x2 = centerX + 10;
  y2 = centerY - (10 * _triangleUp);
  
  _display.fillTriangle(x0, y0, x1, y1, x2, y2, SSD1306_WHITE);
    
  _triangleUp *= -1;
}

/**
 * Draws two rounded rectangles in the bottom right corner that both
 * grow (one in width the other in height) until they reach maximum
 * size, then the size jumps to smallest size and they grow again.
 * 
 * See: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#rounded-rectangles-2002792-18
 */
void drawGrowingRoundedRectangles(){
  const int NUM_RECTS = 2;
  
  int x, y, width, height, centerX, centerY;
  
  for(int i = 0; i < NUM_RECTS; i++){
    centerX = _display.width() * 3 / 4;
    centerY = _display.height() * 3 / 4;
    width = _rectSideLength;
    height = 10;
    if (i % 2 == 1) {
      int temp = width;
      width = height;
      height = temp;
    }
    x = centerX - width/2;
    y = centerY - height/2;

    int cornerRadius = min(width, height) / 4;

    _display.drawRoundRect(x, y, width, height, cornerRadius, SSD1306_WHITE);
  }
}

/**
 * Draws the text centered on the screen
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
