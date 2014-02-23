#pragma once

#include "ofMain.h"
#include "Camera.h"

class RandomMovementCamera : public Camera {

  public:

    RandomMovementCamera(int index, ServoCommand &servoCommand);

    void update();

  private:

    void sleep(int ms);
    bool isSleeping();

    unsigned long long wakeTime;
    bool hasTarget;
};
