#include "TwfApp.h"

using namespace ofxCv;
using namespace cv;

void TwfApp::setup() {
  //ofSetLogLevel(OF_LOG_VERBOSE);

  ofSetFrameRate(60);

  serial.listDevices();
  serial.setup(0, 38400);

  servoCommand.setSerial(serial);

  loadSettings();

  shuttingDown = false;
}

void TwfApp::update() {
  if (shuttingDown) {
    for (vector<Camera*>::iterator i = cameras.begin(); i != cameras.end(); ++i) {
      if (!(*i)->isAnimating()) {
        delete *i;
        cameras.erase(i, i + 1);
        i--;
      }
    }

    if (cameras.size() <= 0) {
      ofExit();
    }

	updateCameras();
    return;
  }

  if (extentCalibration.isRunning()) {
    return;
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
    return;
  }

  updateCameras();

  while (serial.available()) {
    cout << (char)serial.readByte();
  }
}

void TwfApp::draw() {
  ofBackground(100, 100, 100);

  if (extentCalibration.isRunning()
      || extentCalibration.isComplete()) {
    setMessage(extentCalibration.getMessage());
  }

  if (cameraCalibration.isRunning()
      || cameraCalibration.isComplete()) {
    setMessage(cameraCalibration.getMessage());
  }

  ofSetColor(255, 255, 255);
  stringstream reportStream;
  reportStream << "Some information.";
  pointFont.drawString(reportStream.str(), 20, 20);
  pointFont.drawString(message, 20, 40);
}

void TwfApp::exit() {
  for (int i = 0; i < cameras.size(); i++) {
    Camera* cam = cameras[i];
    delete cam;
  }
}

void TwfApp::loadSettings() {
  settings.loadFile("settings.xml");
  pointFont.loadFont("arial.ttf", settings.getValue("settings:fontSize", 0));

  settings.pushTag("settings");

  for (int i = 0; i < cameras.size(); i++) {
    delete cameras[i];
  }
  cameras.clear();

  settings.pushTag("cameras");
  int numCameras = settings.getNumTags("camera");
  for (int i = 0; i < numCameras; i++) {
    settings.pushTag("camera", i);

    Camera* cam = new RandomMovementCamera(
        settings.getValue("id", 0), servoCommand);
    cam->readSettings(settings);
    cam->setPanAndTiltHome();

    cameras.push_back(cam);

    settings.popTag();
  }

  settings.popTag();
  settings.popTag();

  setMessage("Settings loaded.");
}

void TwfApp::saveSettings() {
  settings.pushTag("settings");
  settings.clearTagContents("cameras");
  settings.pushTag("cameras");

  for (int i = 0; i < cameras.size(); i++) {
    Camera* cam = cameras[i];
    settings.addTag("camera");
    settings.pushTag("camera", i);

    cam->pushSettings(settings);

    settings.popTag();
  }

  settings.popTag();

  settings.saveFile("settings.xml");

  setMessage("Settings saved.");
}

void TwfApp::keyPressed(int key) {
  switch (key) {
    case 'x':
      shutdown();
      break;

    case 'l':
      loadSettings();
      break;

    case 's':
      saveSettings();
      break;

    case 'e':
      inputState = INPUT_EXTENT_CALIBRATION;
      setMessage("Extents calibration: input a digit for the camera ID.");
      break;

    case '0'...'9':
      if (inputState == INPUT_EXTENT_CALIBRATION) {
        int id = key - '0';
        startExtentCalibration(id);
      }
  }
}

void TwfApp::mouseMoved(int x, int y) {
  if (extentCalibration.isRunning()) {
    extentCalibration.update(
        ofClamp((float)mouseX / ofGetWindowWidth(), 0, 1),
        ofClamp((float)mouseY / ofGetWindowHeight(), 0, 1));
  }
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

void TwfApp::updateCameras() {
  for (int i = 0; i < cameras.size(); i++) {
    Camera* cam = cameras[i];
    cam->update();
  }
}

void TwfApp::startExtentCalibration(int id) {
  for (int i = 0; i < cameras.size(); i++) {
    Camera* cam = cameras[i];
    if (cam->getId() == id) {
      extentCalibration.start(*cam);
      return;
    }
  }

  stringstream ss;
  ss << "Camera not found. id=" << id << ", cameras.size=" << cameras.size() << endl;
  setMessage(ss.str());
}

void TwfApp::setMessage(string s) {
  if (message != s) {
    cout << s;
  }
  message = s;
}

void TwfApp::shutdown() {
  // Get all the cameras to return home.
  shuttingDown = true;
  for (int i = 0; i < cameras.size(); i++) {
    Camera* cam = cameras[i];
    cam->panAndTiltTo(0.5, 0.5);
  }

  setMessage("Quitting...");
}
