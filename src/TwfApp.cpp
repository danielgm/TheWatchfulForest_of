#include "TwfApp.h"

using namespace ofxCv;
using namespace cv;

void TwfApp::setup() {
  //ofSetLogLevel(OF_LOG_VERBOSE);

  ofSetFrameRate(60);

  serial.listDevices();
  serial.setup(0, 9600);

  pointFont.loadFont("arial.ttf", 12);
}

void TwfApp::update() {
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

void TwfApp::keyPressed (int key) {
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

