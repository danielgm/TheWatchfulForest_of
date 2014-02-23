#pragma once

#include "ofMain.h"
#include "ServoCommand.h"

class Camera {

  public:

    Camera();
    void setup(int id, ServoCommand &servoCommand);
    void update();

    int getId();

    void setPanExtent(int min, int max);
    void setTiltExtent(int min, int max);

    int getPan();
    void setPan(int pan);
    int getTilt();
    void setTilt(int tilt);
    void setPanAndTilt(int pan, int tilt);
    void setPanAndTiltHome();
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

    ServoCommand* servoCommand;

    int id;

    float panMin;
    float panMax;
    float tiltMin;
    float tiltMax;

    float panStart;
    float panTarget;

    float tiltStart;
    float tiltTarget;

    long long int animationStart;
    int animationDuration;
    bool oneLastFrame;

    bool isLaserOn;

    ofVec3f position;
    ofVec3f direction;
};
