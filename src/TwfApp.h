#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxCv.h"

#include "Camera.h"
#include "ServoCommand.h"

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

  ofSerial serial;

  ofTrueTypeFont pointFont;
};
