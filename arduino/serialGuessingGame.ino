#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int DELAY_MS = 5;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

String secretWord = "arduino";
boolean guessed = false;
String oldText = "";

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

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
    
    _display.clearDisplay();
    int16_t x1, y1;
    uint16_t textWidth, textHeight;

    _display.setTextSize(2); 

    String secretPattern = "a_d_i_o";
    String wrong = "WRONG!";

    if (rcvdSerialData.equals(secretWord)) {
      // Measure one line of text     
      _display.getTextBounds(secretWord, 0, 0, &x1, &y1, &textWidth, &textHeight);
      uint16_t yText = _display.height() / 2 - textHeight / 2;
      uint16_t xText = _display.width() / 2 - textWidth / 2; 
      _display.setTextSize(1); 
      _display.setCursor(0, 0);
      _display.print("Yay! the word was:");
      _display.setTextSize(2); 
      _display.setCursor(xText, yText);
      _display.print(secretWord);
      _display.display();
      guessed = true;
    } else if (!guessed && !rcvdSerialData.equals(oldText)) {
      // Measure one line of text     
      _display.getTextBounds(wrong, 0, 0, &x1, &y1, &textWidth, &textHeight);
      uint16_t yText = _display.height() / 2 - textHeight / 2;
      uint16_t xText = _display.width() / 2 - textWidth / 2; 
      _display.setCursor(xText, yText);
      _display.print(wrong);
      _display.display();
    }
    oldText = rcvdSerialData;
    delay(500);
    if (!guessed) {
      _display.clearDisplay();
      // Measure one line of text     
      _display.getTextBounds(secretPattern, 0, 0, &x1, &y1, &textWidth, &textHeight);
      uint16_t yText = _display.height() / 2 - textHeight / 2;
      uint16_t xText = _display.width() / 2 - textWidth / 2; 
      _display.setCursor(xText, yText);
      _display.print(secretPattern);
      _display.display();
    }
    
    // Echo the data back on serial (for debugging purposes)
    // This is not necessary but helpful. Then the webpage can
    // display this debug output (if necessary)
    Serial.print("Arduino received: '");
    Serial.print(rcvdSerialData);
    Serial.println("'");
  }

  delay(DELAY_MS);
}
