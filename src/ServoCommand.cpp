#include "ServoCommand.h"

ofSerial ServoCommand::getSerial() {
  return serial;
}

void ServoCommand::setSerial(ofSerial v) {
  serial = v;
}

void ServoCommand::setServo(int servo, int value) {
  sendInt(servo);
  serial.writeByte(':');
  sendInt(value);
  serial.writeByte(';');
}

void ServoCommand::setAnalog(int pin, int v) {
  sendString("p");
  sendString(":");
  sendInt(v);
  sendString(";");
}

void ServoCommand::setDigital(int pin, bool v) {
  sendString("p");
  sendInt(pin);
  sendString(":");
  sendInt(v ? 1 : 0);
  sendString(";");
}

void ServoCommand::sendString(string v) {
  for (int i = 0; i < v.length(); i++) {
    serial.writeByte(v[i]);
  }
}

void ServoCommand::sendInt(int v) {
  sendString(ofToString(v));
}

