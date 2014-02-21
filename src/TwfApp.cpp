#include "TwfApp.h"

using namespace ofxCv;
using namespace cv;

void TwfApp::setup() {
  //ofSetLogLevel(OF_LOG_VERBOSE);

  ofSetFrameRate(60);

  serial.listDevices();
  serial.setup(0, 9600);

  servoCommand.setSerial(serial);

  pointFont.loadFont("arial.ttf", 12);
}

void TwfApp::update() {
  while (serial.available()) {
    cout << serial.readByte();
  }
}

void TwfApp::draw() {
  ofBackground(100, 100, 100);

  ofSetColor(255, 255, 255);
  stringstream reportStream;
  reportStream << "";
  ofDrawBitmapString(reportStream.str(), 20, 500);
}

void TwfApp::exit() {
}

void TwfApp::keyPressed(int key) {
  switch (key) {
    case '1':
      servoCommand.setServo(0, 200);
      break;
    case '2':
      servoCommand.setServo(0, 400);
      break;
    case '3':
      servoCommand.setServo(0, 600);
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
}

void TwfApp::windowResized(int w, int h) {
}

