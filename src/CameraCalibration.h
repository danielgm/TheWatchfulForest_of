#pragma once

#include "ofMain.h"
#include "Camera.h"

class CameraCalibration {

  public:

    CameraCalibration();

    void start(Camera &camera);
    void update();
    void recordPoint(ofVec3f p);
    void finish();
    bool isRunning();
    bool isComplete();
    bool hasError();
    Camera* getCamera();
    string getMessage();

  private:

    void setStep(int step);
    void nextStep();

    bool getMidpointBetweenLines(ofVec3f p0, ofVec3f v0, ofVec3f p1, ofVec3f v1, ofVec3f &result);

    enum Constants {
      STEP_READY = 0,
      STEP_POINT_0 = 1,
      STEP_POINT_1 = 2,
      STEP_POINT_2 = 3,
      STEP_POINT_3 = 4,
      STEP_COMPLETE = 5
    };

    int step;
    Camera* camera;

    ofVec3f point0;
    ofVec3f point1;
    ofVec3f point2;
    ofVec3f point3;

    bool _hasError;

    string message;
};
