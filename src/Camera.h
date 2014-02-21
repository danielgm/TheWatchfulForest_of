#pragma once

#include "ofMain.h"

class Camera {

  public:

    Camera();
    Camera(int id, int panMin, int panMax, int tiltMin, int tiltMax);
    int getId();
    int getPan();
    int getTilt();
    void setPanAndTilt(int pan, int tilt);
    void panAndTiltTo(int pan, int tilt);
    void panAndTiltTo(int pan, int tilt, int duration);
    bool isAnimating();

    bool getLaser();
    void setLaser(bool on);

    ofVec3f getPosition();
    void setPosition(ofVec3f v);
    ofVec3f getDirection();
    void setDirection(ofVec3f v);

  private:

    int calculateDuration(float panStart, float tiltStart, float panTarget, float tiltTarget);

    float easeIn(float t, float b, float c, float d);
    float easeOut(float t, float b, float c, float d);
    float easeInOut(float t, float b, float c, float d);

    int id;

    float panMin;
    float panMax;
    float tiltMin;
    float tiltMax;

    float panStart;
    float panTarget;

    float tiltStart;
    float tiltTarget;

    long animationStart;
    int animationDuration;

    bool isLaserOn;

    ofVec3f position;
    ofVec3f direction;
};
