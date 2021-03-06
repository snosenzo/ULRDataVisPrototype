#pragma once

#include "ofMain.h"
#include "Incident.h"
#include "ofxCsv.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "lightPoint.h"
#include "ofxTime.h"
#include "ofxJSON.h"
#include "ofxRipples.h"
#include "ofxBounce.h"




class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void resetPitch();

		//void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );

		void gotMessage(ofMessage msg);
    
        ofxCsv csv;
        ofxCsv csvRecorder;
    
    int incidentIndex;
    float max_x;
    float max_y;
    float min_x;
    float min_y;
    deque<Incident> incidents;
    map<string, deque<Incident>> incidentMap;
    map<string, deque<Incident>>::iterator it;
    Poco::DateTime currentTime;
    
    
    ofxPanel gui;
    ofParameter<float> alphaDec;
    ofParameter<float> radInc;
    ofParameter<int> cadence;
    ofParameter<float> lifeDec;
    ofParameter<int> numSamples;
    ofParameter<bool> bShowAnim;
    ofParameter<float> pixelRad;
    ofParameter<float> pitch;
    ofParameter<float> damping;
    ofPolyline alleghenyOhioLine;
    ofPolyline monLine;
    ofxSyphonServer syphon;
    ofFbo fbo;
    vector<LightPoint> lights;
    ofxJSONElement allOhioJson;
    ofxJSONElement monJson;
    
    ofImage facade_back;
    ofImage facade_screen;
    ofVec2f topLeft;
    ofVec2f bottomRight;
    float sw;
    float sh;
    float pixPerInch;
    float lastPitch;
    
    
    ofxRipples  rip;
    ofxBounce   bounce;
		
};
