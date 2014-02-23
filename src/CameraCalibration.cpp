#include "CameraCalibration.h"

CameraCalibration::CameraCalibration() {
  step = STEP_READY;
  _hasError = false;
}

void CameraCalibration::start(Camera &_camera) {
  camera = &_camera;

  setStep(STEP_POINT_0);
}

void CameraCalibration::update() {
}

void CameraCalibration::recordPoint(ofVec3f p) {
  switch (step) {
    case STEP_POINT_0:
      point0 = p;
      nextStep();
      break;
    case STEP_POINT_1:
      point1 = p;
      nextStep();
      break;
    case STEP_POINT_2:
      point2 = p;
      nextStep();
      break;
    case STEP_POINT_3:
      point3 = p;
      nextStep();
      break;
  }
}

void CameraCalibration::finish() {
  setStep(STEP_READY);
}

bool CameraCalibration::isRunning() {
  return step != STEP_READY && step != STEP_COMPLETE;
}

bool CameraCalibration::isComplete() {
  return step == STEP_COMPLETE;
}

bool CameraCalibration::hasError() {
  return _hasError;
}

Camera* CameraCalibration::getCamera() {
  return camera;
}

string CameraCalibration::getMessage() {
  return message;
}

void CameraCalibration::setStep(int s) {
  step = s;
  stringstream ss;
  ofVec3f position;

  switch (step) {
    case STEP_READY:
      _hasError = false;
      message = "";
      break;

    case STEP_POINT_0:
      camera->setLaser(true);
      camera->setPanAndTiltHome();
      message = "Record near point on first line.";
      break;

    case STEP_POINT_1:
      camera->setLaser(true);
      message = "Record far point on first line.";
      break;

    case STEP_POINT_2:
      camera->setLaser(true);
      camera->setPanAndTilt(0.5, 0.75);
      message = "Record near point on second line.";
      break;

    case STEP_POINT_3:
      camera->setLaser(true);
      message = "Record far point on second line.";
      break;

    case STEP_COMPLETE:
      camera->setLaser(false);

      if (getMidpointBetweenLines(point0, point1 - point0, point2, point3 - point2, position)) {
        ofVec3f direction = point1 - point0;

        camera->setPosition(position);
        camera->setDirection(direction);

        ss << "Camera calibration complete."
          << "\n\tposition=" << position
          << "\n\tdirection=" << direction;
        message = ss.str();
      }
      else {
        _hasError = true;

        ss << "Calibration failed. Lines were parallel. Couldn't locate camera."
          << "\n\tpoint0=" << point0
          << "\n\tpoint1=" << point1
          << "\n\tpoint2=" << point2
          << "\n\tpoint3=" << point3;
        message = ss.str();
      }
      break;
  }
}

void CameraCalibration::nextStep() {
  switch (step) {
    case STEP_COMPLETE:
      setStep(STEP_READY);
      break;

    default:
      setStep(step + 1);
  }
}

/**
 * Fetches the midpoint of the shortest line between two lines.
 *
 * The lines are defined as
 * L1(s) = p0 + s * v0
 * L2(t) = p1 + t * v1
 *
 * Returns false if there is no single shortest line between the given lines.
 */
bool CameraCalibration::getMidpointBetweenLines(ofVec3f p0, ofVec3f v0, ofVec3f p1, ofVec3f v1, ofVec3f &result) {
  ofVec3f wp = p0 - p1;

  float a = v0.dot(v0);
  float b = v0.dot(v1);
  float c = v1.dot(v1);
  float d = v0.dot(wp);
  float e = v1.dot(wp);

  float denom = a * c - b * b;
  if (denom == 0) {
    // The lines are parallel so the midpoint between them is undefined.
    return false;
  }
  else {
    // Time parameters values for the closest points on each line.
    float sc = (b * e - c * d) / denom;
    float tc = (a * e - b * d) / denom;

    // Closest points on each line.
    ofVec3f c0 = p0 + sc * v0;
    ofVec3f c1 = p1 + tc * v1;

    // Return midpoint between those two points.
    result = c0 + (c1 - c0) / 2;
    return true;
  }
}

