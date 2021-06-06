// Resource I used for all the animation:
// https://editor.p5js.org/codingtrain/sketches/vhnFx1mml

// ML5.js PoseNet variables
let video;
let poseNet;
let human = null;
let modelReady = false;

// Serial variables
let serial; // the Serial object
let serialOptions = { baudRate: 115200  };

// Duck animation variables
let spritesheetRight;
let spritesheetLeft;
let spritedataRight;
let spritedataLeft;
let animation = {};
let ducks = [];
let newDuck = false;
let bg;

// Dog animation variables
let dogAnimation = {};
let spritedataDog;
let dog;

// Overall game variables
let playGame = false;
let gameOver = false;
let targetColor = 0;
let timer = -1;
let score = 0;
let highscore = 0;
let bulletsLeft = 3;

// Preload function to get all JSON and images before setup
function preload() {
  spritedataRight = loadJSON('duckright.json');
  spritedataLeft = loadJSON('duckleft.json');
  spritedataShot = loadJSON('duckshot.json');
  spritedataDog = loadJSON('dog.json');
  // Sprite images found at: https://www.pikpng.com/pngvi/JbTRbR_duck-hunt-duck-png-clipart/
  spritesheetRight = loadImage('images/spritesheet.png');
  spritesheetLeft = loadImage('images/spritesheetflipped.png');
  bg = loadImage('images/duckhuntbg.png');
}

function setup() {
  createCanvas(640, 480);
  video = createCapture(VIDEO);
  //video.hide();

  // setup PoseNet. This can take a while, so we load it
  // asynchronously (when it's done, it will call modelReady)
  poseNet = ml5.poseNet(video, onModelReady); //call onModelReady when setup
  poseNet.on('pose', onPoseDetected); // call onPoseDetected when pose detected

  // Setup Web Serial using serial.js
  serial = new Serial();
  serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
  serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
  serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
  serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);

  // setup duck animation
  animation.right = getAnimationImages(spritedataRight, spritesheetRight);
  animation.left = getAnimationImages(spritedataLeft, spritesheetLeft);
  animation.shot = getAnimationImages(spritedataShot, spritesheetRight);
  // setup dog animation
  dogAnimation.right = getAnimationImages(spritedataDog, spritesheetRight);
  dogAnimation.left = [];
  dogAnimation.shot = [];
  // create dog sprite
  dog = new Sprite(dogAnimation, 1, 345, 0.2, 0.0, 0.2, true);
  // create duck sprite
  for (let i = 0; i < 1; i++) {
    ducks[i] = new Sprite(animation, 1, 300, 0.2, 0.2, 0.2, false);
  }
}

function onModelReady() {
  // Once the model is ready then we will show the start screen
  modelReady = true;
}

function onPoseDetected(poses) {
  // poses can contain an array of bodies (because PoseNet supports
  // recognition for *multiple* human bodies at the same time
  // however, for this demo, we are interested in only one human body
  // which is at poses[0]
  human = poses[0];
}

function draw() {
  //move image by the width of image to the left
  translate(video.width, 0);
  //then scale it by -1 in the x-axis
  //to flip the image
  scale(-1, 1);
  image(video, 0, 0); // draw the video to screen
  background(bg); // draw background image over the video

  // draw and animate the ducks
  if (playGame) {
    for (let duck of ducks) {
      duck.show();
      duck.animate();
    }
    // count down the timer if it has been set to a number larger than 0
    if (frameCount % 60 == 0 && timer > 0) {
      timer--;
    } else if (timer == 0) { // if the timer is zero then release a duck and reset bullets
      timer--;
      ducks[0].releaseDuck();
      bulletsLeft = 3;
      if (serial.isOpen) {
        serial.writeLine(bulletsLeft);
      }
    }
    // draw the score in the top left (flip it so text isn't backwards)
    push();
    scale(-1, 1);
    textSize(28);
    fill(0);
    text("score: " + score, -620, 40);
    pop();
  } else if (gameOver) { // the game is over (player ran out of bullets)
    // Animate both the duck and the dog (duck will fly off screen)
    for (let duck of ducks) {
      duck.show();
      duck.animate();
    }
    dog.show();
    dog.animate();
    // Display the game over text
    push();
    scale(-1, 1);
    textSize(28);
    fill(0);
    text("GAME OVER", -430, 100);
    textSize(24);
    text("You ran out of bullets", -460, 140);
    text("YOUR SCORE: " + score, -550, 455);
    text("HIGHSCORE: " + highscore, -300, 455);
    noFill();
    if (frameCount % 60 < 30) {
      text("Joystick down to play again!", -500, 200);
    }
    pop();
  } else if (modelReady) { // the PoseNet model is ready so show start screen
    dog.show();
    dog.animate();
    push();
    scale(-1, 1);
    textSize(28);
    fill(0);
    text("DUCK HUNT", -425, 100);
    noFill();
    if (frameCount % 60 < 30) {
      text("Joystick up to shoot", -460, 150);
      text("Joystick down to begin!", -480, 200);
    }
    pop();
  }

  //if a human has been detected, draw overlay shapes!
  if (human != null) {

    // Track the nose and draw the guns target there.
    // PoseNet supports 17 body parts in total, see:
    // https://github.com/tensorflow/tfjs-models/tree/master/posenet#keypoints
    drawTarget(human.pose.nose.x, human.pose.nose.y);
  }

  /**
   * Draws a nose at the given x,y position
   *
   * @param {number} x the x pos of the nose
   * @param {number} y the y pos of the nose
   */
  function drawTarget(x, y) {
    noFill();
    stroke(targetColor, 0, 0);
    if (targetColor === 255) {
      targetColor = 0;
    }
    let radius = 2;
    let offset = 4;
    circle(x, y, 1);
    circle(x, y, 10 * radius);
    line(x + radius + offset, y, x + 2 * radius + 2 * offset, y);
    line(x - radius - offset, y, x - 2 * radius - 2 * offset, y);
    line(x, y + radius + offset, x, y + 2 * radius + 2 *offset);
    line(x, y - radius - offset, x, y - 2 * radius - 2 * offset);
  }
}

/**
 * This is a helper function that takes in a JSON object that has the position
 * of animation images and a spritesheet, then creates an array of animation
 * images to be used by the Sprite class.
 * @param {JSON object} spritedata
 * @param {Image} spritesheet
 * @returns array of images
 */
function getAnimationImages(spritedata, spritesheet) {
  let animation = [];
  let frames = spritedata.frames;
  for (let i = 0; i < frames.length; i++) {
    let pos = frames[i].position;
    let img = spritesheet.get(pos.x, pos.y, pos.w, pos.h);
    animation.push(img);
  }
  return animation;
}


/**
 * Callback function by serial.js when there is an error on web serial
 *
 * @param {} eventSender
 */
 function onSerialErrorOccurred(eventSender, error) {
  console.log("onSerialErrorOccurred", error);
  //pHtmlMsg.html(error);
}

/**
 * Callback function by serial.js when web serial connection is opened
 *
 * @param {} eventSender
 */
function onSerialConnectionOpened(eventSender) {
  console.log("onSerialConnectionOpened");
  //pHtmlMsg.html("Serial connection opened successfully");
}

/**
 * Callback function by serial.js when web serial connection is closed
 *
 * @param {} eventSender
 */
function onSerialConnectionClosed(eventSender) {
  console.log("onSerialConnectionClosed");
  //pHtmlMsg.html("onSerialConnectionClosed");
}

/**
 * Callback function serial.js when new web serial data is received
 *
 * @param {*} eventSender
 * @param {String} newData new data received over serial
 */
function onSerialDataReceived(eventSender, newData) {
  console.log("onSerialDataReceived", newData);
  if (newData === "shoot") { // if data sent is shoot then we shoot
    // Only shoot when playing the game
    if (playGame && !gameOver) {
      // If we are colliding with a duck when we shoot then it's a hit!
      if (ducks[0].collision(human.pose.nose.x, human.pose.nose.y)) {
        ducks[0].shoot(); // trigger shoot animations
        timer = 3; // set timer to release another duck
        score++;
      } else {
        // decrease bullets when duck isn't shot so we don't get game over when a duck is shot.
        bulletsLeft--;
      }
      targetColor = 255; // make target red
      if (bulletsLeft <= 0) { // if we are out of bullets then it's game over
        timer = -1;
        gameOver = true;
        playGame = false;
        ducks[0].flyAway();
        if (score > highscore) {
          highscore = score;
        }
      }
    }
  } else if (newData === "start") { // if data sent is start then we start playing
    // If we are on the start screen then just start
    if (!playGame && !gameOver) {
      playGame = true;
      if (serial.isOpen) {
        serial.writeLine(bulletsLeft);
      }
    } else if (gameOver) { // if on gameover screen then release a duck and start
      score = 0;
      bulletsLeft = 3;
      ducks[0].releaseDuck();
      gameOver = false;
      playGame = true;
      if (serial.isOpen) {
        serial.writeLine(bulletsLeft);
      }
    }
  }
}

/**
 * Called automatically when the mouse button has been pressed and then released
 * According to the p5.js docs, this function is only guaranteed to be called when
 * the left mouse button is clicked.
 *
 * See: https://p5js.org/reference/#/p5/mouseClicked
 */
 function mouseClicked() {
  // if serial connection isn't open then open it
  if (!serial.isOpen()) {
    serial.connectAndOpen(null, serialOptions);
  }
}
