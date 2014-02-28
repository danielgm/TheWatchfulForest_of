#include "Camera.h"

Camera::Camera(int _id, ServoCommand &_servoCommand) {
  id = _id;
  servoCommand = &_servoCommand;

  laserPin = 0;

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
  allowLaser = false;
  isPaused = false;

  nullSignalDelay = 300;
  nullSignalTime = 0;

  position.set(0, 0, 0);
  direction.set(0, 0, 0);

  shuttingDown = false;
}

int Camera::getId() {
  return id;
}

int Camera::getLaserPin() {
  return laserPin;
}

void Camera::setLaserPin(int v) {
  laserPin = v;
}

void Camera::update() {
  if (isAnimating()) {
    servoCommand->setServo(id * 2 + 0, ofMap(getPan(), 0, 1, panMin, panMax));
    servoCommand->setServo(id * 2 + 1, ofMap(getTilt(), 0, 1, tiltMin, tiltMax));
  }
  else if (nullSignalTime != 0 && ofGetSystemTime() > nullSignalTime) {
    sendNullSignal();
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

float Camera::getPan() {
  if (isAnimating()) {
    return easeInOut(
          ofGetSystemTime() - animationStart,
          panStart,
          panTarget - panStart,
          animationDuration);
  }
  else {
    return panTarget;
  }
}

void Camera::setPan(float pan) {
  setPanAndTilt(pan, getTilt());
}

float Camera::getTilt() {
  if (isAnimating()) {
    return easeInOut(
          ofGetSystemTime() - animationStart,
          tiltStart,
          tiltTarget - tiltStart,
          animationDuration);
  }
  else {
    return tiltTarget;
  }
}

void Camera::setTilt(float tilt) {
  setPanAndTilt(getPan(), tilt);
}

void Camera::setPanAndTilt(float pan, float tilt) {
  cout << "Camera(" << id << ")" << "::setPanAndTilt(" << pan << ", " << tilt << ")" << endl;
  panStart = panTarget = pan;
  tiltStart = tiltTarget = tilt;

  animationStart = 0;
  animationDuration = 0;
  oneLastFrame = false;

  nullSignalTime = ofGetSystemTime() + nullSignalDelay;

  servoCommand->setServo(id * 2 + 0, ofMap(getPan(), 0, 1, panMin, panMax));
  servoCommand->setServo(id * 2 + 1, ofMap(getTilt(), 0, 1, tiltMin, tiltMax));
}

void Camera::panAndTiltTo(float pan, float tilt) {
  panAndTiltTo(pan, tilt, calculateDuration(getPan(), getTilt(), pan, tilt));
}

void Camera::setPanAndTiltHome() {
  setPanAndTilt(0.5, 0.5);
}

void Camera::panAndTiltTo(float pan, float tilt, int duration) {
  cout << "Camera(" << id << ")"
    << "::panAndTiltTo(" << pan << ", " << tilt << ", " << duration << ")"
    << ", current=" << getPan() << ", " << getTilt()
    << endl;
  panStart = getPan();
  tiltStart = getTilt();

  panTarget = pan;
  tiltTarget = tilt;

  animationStart = ofGetSystemTime();
  animationDuration = duration;
  oneLastFrame = true;

  nullSignalTime = animationStart + animationDuration + nullSignalDelay;
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

  if (laserPin >= 0) {
    servoCommand->setDigital(laserPin, on);
  }
}

bool Camera::getAllowLaser() {
  return allowLaser;
}

void Camera::setAllowLaser(bool v) {
  allowLaser = v;
}

bool Camera::getPaused() {
  return isPaused;
}

void Camera::setPaused(bool v) {
  isPaused = v;

  if (v) {
    panStart = panTarget = getPan();
    tiltStart = tiltTarget = getTilt();

    animationStart = 0;
    animationDuration = 0;
    oneLastFrame = false;
  }
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

void Camera::readSettings(ofxXmlSettings &settings) {
  id = settings.getValue("id", 0);
  laserPin = settings.getValue("laserPin", 0);

  panMin = settings.getValue("panMin", 0);
  panMax = settings.getValue("panMax", 0);
  tiltMin = settings.getValue("tiltMin", 0);
  tiltMax = settings.getValue("tiltMax", 0);
  position = ofVec3f(
    settings.getValue("position:x", 0.0),
    settings.getValue("position:y", 0.0),
    settings.getValue("position:z", 0.0));
  direction = ofVec3f(
  settings.getValue("direction:x", 0.0),
  settings.getValue("direction:y", 0.0),
  settings.getValue("direction:z", 0.0));

  cout << "Loaded camera " << id
    << ": pan=[" << panMin << "," << panMax
    << "], tilt=[" << tiltMin << "," << tiltMax << "]"
    << endl;
}

void Camera::pushSettings(ofxXmlSettings &settings) {
  settings.setValue("id", id);
  settings.setValue("laserPin", laserPin);

  settings.setValue("tiltMin", (int) tiltMin);
  settings.setValue("tiltMax", (int) tiltMax);
  settings.setValue("panMin", (int) panMin);
  settings.setValue("panMax", (int) panMax);

  settings.setValue("position:x", position[0]);
  settings.setValue("position:y", position[1]);
  settings.setValue("position:z", position[2]);
  settings.setValue("direction:x", direction[0]);
  settings.setValue("direction:y", direction[1]);
  settings.setValue("direction:z", direction[2]);
}

void Camera::sendNullSignal() {
  servoCommand->setServo(id * 2 + 0, 0);
  servoCommand->setServo(id * 2 + 1, 0);
  nullSignalTime = 0;
}

bool Camera::isShuttingDown() {
  return shuttingDown;
}

void Camera::shutdown() {
  shuttingDown = true;
  panAndTiltTo(0.5, 0.5);
}

int Camera::calculateDuration(float p0, float t0, float p1, float t1) {
    float dPan = p1 - p0;
    float dTilt = t1 - t0;
    float delta = sqrt(dPan * dPan + dTilt * dTilt);

    float rDelta = sqrt(2);

    return floor(ofMap(delta / rDelta, 0, 1, 2000, 4000));
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

