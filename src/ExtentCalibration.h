#pragma once

#include "ofMain.h"
#include "Camera.h"

class ExtentCalibration {

  public:

    ExtentCalibration();

    void start(Camera &camera);
    void update(float x, float y);
    void recordPoint(float x, float y);
    void finish();
    bool isRunning();
    bool isComplete();
    Camera* getCamera();
    string getMessage();

  private:

    void setStep(int step);
    void nextStep();

    const int BASE_EXTENT_MIN = 0;
    const int BASE_EXTENT_MAX = 1000;

    enum Constants {
      STEP_READY = 0,
      STEP_PAN_LEFT = 1,
      STEP_PAN_RIGHT = 2,
      STEP_TILT_UP = 3,
      STEP_TILT_DOWN = 4,
      STEP_COMPLETE = 5
    };

    int step;
    Camera* camera;

    float panLeft;
    float panRight;
    float tiltUp;
    float tiltDown;

    string message;
};
