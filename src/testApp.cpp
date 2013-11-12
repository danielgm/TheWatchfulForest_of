#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	
	minAreaRadius = 2;
	maxAreaRadius = 10;
	
	contourFinder.setTargetColor(ofColor(16, 161, 16), TRACK_COLOR_RGB);
	contourFinder.setMinAreaRadius(minAreaRadius);
	contourFinder.setMaxAreaRadius(maxAreaRadius);
	contourFinder.setThreshold(50);
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	kinectAngle = 0;
	kinect.setCameraTiltAngle(kinectAngle);
	
	serial.listDevices();
	serial.setup(0, 9600);
	
	isLaserOn = false;
	
	okSound.loadSound("ok.wav");
	
	calibrationStep = -1;
	isCalibrated = false;
	
	pointFont.loadFont("arial.ttf", 12);
	
	realTimeTracking = false;
	
	calibrationFilename = "calibration.txt";
	isCalibrated = loadCalibration();
	
	isDrawingMask = false;
}


void testApp::update() {
	ofBackground(100, 100, 100);
	
	kinect.update();
	if(kinect.isFrameNew()) {
		drawImage.setFromPixels(kinect.getPixelsRef());
		contourFinder.findContours(drawImage);
		
		long now = ofGetSystemTime();
		if (isCalibrated && realTimeTracking) {
			if (now > contourFoundTime + 50) {
				if (contourFinder.size() > 0 && contourFinder.getContourArea(0) > 0) {
					pScreen = toOf(contourFinder.getCentroid(0));
					pWorld = kinect.getWorldCoordinateAt(pScreen.x, pScreen.y);
					ofVec3f translated = pWorld - cameraPosition;
					ofVec3f rotated = translated.getRotatedRad(0, -atan2(cameraDirection.x, cameraDirection.z), 0);
					pCamera = rotated;
						
					int pan = 90 + atan2(pCamera.x, pCamera.z) * 180/PI;
					int tilt = 90 - atan2(pCamera.y, pCamera.z) * 180/PI;
					
					panAndTiltTo(pan, tilt);
					contourFoundTime = now;
				}
			}
		}
		else {
			if (now > contourFoundTime + 5000) {
				if (contourFinder.size() > 0 && contourFinder.getContourArea(0) > 0) {
					pScreen = toOf(contourFinder.getCentroid(0));
					pWorld = kinect.getWorldCoordinateAt(pScreen.x, pScreen.y);
					
					if (isCalibrated) {
						ofVec3f translated = pWorld - cameraPosition;
						ofVec3f rotated = translated.getRotatedRad(0, -atan2(cameraDirection.x, cameraDirection.z), 0);
						pCamera = rotated;
						
						cout << "pWorld: " << pWorld << endl;
						cout << "translted: " << translated << endl;
						cout << "pCamera: " << pCamera << endl;
						
						int pan = 90 + atan2(pCamera.x, pCamera.z) * 180/PI;
						int tilt = 90 - atan2(pCamera.y, pCamera.z) * 180/PI;
						
						panAndTiltTo(pan, tilt);
					}
					else {
						calibrationStep = handleCalibrationStep(calibrationStep);
					}
					
					okSound.play();
					contourFoundTime = now;
				}
			}
		}
	}
}


void testApp::draw() {
	ofSetColor(255, 255, 255);
	
	if (drawImage.isAllocated()) {
		if (isDrawingMask) {
			kinect.drawDepth(kinect.width, 0);
		}
		else {
			drawImage.draw(0, 0);
		}
		contourFinder.draw();
		
		long now = ofGetSystemTime();
		if (now < contourFoundTime + 5000) {
			ofSetColor(255, 0, 0);
			ofNoFill();
			ofCircle(pScreen.x, pScreen.y, maxAreaRadius);
			
			ofSetColor(255);
			ofFill();
			pointFont.drawString("World: " + ofToString(pWorld) + "\nCamera: " + ofToString(pCamera), 20, 30);
		}
	}
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << ""
	<< "camera position: (" << cameraPosition << ")" << endl
	<< "camera direction: (" << cameraDirection << ")" << endl << endl
	<< "target color: " << targetColor << endl
	<< "fps: " << ofGetFrameRate() << endl;
	ofDrawBitmapString(reportStream.str(), 20, 500);
	
	if (isRecording) {
		recordingImage.grabScreen(0, 0, kinect.width, kinect.height);
		recordingImage.saveImage(recordingPath + "/frame" + ofToString(recordingImageIndex++, 0, 4, '0') + ".bmp");
	}
}


void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}


void testApp::keyPressed (int key) {
	switch (key) {
		case 't':
			setLaser(!isLaserOn);
			break;
		
		case 'c':
			panAndTiltTo(90, 90);
			setLaser(true);
			calibrationStep = 0;
			isCalibrated = false;
			
			cout << "Calibration: started." << endl;
			break;
			
		case 'd':
			isCalibrated = loadCalibration();
			break;
			
		case 's':
			saveCalibration();
			break;
		
		case 'r':
			realTimeTracking = !realTimeTracking;
			cout << "Real-time tracking: " << (realTimeTracking ? "on" : "off") << endl;
			break;
		
		case 'm':
			isRecording = !isRecording;
			if (isRecording) {
				// Create recording folder from timestamp.
				time_t rawtime;
				struct tm * timeinfo;
				char buffer [80];
				
				time (&rawtime);
				timeinfo = localtime (&rawtime);
				
				strftime(buffer, 80, "%Y-%m-%d %H-%M-%S", timeinfo);
				stringstream ss;
				ss << buffer;
				recordingPath = ss.str();
				
				ofDirectory d;
				if (!d.doesDirectoryExist(recordingPath)) {
					d.createDirectory(recordingPath);
					recordingImageIndex = 0;
				}
				
				cout << "Recording to " << recordingPath << endl;
			}
			else {
				cout << "Recorded " << recordingImageIndex << " frames to " << recordingPath << endl;
				recordingPath = "";
				recordingImageIndex = 0;
			}
	}
}

void testApp::mouseMoved(int x, int y) {
}

void testApp::mouseDragged(int x, int y, int button) {}

void testApp::mousePressed(int x, int y, int button) {
	targetColor = kinect.getPixelsRef().getColor(x, y);
	contourFinder.setTargetColor(targetColor, TRACK_COLOR_RGB);
}

void testApp::mouseReleased(int x, int y, int button) {
}

void testApp::windowResized(int w, int h) {
}

/**
 * Sends a command to the arduino controlling the servos.
 * Commands:
 *  p: pan
 *  t: tilt
 *  a: laser on
 *  b: laser off
 *
 * @param r Numeric parameter.
 * @param command The command to send.
 */
void testApp::servoCommand(char command) {
	serial.writeByte(command);
	cout << command << endl;
}
void testApp::servoCommand(int r, char command) {
	if (r >= 0) {
		string s = ofToString(r);
		for (int i = 0; i < s.length(); i++) {
			serial.writeByte(s[i]);
			cout << s[i];
		}
	}
	serial.writeByte(command);
	cout << command << endl;
}

void testApp::panAndTiltTo(int pan, int tilt) {
	servoCommand(pan, 'p');
	servoCommand(tilt, 't');
}

void testApp::setLaser(bool on) {
	isLaserOn = on;
	servoCommand(on ? 'a' : 'b');
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
bool testApp::getMidpointBetweenLines(ofVec3f p0, ofVec3f v0, ofVec3f p1, ofVec3f v1, ofVec3f& result) {
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

bool testApp::loadCalibration() {
	cout << "Loading " << calibrationFilename << endl;
	
	ofBuffer b = ofBufferFromFile(calibrationFilename);
	vector<string> parts = ofSplitString(b.getText(), ",");
	if (parts.size() < 6) {
		cout << "Bad file format: calibration.txt should be six comma-separated values." << endl;
		return false;
	}
	else {
		cameraPosition = ofVec3f(ofToFloat(parts[0]), ofToFloat(parts[1]), ofToFloat(parts[2]));
		cameraDirection = ofVec3f(ofToFloat(parts[3]), ofToFloat(parts[4]), ofToFloat(parts[5]));
		return true;
	}
}

bool testApp::saveCalibration() {
	ofFile f(calibrationFilename, ofFile::WriteOnly);
	f << cameraPosition.x << ',' << cameraPosition.y << ',' << cameraPosition.z << ',' << cameraDirection.x << ',' << cameraDirection.y << ',' << cameraDirection.z;
	
	cout << "Saved " << calibrationFilename << endl;
	return true;
}

int testApp::handleCalibrationStep(int step) {
	switch (step) {
		case 0:
			calibrationLine0.p.x = pWorld.x;
			calibrationLine0.p.y = pWorld.y;
			calibrationLine0.p.z = pWorld.z;
			
			cout << "Calibration: got first point. Searching for second point." << endl;
			return step + 1;
			
		case 1:
			calibrationLine0.v.x = calibrationLine0.p.x - pWorld.x;
			calibrationLine0.v.y = calibrationLine0.p.y - pWorld.y;
			calibrationLine0.v.z = calibrationLine0.p.z - pWorld.z;
			
			// Move to next line.
			panAndTiltTo(90, 120);
			
			cout << "Calibration: got first line. Searching for point on second line." << endl;
			return step + 1;
			
		case 2:
			calibrationLine1.p.x = pWorld.x;
			calibrationLine1.p.y = pWorld.y;
			calibrationLine1.p.z = pWorld.z;
			
			cout << "Calibration: got first point. Searching for second point (line 2)." << endl;
			return step + 1;
			
		case 3:
			calibrationLine1.v.x = calibrationLine1.p.x - pWorld.x;
			calibrationLine1.v.y = calibrationLine1.p.y - pWorld.y;
			calibrationLine1.v.z = calibrationLine1.p.z - pWorld.z;
			
			if (getMidpointBetweenLines(calibrationLine0.p, calibrationLine0.v, calibrationLine1.p, calibrationLine1.v, cameraPosition)) {
				
				cout << "Calibration finished. Camera at: (" << cameraPosition << ")" << endl;
				
				cameraDirection.x = calibrationLine0.v.x;
				cameraDirection.y = calibrationLine0.v.y;
				cameraDirection.z = calibrationLine0.v.z;
				saveCalibration();
				
				setLaser(false);
				
				isCalibrated = true;
				return step + 1;
			}
			else {
				cout << "Calibration failed. Lines were parallel. Couldn't locate camera." << endl;
				
				cameraPosition = ofVec3f(0, 0, 0);
				cameraDirection = ofVec3f(0, 0, 0);
				isCalibrated = false;
				return 0;
			}
		
		default:
			return -1;
	}
}

