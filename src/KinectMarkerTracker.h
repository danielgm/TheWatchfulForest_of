#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxCv.h"

class KinectMarkerTracker {

  public:

    KinectMarkerTracker();

    void setup(ofxKinect &kinect);
    void update();
    bool hasResult();
    ofVec3f getResult();

    void setMinAreaRadius(int n);
    void setMaxAreaRadius(int n);
    void setTargetColor(ofColor color);
    void setThreshold(int n);

  private:

    ofImage depthImage;
    ofxKinect* kinect;
    long foundTime;
    ofxCv::ContourFinder contourFinder;

    bool _hasResult;
    ofVec3f result;
};
