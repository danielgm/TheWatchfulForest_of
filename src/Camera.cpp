#include "camera.h"

Camera::Camera() {
  id = -1;
  panMin = 0;
  panMax = 0;
  tiltMin = 0;
  tiltMax = 0;

  panStart = 0;
  panTarget = 0;

  tiltStart = 0;
  tiltTarget = 0;

  animationStart = 0;
  animationDuration = 0;
  oneLastFrame = false;

  isLaserOn = false;

  position.set(0, 0, 0);
  direction.set(0, 0, 0);
}

int Camera::getId() {
  return id;
}

void Camera::setup(int _id, ServoCommand &_servoCommand) {
  id = _id;
  servoCommand = &_servoCommand;
}

void Camera::update() {
  if (isAnimating()) {
    cout << ofGetSystemTime() - animationStart << " " << panStart << " " << panTarget << " " << getPan() << endl;
    servoCommand->setServo(0, getPan());
    servoCommand->setServo(1, getTilt());
  }
}

void Camera::setPanExtent(int min, int max) {
  panMin = min;
  panMax = max;
}

void Camera::setTiltExtent(int min, int max) {
  tiltMin = min;
  tiltMax = max;
}

int Camera::getPan() {
  if (isAnimating()) {
    return floor(easeInOut(
          ofGetSystemTime() - animationStart,
          panStart,
          panTarget - panStart,
          animationDuration));
  }
  else {
    return floor(panTarget);
  }
}

int Camera::getTilt() {
  if (isAnimating()) {
    return floor(easeInOut(
          ofGetSystemTime() - animationStart,
          tiltStart,
          tiltTarget - tiltStart,
          animationDuration));
  }
  else {
    return floor(tiltTarget);
  }
}

void Camera::setPanAndTilt(int pan, int tilt) {
  panStart = panTarget = pan;
  tiltStart = tiltTarget = tilt;

  animationStart = 0;
  animationDuration = 0;
  oneLastFrame = true;
}

void Camera::panAndTiltTo(int pan, int tilt) {
  panAndTiltTo(pan, tilt, calculateDuration(getPan(), getTilt(), pan, tilt));
}

void Camera::setPanAndTiltHome() {
  setPanAndTilt(panMin + (panMax - panMin) / 2, tiltMin + (tiltMax - tiltMin) / 2);
}

void Camera::panAndTiltTo(int pan, int tilt, int duration) {
  cout << "panAndTiltTo(" << pan << ", " << tilt << ", " << duration << ")" << endl;
  panStart = getPan();
  tiltStart = getTilt();

  panTarget = pan;
  tiltTarget = tilt;

  animationStart = ofGetSystemTime();
  animationDuration = duration;
  oneLastFrame = true;

  cout << "panStart = " << panStart << ", panTarget = " << panTarget << endl;
  cout << "tiltStart = " << tiltStart << ", tiltTarget = " << tiltTarget << endl;
}

bool Camera::isAnimating() {
  if (ofGetSystemTime() < animationStart + animationDuration) {
    return true;
  }
  if (oneLastFrame) {
    oneLastFrame = false;
    return true;
  }
  return false;
}

bool Camera::getLaser() {
  return isLaserOn;
}

void Camera::setLaser(bool on) {
  isLaserOn = on;
}

ofVec3f Camera::getPosition() {
  return position;
}
void Camera::setPosition(ofVec3f v) {
  position = v;
}

ofVec3f Camera::getDirection() {
  return direction;
}
void Camera::setDirection(ofVec3f v) {
  direction = v;
}

int Camera::calculateDuration(float p0, float t0, float p1, float t1) {
    float dPan = p1 - p0;
    float dTilt = t1 - t0;
    float delta = sqrt(dPan * dPan + dTilt * dTilt);

    float rPan = panMax - panMin;
    float rTilt = tiltMax - tiltMin;
    float rDelta = sqrt(rPan * rPan + rTilt * rTilt);

    return floor(ofMap(delta / rDelta, 0, 1, 1000, 3500));
}

/**
 * Thanks, Penner! Quartic easing functions.
 * @see https://github.com/jesusgollonet/processing-penner-easing/blob/master/src/Quart.java
 *
 * t: current time, b: begInnIng value, c: change In value, d: duration
 */
float Camera::easeIn(float t, float b, float c, float d) {
  return c*(t/=d)*t*t*t + b;
}

float Camera::easeOut(float t, float b, float c, float d) {
  return -c * ((t=t/d-1)*t*t*t - 1) + b;
}

float Camera::easeInOut(float t, float b, float c, float d) {
  if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
  return -c/2 * ((t-=2)*t*t*t - 2) + b;
}

