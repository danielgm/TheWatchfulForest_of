#include "KinectMarkerTracker.h"

KinectMarkerTracker::KinectMarkerTracker() {
  _hasResult = false;

  contourFinder.setTargetColor(ofColor(64, 153, 33), ofxCv::TRACK_COLOR_RGB);
  contourFinder.setMinAreaRadius(1);
  contourFinder.setMaxAreaRadius(10);
  contourFinder.setThreshold(50);

  foundTime = ofGetSystemTime();
}

void KinectMarkerTracker::setup(ofxKinect &_kinect) {
  kinect = &_kinect;
}

void KinectMarkerTracker::update() {
  _hasResult = false;
  result = ofVec3f();

  depthImage.setFromPixels(kinect->getPixelsRef());
  contourFinder.findContours(depthImage);

  long now = ofGetSystemTime();
  if (now > foundTime + 5000) {
    if (contourFinder.size() > 0 && contourFinder.getContourArea(0) > 0) {
      ofVec3f pScreen = ofxCv::toOf(contourFinder.getCentroid(0));
      ofVec3f pWorld = kinect->getWorldCoordinateAt(pScreen.x, pScreen.y);

      if (pWorld.x != 0 && pWorld.y != 0 && pWorld.z != 0) {
        _hasResult = true;
        result = pWorld;
        foundTime = now;
      }
    }
  }
}

void KinectMarkerTracker::setMinAreaRadius(int n) {
  contourFinder.setMinAreaRadius(n);
}

void KinectMarkerTracker::setMaxAreaRadius(int n) {
  contourFinder.setMaxAreaRadius(n);
}

void KinectMarkerTracker::setTargetColor(ofColor c) {
  contourFinder.setTargetColor(c, ofxCv::TRACK_COLOR_RGB);
}

void KinectMarkerTracker::setThreshold(int n) {
  contourFinder.setThreshold(n);
}

bool KinectMarkerTracker::hasResult() {
  return _hasResult;
}

ofVec3f KinectMarkerTracker::getResult() {
  return result;
}
