#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxCv.h"

struct Line {
  ofVec3f p;
  ofVec3f v;
};

class testApp : public ofBaseApp {
public:

  void setup();
  void update();
  void draw();
  void exit();

  void keyPressed(int key);
  void mouseMoved(int w, int h);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);

  void writeString(string v);
  void writeInt(int v);
  void setPanAndTilt(int pan, int tilt);
  void setServo(int servo, int value);
  void setLaser(bool on);

  bool getMidpointBetweenLines(ofVec3f p0, ofVec3f v0, ofVec3f p1, ofVec3f v1, ofVec3f& result);

  bool loadCalibration();
  bool saveCalibration();
  int handleCalibrationStep(int step);

  ofxKinect kinect;
  ofImage drawImage;

  ofxCv::ContourFinder contourFinder;
  ofColor targetColor;
  float colorThreshold;
  long contourFoundTime;

  int kinectAngle;
  int minAreaRadius;
  int maxAreaRadius;

  ofSerial serial;

  bool isLaserOn;

  ofSoundPlayer okSound;

  // Calibration steps:
  // 0 - record first point on first line.
  // 1 - record second point on first line.
  // 2 - record first point on second line.
  // 3 - record second point on second line.
  // 4 - done.
  int calibrationStep;
  Line calibrationLine0;
  Line calibrationLine1;
  bool isCalibrated;

  ofVec3f cameraPosition;
  ofVec3f cameraDirection;

  ofVec2f pScreen;
  ofVec3f pWorld;
  ofVec3f pCamera;

  ofTrueTypeFont pointFont;

  bool realTimeTracking;

  string calibrationFilename;

  bool isRecording;
  ofImage recordingImage;
  string recordingPath;
  int recordingImageIndex;

  int isDrawingMask;
};
