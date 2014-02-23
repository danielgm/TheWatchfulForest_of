#include "RandomMovementCamera.h"

RandomMovementCamera::RandomMovementCamera(int id, ServoCommand &servoCommand)
    : Camera(id, servoCommand) {
  wakeTime = ofGetSystemTime();
  hasTarget = false;
}

void RandomMovementCamera::update() {
  if (hasTarget && !isAnimating()) {
    sleep(1000 + floor(ofRandom(4000)));
    hasTarget = false;
  }
  else if (!hasTarget && !isSleeping()) {
    panAndTiltTo(ofRandom(1), ofRandom(1));
    hasTarget = true;
  }

  Camera::update();
}

void RandomMovementCamera::sleep(int ms) {
  wakeTime = ofGetSystemTime() + ms;
}

bool RandomMovementCamera::isSleeping() {
  return ofGetSystemTime() < wakeTime;
}

