#include "camera.h"

Camera::Camera() {
  id = -1;
  panMin = 0;
  panMax = 0;
  tiltMin = 0;
  tiltMax = 0;

  isLaserOn = false;

  position.set(0, 0, 0);
  direction.set(0, 0, 0);
}

Camera::Camera(int _id, int _panMin, int _panMax, int _tiltMin, int _tiltMax) {
  id = _id;
  panMin = _panMin;
  panMax = _panMax;
  tiltMin = _tiltMin;
  tiltMax = _tiltMax;

  isLaserOn = false;

  position.set(0, 0, 0);
  direction.set(0, 0, 0);
}

int Camera::getId() {
  return id;
}

int Camera::getPan() {
  if (isAnimating()) {
    return easeInOut(ofGetSystemTime() - animationStart, panStart, panTarget - panStart, animationDuration);
  }
  else {
    return floor(panTarget);
  }
}

int Camera::getTilt() {
  if (isAnimating()) {
    return easeInOut(ofGetSystemTime() - animationStart, tiltStart, tiltTarget - tiltStart, animationDuration);
  }
  else {
    return floor(tiltTarget);
  }
}

void Camera::setPanAndTilt(int pan, int tilt) {
  animationDuration = 0;
  animationStart = 0;

  panStart = panTarget = pan;
  tiltStart = tiltTarget = tilt;
}

void Camera::panAndTiltTo(int pan, int tilt) {
  panAndTiltTo(pan, tilt, calculateDuration(getPan(), getTilt(), pan, tilt));
}

void Camera::panAndTiltTo(int pan, int tilt, int duration) {
  animationDuration = duration;
  animationStart = ofGetSystemTime();

  panStart = getPan();
  tiltStart = getTilt();

  panTarget = pan;
  tiltTarget = tilt;
}

bool Camera::isAnimating() {
  return ofGetSystemTime() < animationStart + animationDuration;
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

