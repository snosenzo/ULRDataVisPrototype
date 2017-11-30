#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel("ofxCsv", OF_LOG_VERBOSE);
    ofSetCircleResolution(100);
    
    if(csv.load("policedata.csv")) {
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
    
    facade_back.load("facade_no_screen.png");
    facade_screen.load("facade_screen.png");
    
    facade_back.resize(facade_back.getWidth()*0.75, facade_back.getHeight()*0.75);
    facade_screen.resize(facade_screen.getWidth()*0.75, facade_screen.getHeight()*0.75);
    
    gui.setup("gui", "settings.xml");
    gui.add(alphaDec.set("alphaDec", 0.1, 0.1, 2.0));
    gui.add(radInc.set("rad inc", 1.0, 0.1, 2.0));
    gui.add(cadence.set("cadence", 10, 1, 500));
    gui.add(numSamples.set("num samples", 10, 1, 100));
    gui.add(pixelRad.set("pixel radius", 1, 0.0, 3.0));
    gui.add(pitch.set("pitch", 6.0, 1.0, 12.0));
    gui.setPosition(facade_back.getWidth() + 10,10);
    gui.loadFromFile("settings.xml");
    
    syphon.setName("lawfirm_output");
    
    topLeft.set(325, 50);
    bottomRight.set(820, 330);
    
    sw = bottomRight.x - topLeft.x;
    sh = bottomRight.y - topLeft.y;
    pixPerInch = sw/(30*12);
    resetPitch();
    
    fbo.allocate(sw, sh);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(lastPitch != pitch) resetPitch();
    
    if(ofGetFrameNum() % cadence == 0) {
        
        float x = ofMap(csv[incidentIndex].getFloat(csv[incidentIndex].size() - 2), min_x, max_x, 0, sw);
        float y = ofMap(csv[incidentIndex].getFloat(csv[incidentIndex].size() - 1), min_y, max_y, 0, sh);
        if(x < ofGetWindowWidth() && x > 0 && y < ofGetWindowHeight() && y > 0) {
            Incident temp;
            temp.setup(x, y);
            incidents.push_front(temp);
            //            incidentIndex++;
            
        }
        incidentIndex++;
    }

    if(incidents.size() > 0 && incidents[incidents.size() - 1].readyToDie()) incidents.pop_back();
    
    for(auto& i: incidents) {
        
        i.radInc = radInc;
        i.alphaDec = alphaDec;
        i.lifeDec = lifeDec;
        i.update();
    }
    
    fbo.begin();
    ofClear(0,0,0,0);
    for(auto& i: incidents) {
        i.display();
    }
    fbo.end();
    
    ofSetColor(255,255,255,255);
    ofPixels samplePix;
    fbo.readToPixels(samplePix);
    for(auto& lp : lights){
        lp.setAvgSamplingSize(numSamples);
        lp.setCurrentVal(samplePix.getColor(lp.getLoc().x, lp.getLoc().y).getBrightness());
        lp.setPixelRad(pixelRad*pixPerInch);
    }
    lastPitch = pitch;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(0, 0, 0);
    
    ofSetColor(200,200,200,255);
    facade_back.draw(0,0);

    ofPushMatrix();
    ofTranslate(topLeft.x, topLeft.y);
    for(auto& lp: lights){
        lp.draw();
    }
    ofPopMatrix();
    
    ofSetColor(200,200,200,255);
    facade_screen.draw(0,0);
    
    syphon.publishScreen();
    
    string loc = ofToString(ofGetMouseX());
    loc += ",";
    loc += ofToString(ofGetMouseY());
    ofDrawBitmapStringHighlight(loc,ofGetMouseX(), ofGetMouseY());
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::resetPitch(){
    lights.clear();
    float screenpitch = pixPerInch * pitch;
    
    
    int numWide = sw/screenpitch;
    int numHigh = sh/screenpitch;
    
    
    for(int x = 0; x < numWide; x++){
        for(int y = 0; y < numHigh; y++){
            LightPoint lp;
            lp.setup(ofVec2f(x*screenpitch, y*screenpitch));
            //            lp.setup(ofVec2f(x*pitch+topLeft.x, y*pitch+topLeft.y));
            
            lights.push_back(lp);
        }
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

