#pragma once

#include "ofMain.h"
#include "ofxKinect.h"

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

  void keyPressed(int key);
  void mouseMoved(int w, int h);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);

private:

  ofSerial serial;
  ServoCommand servoCommand;
  vector<Camera*> cameras;
  ExtentCalibration extentCalibration;
  CameraCalibration cameraCalibration;
  ofxKinect kinect;
  KinectMarkerTracker kinectMarkerTracker;

  ofTrueTypeFont pointFont;
};
