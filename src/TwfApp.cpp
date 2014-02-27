#include "TwfApp.h"

using namespace ofxCv;
using namespace cv;

void TwfApp::setup() {
  //ofSetLogLevel(OF_LOG_VERBOSE);

  ofSetFrameRate(60);

  serial.listDevices();
  serial.setup(0, 9600);

  servoCommand.setSerial(serial);

  Camera* cam;

  cam = new RandomMovementCamera(42, servoCommand);
  cam->setPanExtent(200, 700);
  cam->setTiltExtent(200, 700);
  cam->setPanAndTiltHome();
  cameras.push_back(cam);

  cam = new RandomMovementCamera(0, servoCommand);
  cam->setPanExtent(200, 700);
  cam->setTiltExtent(200, 700);
  cam->setPanAndTiltHome();
  cameras.push_back(cam);

  loadSettings();

  //extentCalibration.start(cam);
  //cameraCalibration.start(cam);
}

void TwfApp::update() {
  if (extentCalibration.isRunning()) {
    extentCalibration.update(
        ofClamp((float)mouseX / ofGetWindowWidth(), 0, 1),
        ofClamp((float)mouseY / ofGetWindowHeight(), 0, 1));
  }

  if (cameraCalibration.isRunning()) {
    kinect.update();
    if (kinect.isFrameNew()) {
      kinectMarkerTracker.update();
      if (kinectMarkerTracker.hasResult()) {
        ofVec3f p = kinectMarkerTracker.getResult();
        cameraCalibration.recordPoint(p);
      }
    }
  }

  for (int i = 0; i < cameras.size(); i++) {
    Camera* cam = cameras[i];
    cam->update();
    //cam.setTargets(targets);
  }

  while (serial.available()) {
    cout << (char)serial.readByte();
  }
}

void TwfApp::draw() {
  ofBackground(100, 100, 100);

  if (extentCalibration.isRunning() || extentCalibration.isComplete()) {
    ofDrawBitmapString(extentCalibration.getMessage(), 20, 500);
  }

  if (cameraCalibration.isRunning()
      || cameraCalibration.isComplete()) {
    ofDrawBitmapString(cameraCalibration.getMessage(), 20, 500);
  }

  ofSetColor(255, 255, 255);
  stringstream reportStream;
  reportStream << "Some information.";
  pointFont.drawString(reportStream.str(), 20, 500);
}

void TwfApp::exit() {
}

void TwfApp::loadSettings() {
  settings.loadFile("settings.xml");
  pointFont.loadFont("arial.ttf", settings.getValue("settings:fontSize", 0));
}

void TwfApp::saveSettings() {

  settings.saveFile("settings.xml");
}

void TwfApp::keyPressed(int key) {
  switch (key) {
    case 'l':
      loadSettings();
      break;

    case 's':
      saveSettings();
      break;
  }
}

void TwfApp::mouseMoved(int x, int y) {
}

void TwfApp::mouseDragged(int x, int y, int button) {
}

void TwfApp::mousePressed(int x, int y, int button) {
}

void TwfApp::mouseReleased(int x, int y, int button) {
  if (extentCalibration.isRunning()) {
    extentCalibration.recordPoint(
        ofClamp((float)x / ofGetWindowWidth(), 0, 1),
        ofClamp((float)y / ofGetWindowHeight(), 0, 1));
  }
  else if (extentCalibration.isComplete()) {
    extentCalibration.finish();
  }

  // FIXME: Probably a better time to set the target color than while
  // running the camera calibration that uses it. \-:
  if (cameraCalibration.isRunning()) {
    ofColor targetColor = kinect.getPixelsRef().getColor(x, y);
    kinectMarkerTracker.setTargetColor(targetColor);
  }
}

void TwfApp::windowResized(int w, int h) {
}

