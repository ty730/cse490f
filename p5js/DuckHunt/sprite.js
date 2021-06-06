
// This class is a modification of:
// https://editor.p5js.org/codingtrain/sketches/vhnFx1mml
// This class creates a sprite for either a duck or a dog that is animated.
class Sprite {
  constructor(animation, x, y, speedX, speedY, animSpeed, isDog) {
    this.x = x;
    this.y = y;
    this.animation = animation;
    this.w = this.animation.right[0].width;
    this.h = this.animation.right[0].height;
    this.len = this.animation.right.length;
    this.animSpeed = animSpeed;
    this.speedX = speedX * 15;
    this.speedY = speedY * 15;
    this.index = 0;
    this.shot = false;
    this.shotTimer = 20;
    this.falling = false;
    this.isDog = isDog;
    this.goOffScreen = false;
  }

  show() {
    let index;
    let img;
    if (this.falling) {
      img = this.animation.shot[1];
    } else if (this.shot) {
      img = this.animation.shot[0];
    } else if (this.speedX > 0) {
      index = floor(this.index) % this.animation.right.length;
      img = this.animation.right[index];
    } else {
      index = floor(this.index) % this.animation.left.length;
      img = this.animation.left[index];
    }
    image(img, this.x, this.y);
  }

  animate() {
    this.index += this.animSpeed;
    if (!this.shot) {
      this.x += this.speedX;
      this.y -= this.speedY;
    } else if (this.falling) {
      if (this.y <= height) {
        this.y += 4;
      }
    } else {
      if (this.shotTimer < 0) {
        this.falling = true;
      } else {
        this.shotTimer -= 1;
      }
    }
    if (!this.isDog && (this.x + this.w > width || this.x < 0) && !this.goOffScreen) {
      this.speedX = -this.speedX;
    } else if (this.x > width && this.isDog) {
      this.x = -this.w;
    }
    if ((this.y + this.h > height || this.y < 0) && !this.goOffScreen) {
      this.speedY = -this.speedY;
    }
  }

  getX() {
    return this.x;
  }

  getY() {
    return this.y;
  }

  getWidth() {
    return this.w;
  }

  getHeight() {
    return this.h;
  }

  shoot() {
    this.shot = true;
  }

  releaseDuck() {
    this.shot = false;
    this.falling = false;
    this.shotTimer = 20;
    this.goOffScreen = false;
    this.x = random(this.w, width - this.w);
    this.y = height - this.h;
    this.speedX = (abs(this.speedX) + 0.3) * random([1, -1]);
    this.speedY = (abs(this.speedY) + random(-0.3, 0.3)) * random([1, -1]);
  }

  flyAway() {
    this.goOffScreen = true;
  }

  collision(x, y) {
    return x >= this.x && x <= this.x + this.w && y >= this.y && y <= this.y + this.h;
  }
}