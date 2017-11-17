#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel("ofxCsv", OF_LOG_VERBOSE);
    
    if(csv.load("policedata.csv")) {
        //csv.trim(); // Trim leading/trailing whitespace from non-quoted fields.
        
        // Like with C++ vectors, the index operator is a quick way to grab row
        // & col data, however this will cause a crash if the row or col doesn't
        // exist, ie. the file didn't load.
        ofLog() << "Print out a specific CSV value";
        ofLog() << csv[0][1];
        // also you can write...
        ofLog() << csv[0].at(1);
        // or you can get the row itself...
        ofxCsvRow row = csv[0];
        ofLog() << row.getString(0);
    }
    
    min_y = 100000;
    min_x = 100000;
    max_y = -100000;
    max_x = -100000;
    for(auto row: csv) {
        float x = row.getFloat(row.size() - 2);
        float y = row.getFloat(row.size() - 1);
        ofLog() << x << "|" << y;
        if(x > max_x && x!=0) max_x = x;
        if(y > max_y && y!=0) max_y = y;
        if(x < min_x && x!=0) min_x = x;
        if(y < min_y && y!=0) min_y = y;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    for(Incident i: incidents) {
        i.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(0, 0, 0);
    ofSetColor(255);
    if(ofGetFrameNum() % 10 == 0) {
        
        float x = ofMap(csv[incidentIndex].getFloat(csv[incidentIndex].size() - 2), min_x, max_x, 0, ofGetWindowWidth());
        float y = ofMap(csv[incidentIndex].getFloat(csv[incidentIndex].size() - 1), min_y, max_y, 0, ofGetWindowHeight());
        if(x < ofGetWindowWidth() && x > 0 && y < ofGetWindowHeight() && y > 0) {
            Incident temp;
            temp.setup(x, y);
            incidents.push_front(temp);
//            incidentIndex++;
    
        }
         incidentIndex++;
    }
    
    for(auto i: incidents) {
        i.display();
    }
    
    if(incidents.size() > 0 && incidents[incidents.size() - 1].readyToDie()) incidents.pop_back();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
