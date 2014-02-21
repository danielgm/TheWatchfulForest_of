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
  cam.setPanRange(200, 700);
  cam.setTiltRange(200, 700);
  cam.setPanAndTiltHome();

  pointFont.loadFont("arial.ttf", 12);
}

void TwfApp::update() {
  cam.update();
  while (serial.available()) {
    cout << (char)serial.readByte();
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
    case '0':
      cam.panAndTiltTo(450, 450);
      break;

    case '1':
      cam.panAndTiltTo(300, 500);
      break;
    case '2':
      cam.panAndTiltTo(400, 300);
      break;
    case '3':
      cam.panAndTiltTo(700, 400);
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

