#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "ServoCommand.h"

class Camera {

  public:

    Camera(int id, ServoCommand &servoCommand);
    virtual void update();

    int getId();

    int getLaserPin();
    void setLaserPin(int v);

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

    bool getAllowLaser();
    void setAllowLaser(bool v);

    virtual bool getPaused();
    virtual void setPaused(bool paused);

    ofVec3f getPosition();
    void setPosition(ofVec3f v);
    ofVec3f getDirection();
    void setDirection(ofVec3f v);

    virtual void readSettings(ofxXmlSettings &settings);
    virtual void pushSettings(ofxXmlSettings &settings);

    // Sends a null signal to the camera servos.
    void sendNullSignal();

    virtual bool isShuttingDown();
    virtual void shutdown();

  private:

    int calculateDuration(float panStart, float tiltStart, float panTarget, float tiltTarget);

    float easeIn(float t, float b, float c, float d);
    float easeOut(float t, float b, float c, float d);
    float easeInOut(float t, float b, float c, float d);

    ServoCommand* servoCommand;

    int id;

    // NOTE: Don't use pins 0 and 1 for the laser. They're used
    // by the Adafruit 16-channel PWM shield.
    int laserPin;

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
    bool allowLaser;
    bool isPaused;

    int nullSignalDelay;
    unsigned long long nullSignalTime;

    ofVec3f position;
    ofVec3f direction;

    bool shuttingDown;
};
