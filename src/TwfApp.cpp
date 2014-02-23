#include "TwfApp.h"

using namespace ofxCv;
using namespace cv;

void TwfApp::setup() {
  //ofSetLogLevel(OF_LOG_VERBOSE);

  ofSetFrameRate(60);

  serial.listDevices();
  serial.setup(0, 9600);

  servoCommand.setSerial(serial);

  cam.setup(0, servoCommand);
  cam.setPanExtent(200, 700);
  cam.setTiltExtent(200, 700);
  cam.setPanAndTiltHome();

  pointFont.loadFont("arial.ttf", 12);

  //extentCalibration.start(cam);
  cameraCalibration.start(cam);
}

void TwfApp::update() {
  if (extentCalibration.isRunning()) {
    extentCalibration.update(
        ofClamp((float)mouseX / ofGetWindowWidth(), 0, 1),
        ofClamp((float)mouseY / ofGetWindowHeight(), 0, 1));
  }
  //cam.update();

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
  reportStream << "";
  ofDrawBitmapString(reportStream.str(), 20, 500);
}

void TwfApp::exit() {
}

void TwfApp::keyPressed(int key) {
  cameraCalibration.recordPoint(ofVec3f(1, 2, 3));
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
}

void TwfApp::windowResized(int w, int h) {
}

