#include "RandomMovementCamera.h"

RandomMovementCamera::RandomMovementCamera(int id, ServoCommand &servoCommand)
    : Camera(id, servoCommand) {
  wakeTime = ofGetSystemTime();
  laserToggleTime = ofGetSystemTime();
  hasTarget = false;
}

void RandomMovementCamera::update() {
  if (!getPaused() && !isShuttingDown()) {
    if (hasTarget && !isAnimating()) {
      sleep(1000 + floor(ofRandom(4000)));
      hasTarget = false;
    }
    else if (!hasTarget && !isSleeping()) {
      panAndTiltTo(ofRandom(1), ofRandom(1));
      hasTarget = true;
    }
  }

  // Leave laser off while setting up. Nobody needs to know about this yet. (-:
  //if (ofGetSystemTime() > laserToggleTime) {
    //setLaser(!getLaser());
    //laserToggleTime = ofGetSystemTime() + 1000; //(int)(3000 + floor(ofRandom(7000)));
  //}

  Camera::update();
}

void RandomMovementCamera::setPaused(bool v) {
  Camera::setPaused(v);

  if (v) {
    hasTarget = false;
    wakeTime = ofGetSystemTime();
  }
}

void RandomMovementCamera::sleep(int ms) {
  wakeTime = ofGetSystemTime() + ms;
}

bool RandomMovementCamera::isSleeping() {
  return ofGetSystemTime() < wakeTime;
}

