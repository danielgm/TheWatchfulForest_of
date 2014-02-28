#include "ExtentCalibration.h"

ExtentCalibration::ExtentCalibration() {
  step = STEP_READY;
}

void ExtentCalibration::start(Camera &_camera) {
  camera = &_camera;

  setStep(STEP_PAN_LEFT);
}

void ExtentCalibration::update(float x, float y) {
  camera->setPanAndTilt(x, y);
  camera->update();
}

void ExtentCalibration::recordPoint(float x, float y) {
  switch (step) {
    case STEP_PAN_LEFT:
      panLeft = ofMap(x, 0, 1, BASE_EXTENT_MIN, BASE_EXTENT_MAX);
      nextStep();
      break;
    case STEP_PAN_RIGHT:
      panRight = ofMap(x, 0, 1, BASE_EXTENT_MIN, BASE_EXTENT_MAX);
      nextStep();
      break;
    case STEP_TILT_UP:
      tiltUp = ofMap(y, 0, 1, BASE_EXTENT_MIN, BASE_EXTENT_MAX);
      nextStep();
      break;
    case STEP_TILT_DOWN:
      tiltDown = ofMap(y, 0, 1, BASE_EXTENT_MIN, BASE_EXTENT_MAX);
      nextStep();
      break;
  }
}

void ExtentCalibration::finish() {
  setStep(STEP_READY);
}

bool ExtentCalibration::isRunning() {
  return step != STEP_READY && step != STEP_COMPLETE;
}

bool ExtentCalibration::isComplete() {
  return step == STEP_COMPLETE;
}

Camera* ExtentCalibration::getCamera() {
  return camera;
}

string ExtentCalibration::getMessage() {
  return message;
}

void ExtentCalibration::setStep(int s) {
  step = s;
  stringstream ss;

  switch (step) {
    case STEP_READY:
      message = "";
      break;

    case STEP_PAN_LEFT:
      camera->setPaused(true);
      camera->setPanExtent(BASE_EXTENT_MIN, BASE_EXTENT_MAX);
      camera->setTiltExtent(BASE_EXTENT_MIN, BASE_EXTENT_MAX);
      message = "Pan left.";
      break;

    case STEP_PAN_RIGHT:
      message = "Pan right.";
      break;

    case STEP_TILT_UP:
      message = "Tilt up.";
      break;

    case STEP_TILT_DOWN:
      message = "Tilt down.";
      break;

    case STEP_COMPLETE:
      camera->setPaused(false);
      camera->setPanExtent(
          MIN(panLeft, panRight),
          MAX(panLeft, panRight));
      camera->setTiltExtent(
          MIN(tiltDown, tiltUp),
          MAX(tiltDown, tiltUp));
      camera->setPanAndTilt(0.5, 0.5);
      camera->panAndTiltTo(0.5, 0.5);

      ss << "Extent calibration complete. pan=["
        << MIN(panLeft, panRight) << ", "
        << MAX(panLeft, panRight) << "] tilt=["
        << MIN(tiltUp, tiltDown) << ", "
        << MAX(tiltUp, tiltDown) << "]";
      message = ss.str();
      break;
  }
}

void ExtentCalibration::nextStep() {
  setStep(step == STEP_COMPLETE ? STEP_READY : step + 1);
}
