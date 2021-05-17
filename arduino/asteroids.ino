#include <Wire.h>
#include <SPI.h> 
#include <Shape.hpp>; // from library at https://github.com/makeabilitylab/arduino
#include <ParallaxJoystick.hpp>; // from library at https://github.com/makeabilitylab/arduino
#include <Tone32.hpp> // from library at https://github.com/makeabilitylab/arduino
#include <PushButton.h> // from library at https://github.com/kristianklein/PushButton
#include <EEPROM.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int DELAY_GAME = 40; // delay interval for how often to update game
const int JOYSTICK_UPDOWN_PIN = A7;
const int JOYSTICK_LEFTRIGHT_PIN = A6;
const char STR_TITLE[] = "ASTEROIDS";
const char STR_GAME_OVER[] = "GAME OVER";
const char STR_INSTRUCTIONS[] = "click to play";

const int SHOOT_BUTTON_INPUT_PIN = 21;
const int NUM_BULLETS = 4;
const int NUM_ASTEROIDS = 4;

const int TONE_OUTPUT_PIN = 26; // output pin for piezo buzzer
// The ESP32 has 16 channels which can generate 16 independent waveforms
// We'll just choose PWM channel 1 here
const int TONE_PWM_CHANNEL = 1; 
// Create our Tone32 object
Tone32 _tone32(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);

const int VIBROMOTOR_OUTPUT_PIN = 25;
const int VIBRO_DURATION = 500;
int _vibroStart = -500;

// LED output pins to indicate number of player lives
const int LED1_OUTPUT_PIN = 15;
const int LED2_OUTPUT_PIN = 33;
const int LED3_OUTPUT_PIN = 27;

// Dynamic arrays for circular bullets. We'll create and initialize this in setup()
Circle **_bulletArray;
float *_bulletDirections;
boolean *_isShooting;
int _bulletIndex = 0;

// Dynamic array for asteroids. We'll create and initialize this in setup()
Ball **_asteroidArray;

const int MAX_ANALOG_VAL = 1023;
const enum JoystickYDirection JOYSTICK_Y_DIR = RIGHT;

// Analog joystick
ParallaxJoystick _analogJoystick(JOYSTICK_UPDOWN_PIN, JOYSTICK_LEFTRIGHT_PIN, MAX_ANALOG_VAL, JOYSTICK_Y_DIR);

// initialize button to use debounced button library
// found at https://github.com/kristianklein/PushButton
PushButton shootBtn(SHOOT_BUTTON_INPUT_PIN);

// Triangle variables
int _xTriangle = 0; // x coordinate for top of triangle
int _yTriangle = 0; // y coordinate for top of triangle
float _rotation = 0.0;

// game information
int _lives = 3;
int _score = 0;
int _mode = 0;

unsigned long _prevTime = 0; // the last time the game was updated

void setup() {
  Serial.begin(9600); // for printing to the console

  EEPROM.begin(64); // initialize EEPROM with size 64 for holding high scores

  pinMode(SHOOT_BUTTON_INPUT_PIN, INPUT_PULLUP);
  shootBtn.setActiveLogic(LOW);

  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
  pinMode(LED1_OUTPUT_PIN, OUTPUT);
  pinMode(LED2_OUTPUT_PIN, OUTPUT);
  pinMode(LED3_OUTPUT_PIN, OUTPUT);

  // if analog input pin 5 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(A5));

  // In order for polymorphism to work, we need to use pointers
  // See: https://stackoverflow.com/a/45176572
  // So, we dynamically create our bullets here
  _bulletArray = new Circle*[NUM_BULLETS];
  _bulletDirections = new float[NUM_BULLETS];
  _isShooting = new bool[NUM_BULLETS];
  for (int i = 0; i < NUM_BULLETS; i++) {
    _bulletArray[i] = new Circle(0, 0, 1);
    _isShooting[i] = false;
  }
  // similarly to the bullets, create the asteroids.
  // for each asteroid give is a random speed, random size, and put them in the corners.
  _asteroidArray = new Ball*[NUM_ASTEROIDS];
  for (int i = 0; i < NUM_ASTEROIDS; i++) {
    int cornerX = (_display.width() - 31) * (i % 2) + 15;
    int cornerY = (_display.height() - 31) * ((i / 2) % 2) + 15;
    int randSize = 4 * random(1, 3);
    _asteroidArray[i] = new Ball(cornerX, cornerY, randSize);
    double randXSpeed = 0.5 * random(2, 3);
    double randYSpeed = 0.5 * random(2, 3);
    _asteroidArray[i]->setSpeed(randXSpeed, randYSpeed);
  }
  
  initializeOledAndShowStartupScreen();

  // Initialize Triangle to center of screen
  _xTriangle = _display.width() / 2;
  _yTriangle = _display.height() / 2;
}

void loop() {
  unsigned long currentTime = millis(); // get current time to check if we have to update game
  // Set the LEDs based on number of lives
  int led1 = LOW;
  int led2 = LOW;
  int led3 = LOW;
  if (_lives > 0) {
    led1 = HIGH;
  }
  if (_lives > 1) {
    led2 = HIGH;
  }
  if (_lives > 2) {
    led3 = HIGH;
  }
  digitalWrite(LED1_OUTPUT_PIN, led1); 
  digitalWrite(LED2_OUTPUT_PIN, led2); 
  digitalWrite(LED3_OUTPUT_PIN, led3);
  
  if (currentTime - _vibroStart >= VIBRO_DURATION) { // turn of the vibromotor after the interval
    digitalWrite(VIBROMOTOR_OUTPUT_PIN, LOW);
  } else {                                           // otherwise have the vibromotor on
    digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
  }
  
  if (_mode == 0 || _mode == 2) {
    // if we are in start game (mode 0) or game over (mode 2) state then just display text
    _display.clearDisplay();
    drawCenteredText(); // display the text for the current mode we are in
    _display.display();
    // call update() to turn off the sound automatically after the play duration expires
    _tone32.update();
    // if the shoot button is clicked then switch to playing the game
    shootBtn.update();
    if (shootBtn.isClicked()) {
      // if it's the game over state then reset the game
      // statistics and move asteroids away from the player
      if (_mode == 2) {
        _lives = 3;
        _score = 0;
        for (int i = 0; i < NUM_ASTEROIDS; i++) {
          createNewAsteroid(_asteroidArray[i]);
        }
      }
      _mode = 1; // change to playing the game (mode 1)
    }
  } else if (currentTime - _prevTime >= DELAY_GAME) {
    // only every DELAY_GAME ms we update the game, this makes the player and asteroids not too fast
    _prevTime = currentTime;
    _display.clearDisplay();
  
    drawLivesText(); // display the current amount of lives the player has
    drawScoreText(); // display the current score of the player

    // loop through all asteroids
    for (int j = 0; j < NUM_ASTEROIDS; j++) {
      if (_asteroidArray[j]->checkXBounce(0, _display.width() - 1)) {
        _asteroidArray[j]->reverseXSpeed(); // if asteroid hits vertical walls then bounce
      }
      if (_asteroidArray[j]->checkYBounce(0, _display.height() - 1)) {
        _asteroidArray[j]->reverseYSpeed(); // if asteroid hits horizontal walls then bounce
      }
      // loop through all bullets to check for collisions
      for (int k = 0; k < NUM_BULLETS; k++) {
        Circle const& constBullet = *_bulletArray[k];
        if (_asteroidArray[j]->overlaps(constBullet)) { // collision between bullet and asteroid
          _tone32.playNote(NOTE_D, 3, 280); // play collision sound effect
          _score += 1; // increase the score
          // rotate the asteroid by 90 degrees by doing setSpeed(y, -x)
          _asteroidArray[j]->reverseXSpeed();
          _asteroidArray[j]->setSpeed(_asteroidArray[j]->getYSpeed(), _asteroidArray[j]->getXSpeed());
          // decrease the radius
          int newRadius = _asteroidArray[j]->getRadius() / 2 + 1;
          if (newRadius < 4) { // radius is below minimum so create new asteroid
            createNewAsteroid(_asteroidArray[j]);
          } else { // radius is good size so set it
            _asteroidArray[j]->setRadius(newRadius);
          }
          _isShooting[k] = false; // make the bullet stop shooting
          _bulletArray[k]->setCenter(-10, -10);
        }
      }
      // get the dimensions of the triangle space ship and asteroid
      int xTriCenter = _xTriangle - (int)(10.0 * cos(_rotation));
      int yTriCenter = _yTriangle - (int)(10.0 * sin(_rotation));
      int xTriLeft = xTriCenter + (int)(4.0 * cos(_rotation + PI/2.0));
      int yTriLeft = yTriCenter + (int)(4.0 * sin(_rotation + PI/2.0));
      int xTriRight = xTriCenter + (int)(4.0 * cos(_rotation - PI/2.0));
      int yTriRight = yTriCenter + (int)(4.0 * sin(_rotation - PI/2.0));
      int astX = _asteroidArray[j]->getCenterX();
      int astY = _asteroidArray[j]->getCenterY();
      int astRadius = _asteroidArray[j]->getRadius();
      // check for collision
      if (collision(_xTriangle, _yTriangle, xTriLeft, yTriLeft, astX, astY, astRadius) ||
          collision(_xTriangle, _yTriangle, xTriRight, yTriRight, astX, astY, astRadius) ||
          collision(xTriLeft, yTriLeft, xTriRight, yTriRight, astX, astY, astRadius)) {
        createNewAsteroid(_asteroidArray[j]); // create new asteroid so same collision doesn't register again
        _lives -= 1; // decrease player lives
        _tone32.playNote(NOTE_D, 1, 280); // play collision sound effect
        _vibroStart = currentTime; // start vibration for player getting hit by asteroid
      }
      _asteroidArray[j]->update(); // move the asteroid by its speed
      _asteroidArray[j]->setDrawFill(true);
      _asteroidArray[j]->draw(_display);
    }
    // call update() to turn off the sound automatically after the play duration expires
    _tone32.update();
    if (_lives <= 0) { // change the mode to game over if out of lives
      int highscore = 0;
      EEPROM.get(0, highscore); // get the current high score stored at address 0
      if (_score > highscore) { // new high score!
        EEPROM.put(0, _score); // store the new high score at address 0
        EEPROM.commit(); // commit our change
      }
      _mode = 2;
    }
    shootBtn.update();
    if (shootBtn.isClicked()) { // make bullet shoot if shoot button is clicked
      //_tone32.playNote(NOTE_A, 5, 80); // play shooting sound effect
      
      _bulletArray[_bulletIndex]->setCenter(_xTriangle, _yTriangle);
      _bulletDirections[_bulletIndex] = _rotation;
      _isShooting[_bulletIndex] = true;
      if (_bulletIndex == NUM_BULLETS - 1) {
        _bulletIndex = 0;
      } else {
        _bulletIndex += 1;
      }
    }
    // loop through all bullets and move them and display them if they are shooting
    for (int i = 0; i < NUM_BULLETS; i++) { 
      if (_isShooting[i]) {                  
        int xBull = _bulletArray[i]->getCenterX() + 3.0*cos(_bulletDirections[i]);
        int yBull = _bulletArray[i]->getCenterY() + 3.0*sin(_bulletDirections[i]);
        _bulletArray[i]->setCenter(xBull, yBull);
        if (xBull < 0 || xBull > _display.width() ||
            yBull < 0 || yBull > _display.height()) { // bullet hit the wall of the screen so stop shooting
          _isShooting[i] = false;
        } else { // we are good to shoot
          _bulletArray[i]->setDrawFill(true);
          _bulletArray[i]->draw(_display);
        }
      }
    }
    
    // read analog joystick
    _analogJoystick.read();
    // get the left-right value which determines the rotation (left = clockwise, right = counterclockwise)
    int leftRightVal = _analogJoystick.getLeftRightVal();
    float rotationStep = mapfloat(leftRightVal, -2800, 1023, -0.25, 0.25);
    if (rotationStep < -0.01 || rotationStep > 0.01) {
      _rotation += rotationStep;
    }
    // get the up-down value which makes the player move forwards or backwards in the current direction
    int upDownVal = _analogJoystick.getUpDownVal();
    float forwardStep = mapfloat(upDownVal, 0, 3860, -7.0, 7.0);
    int newX = _xTriangle + (int)(forwardStep * cos(_rotation));
    int newY = _yTriangle + (int)(forwardStep * sin(_rotation));
    if (newX > 0 && newX < _display.width() && newY > 0 && newY < _display.height()) { // don't have front of player beyond screen
      _xTriangle = newX;
      _yTriangle = newY;
    }
    // get the left and right back points of the triangle that is the player
    int xTriCenter = _xTriangle - (int)(10.0 * cos(_rotation));
    int yTriCenter = _yTriangle - (int)(10.0 * sin(_rotation));
    int xTriLeft = xTriCenter + (int)(4.0 * cos(_rotation + PI/2.0));
    int yTriLeft = yTriCenter + (int)(4.0 * sin(_rotation + PI/2.0));
    int xTriRight = xTriCenter + (int)(4.0 * cos(_rotation - PI/2.0));
    int yTriRight = yTriCenter + (int)(4.0 * sin(_rotation - PI/2.0));
    // draw the player triangle
    _display.drawTriangle(_xTriangle, _yTriangle, xTriLeft, yTriLeft, xTriRight, yTriRight, SSD1306_WHITE);
    // render buffer to screen
    _display.display();
  }
}

/**
 * Call this from setup() to initialize the OLED screen
 */
void initializeOledAndShowStartupScreen(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  _display.clearDisplay();

  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);
  _display.setCursor(0, 0);
  _display.println("Screen initialized!");
  _display.display();
  delay(500);
  _display.clearDisplay();
}

/**
 * Maps a value from one range to another using all floating point numbers
 */
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Returns true if there is a collision between the given line (x1,y1)->(x2,y2)
 * and the given circle (centerX,centerY,radius). Otherwise returns false.
 * 
 * Collision math found at: http://www.jeffreythompson.org/collision-detection/poly-circle.php
 * 
 */
boolean collision(int x1, int y1, int x2, int y2, int centerX, int centerY, int radius) {
  float len = distance(x1, y1, x2, y2); // get length of line
  // get dot product of the line and circle
  double dot = ( ((centerX-x1)*(x2-x1)) + ((centerY-y1)*(y2-y1)) ) / pow(len,2);
  // find the closest point on the line
  double closestX = x1 + (dot * (x2-x1));
  double closestY = y1 + (dot * (y2-y1));
  // is this point actually on the line segment?
  // if so keep going, but if not, return false
  boolean onSegment = linePoint(x1,y1,x2,y2, closestX,closestY);
  if (!onSegment) {
    return false;
  }
  float dist = distance(closestX, closestY, centerX, centerY);
  if (dist <= radius) {
    return true;
  } else {
    return false;
  }
}

/**
 * Calculates the Euclidean distance between two points
 * 
 */
float distance(int x1, int y1, int x2, int y2){
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

/**
 * Returns true if the given line (x1,y1)->(x2,y2) is
 * intersecting the given point (px,py). Otherwise returns false.
 * 
 * Collision math found at: http://www.jeffreythompson.org/collision-detection/line-point.php
 * 
 */
boolean linePoint(int x1, int y1, int x2, int y2, int px, int py) {
  // get distance from the point to the two ends of the line
  float d1 = distance(px,py, x1,y1);
  float d2 = distance(px,py, x2,y2);

  // get the length of the line
  float lineLen = distance(x1,y1, x2,y2);

  // since floats are so minutely accurate, add
  // a little buffer zone that will give collision
  float buffer = 0.1;    // higher # = less accurate

  // if the two distances are equal to the line's 
  // length, the point is on the line!
  // note we use the buffer here to give a range, 
  // rather than one #
  if (d1+d2 >= lineLen-buffer && d1+d2 <= lineLen+buffer) {
    return true;
  }
  return false;
}

/**
 * Takes the current asteroid and makes is the maximum asteroid size,
 * and sets it in one of the screens corners away from the player
 * (so it doesn't immediately collide with the player when being "created")
 * 
 */
void createNewAsteroid(Ball *asteroid) {
  int newAsteroidX = 15;
  if (_xTriangle < _display.width() / 2) { // if player is on left half of screen
    newAsteroidX = _display.width() - 16; // make asteroid along right border of screen
  }
  int newAsteroidY = (_display.height() - 31) * random(0, 2) + 15; // randomly choose top or bottom corner
  asteroid->setCenter(newAsteroidX, newAsteroidY);
  asteroid->setRadius(8);
}

/**
 * Draws the lives text in the upper left hand corner
 * 
 * See: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#characters-and-text-2002798-24
 */
void drawLivesText() {
  // Setup text rendering parameters
  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);

  // Print out the string
  String fullText = "lives:" + String(_lives);
  char livesCharArr[fullText.length() + 1];
  strcpy(livesCharArr, fullText.c_str());
  
  // Put the text in the top left corner
  _display.setCursor(0, 0);

  // Print out the string
  _display.print(livesCharArr);
}

/**
 * Draws the text for the score in the upper right hand corner
 * 
 * See: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#characters-and-text-2002798-24
 */
void drawScoreText() {
  int16_t x, y;
  uint16_t textWidth, textHeight;

  // Setup text rendering parameters
  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);

  String fullScore = "score:" + String(_score);
  char scoreCharArr[fullScore.length() + 1];
  strcpy(scoreCharArr, fullScore.c_str());

  // Measure the text with those parameters
  _display.getTextBounds(scoreCharArr, 0, 0, &x, &y, &textWidth, &textHeight);

  // Put the text in the top right corner
  _display.setCursor(_display.width() - textWidth, 0);

  // Print out the string
  _display.print(scoreCharArr);
}

/**
 * Draws either the start game or game over screen text in the middle depending on what mode we are currently in
 * 
 * See: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#characters-and-text-2002798-24
 */
void drawCenteredText() {
  int16_t x, y;
  uint16_t textWidth, textHeight;
  int16_t instrX, instrY;
  uint16_t instrWidth, instrHeight;

  // Setup text rendering parameters
  _display.setTextSize(2);
  _display.setTextColor(WHITE, BLACK);

  if (_mode == 0) { // Start game mode
    _display.getTextBounds(STR_TITLE, 0, 0, &x, &y, &textWidth, &textHeight); // get dimensions of text
    // Make the text centered horizontally and a little above the center veritcally
    _display.setCursor(_display.width() / 2 - textWidth / 2, _display.height() / 2 - textHeight);
    _display.print(STR_TITLE); // Print out the title: ASTEROIDS
    _display.setTextSize(1); // reset the text size for the sub-heading
    _display.getTextBounds(STR_INSTRUCTIONS, 0, 0, &instrX, &instrY, &instrWidth, &instrHeight); // get dimensions
    // Center text horizontally and have it a little below the center veritcally
    _display.setCursor(_display.width() / 2 - instrWidth / 2, _display.height() / 2 + instrHeight / 2);
    // Print out the string for the instructions
    _display.print(STR_INSTRUCTIONS);
  } else { // Game over game mode
    // make instructions say: "click to play again"
    char instructions[sizeof(STR_INSTRUCTIONS) + 10];
    strcpy(instructions, STR_INSTRUCTIONS);
    strcat(instructions, " again");
    // get the current score as a string to display on the screen
    String playerScore = String("score:") + String(_score);
    char pScore[playerScore.length()+1];
    strcpy(pScore, playerScore.c_str());
    // get the high score as a string to display on the screen
    int savedScore = 0;
    EEPROM.get(0, savedScore); // get the current high score saved at address 0 on EEPROM
    String highScore = String("highscore:") + String(savedScore);
    char hScore[highScore.length()+1];
    strcpy(hScore, highScore.c_str());
    
    _display.getTextBounds(STR_GAME_OVER, 0, 0, &x, &y, &textWidth, &textHeight); // get dimensions
    // Make the text centered horizontally and a little above the center veritcally
    _display.setCursor(_display.width() / 2 - textWidth / 2, _display.height() / 2 - textHeight);
    _display.print(STR_GAME_OVER); // Print out the title: GAME OVER
    _display.setTextSize(1); // change text size for sub-heading
    _display.getTextBounds(instructions, 0, 0, &instrX, &instrY, &instrWidth, &instrHeight);
    // Center text horizontally and have it a little below the center veritcally
    _display.setCursor(_display.width() / 2 - instrWidth / 2, _display.height() / 2 + instrHeight / 2);
    // Print out the instructions string
    _display.print(instructions);
    
    _display.setCursor(0, _display.height() - instrHeight); // bottom left
    _display.print(pScore); // display players score
    _display.getTextBounds(hScore, 0, 0, &instrX, &instrY, &instrWidth, &instrHeight); // get bounds for highscore
    _display.setCursor(_display.width() - instrWidth, _display.height() - instrHeight); // bottom right
    _display.print(hScore); // display high score
  }
}
