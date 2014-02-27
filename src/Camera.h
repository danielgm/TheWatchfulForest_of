#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "ServoCommand.h"

class Camera {

  public:

    Camera(int id, ServoCommand &servoCommand);
    virtual void update();

    int getId();

    void setPanExtent(int min, int max);
    void setTiltExtent(int min, int max);

    float getPan();
    void setPan(float pan);
    float getTilt();
    void setTilt(float tilt);
    void setPanAndTilt(float pan, float tilt);
    void setPanAndTiltHome();
    void panAndTiltTo(float pan, float tilt);
    void panAndTiltTo(float pan, float tilt, int duration);
    bool isAnimating();

    bool getLaser();
    void setLaser(bool on);

    ofVec3f getPosition();
    void setPosition(ofVec3f v);
    ofVec3f getDirection();
    void setDirection(ofVec3f v);

    void pushSettings(ofxXmlSettings &settings, int index);

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

    unsigned long long animationStart;
    int animationDuration;
    bool oneLastFrame;

    bool isLaserOn;

    ofVec3f position;
    ofVec3f direction;
};
