#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(3);
	ofSetCircleResolution(60);

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
	auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	ofColor color;
	if (ofGetFrameNum() % 5 == 0) {
		
		this->radius_list.push_back(5);
		this->max_radius_list.push_back(400);
		this->rotate_list.push_back(
			glm::vec3(
				ofMap(ofNoise(1984, ofGetFrameNum() * 0.0035), 0, 1, -360, 360),
				ofMap(ofNoise(1103, ofGetFrameNum() * 0.0035), 0, 1, -360, 360),
				ofMap(ofNoise(0702, ofGetFrameNum() * 0.0035), 0, 1, -360, 360))
		);
		color.setHsb((ofGetFrameNum() * 5) % 255, 255, 255);
		this->color_list.push_back(color);
	}

	for (int i = this->radius_list.size() - 1; i > -1; i--) {

		if (this->radius_list[i] < this->max_radius_list[i]) {

			this->radius_list[i] += 5;
		}
		else {

			this->radius_list.erase(this->radius_list.begin() + i);
			this->max_radius_list.erase(this->max_radius_list.begin() + i);
			this->rotate_list.erase(this->rotate_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	for (int i = 0; i < this->radius_list.size(); i++) {

		ofPushMatrix();

		ofRotateZ(this->rotate_list[i].z);
		ofRotateY(this->rotate_list[i].y);
		ofRotateX(this->rotate_list[i].x);

		ofFill();
		ofSetColor(this->color_list[i], ofMap(this->radius_list[i], 10, this->max_radius_list[i], 128, 0));

		ofBeginShape();

		auto radius = this->radius_list[i] + 10;
		for (int deg = 0; deg < 360; deg++) {

			ofVertex(glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
		}
		
		ofNextContour(true);

		radius = this->radius_list[i] - 10;
		for (int deg = 0; deg < 360; deg++) {

			ofVertex(glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
		}

		ofEndShape(true);

		ofNoFill();
		ofSetColor(this->color_list[i], ofMap(this->radius_list[i], 10, this->max_radius_list[i], 255, 0));
		ofBeginShape();
		for (int deg = 0; deg < 360; deg++) {

			ofVertex(glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
		}
		ofEndShape(true);

		radius = this->radius_list[i] + 10;
		ofBeginShape();
		for (int deg = 0; deg < 360; deg++) {

			ofVertex(glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
		}
		ofEndShape(true);

		ofPopMatrix();
	}

	this->cam.end();

	int start = 250;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}