#pragma once

#include "ofMain.h"
#include "Incident.h"
#include "ofxCsv.h"
#include "ofxGui.h"
#include "ofxSyphon.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofxCsv csv;
        ofxCsv csvRecorder;
    
    int incidentIndex;
    float max_x;
    float max_y;
    float min_x;
    float min_y;
    deque<Incident> incidents;
    
    ofxPanel gui;
    ofParameter<float> alphaDec;
    ofParameter<float> radInc;
    ofParameter<int> cadence;
    ofParameter<float> lifeDec;
    
    ofxSyphonServer syphon;
    ofFbo fbo;
    
		
};
