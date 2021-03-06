#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxXmlSettings.h"

#include "Camera.h"
#include "CameraCalibration.h"
#include "ExtentCalibration.h"
#include "KinectMarkerTracker.h"
#include "RandomMovementCamera.h"
#include "ServoCommand.h"

using namespace std;

class TwfApp : public ofBaseApp {

public:

  void setup();
  void update();
  void draw();
  void exit();

  void loadSettings();
  void saveSettings();

  void keyPressed(int key);
  void mouseMoved(int w, int h);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);

private:

  Camera* getCameraById(int id);
  void updateCameras();
  void startExtentCalibration(int id);
  void setMessage(string s);
  void shutdown();

  enum Constants {
    INPUT_NONE = -1,
    INPUT_EXTENT_CALIBRATION = 0,
    INPUT_PAUSE = 1,
    INPUT_LASER = 2
  };

  ofxXmlSettings settings;

  ofSerial serial;
  ServoCommand servoCommand;
  vector<Camera*> cameras;
  ExtentCalibration extentCalibration;
  CameraCalibration cameraCalibration;
  ofxKinect kinect;
  KinectMarkerTracker kinectMarkerTracker;

  ofTrueTypeFont pointFont;

  int inputState;
  string message;

  bool shuttingDown;
};
