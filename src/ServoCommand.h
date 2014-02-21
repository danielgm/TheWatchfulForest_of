#pragma once

#include "ofMain.h"

class ServoCommand {
  public:

    ofSerial getSerial();
    void setSerial(ofSerial v);

    void setServo(int servo, int value);
    void setAnalog(int pin, int v);
    void setDigital(int pin, bool v);

  private:

    void sendString(string v);
    void sendInt(int v);

    ofSerial serial;
};
