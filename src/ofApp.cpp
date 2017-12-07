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
    gui.add(bShowAnim.set("show animation", true));
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
    
    string alleghenyOhioFilename = "AlleghenyOhio.json";
    string monFilename = "Mon.json";
    
    bool success = allOhioJson.open(alleghenyOhioFilename);
    success = success && monJson.open(monFilename);
    
    if(success) {
        for(int i = 0; i < allOhioJson["points"].size(); i++) {
            float x = ofMap(allOhioJson["points"][i][0].asFloat(), min_x, max_x, 0, sw);
            float y = ofMap(allOhioJson["points"][i][1].asFloat(), min_y, max_y, sw, 0);
            if(x < ofGetWindowWidth() && x > 0 && y < ofGetWindowHeight() && y > 0) {
                alleghenyOhioLine.addVertex(x,y);
            }
        }
        
        for(int i = 0; i < monJson["points"].size(); i++) {
            float x = ofMap(monJson["points"][i][0].asFloat(), min_x, max_x, 0, sw);
            float y = ofMap(monJson["points"][i][1].asFloat(), min_y, max_y, sw, 0);
            if(x < ofGetWindowWidth() && x > 0 && y < ofGetWindowHeight() && y > 0) {
                monLine.addVertex(x, y);
            }
            
        }
    } else {
        ofLogError() << "failed to read Json";
    }
    
    for(auto row: csv) {
        float x = ofMap(row.getFloat(row.size() - 2), min_x, max_x, 0, sw);
        float y = ofMap(row.getFloat(row.size() - 1), min_y, max_y, sw, 0);
        if(x < ofGetWindowWidth() && x > 0 && y < ofGetWindowHeight() && y > 0) {
            Incident temp;
            temp.setup(x, y);
            incidentMap[row.getString(4)].push_front(temp);;
        }
    }
    
    it = incidentMap.begin();
    int zero = 0;
    try {
        currentTime = Poco::DateTimeParser::parse(it->first, zero);
        ofLog() << "Succeeded: " << ofxTime::Utils::format(currentTime);
    } catch(const Poco::SyntaxException& exc) {
        ofLogError() << "Syntax Exception: \n" << exc.displayText();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(lastPitch != pitch) resetPitch();
    

    Poco::DateTime nextDateTime;
    try {
        int zero = 0;
        // Reads next DateTime from iterator
        nextDateTime = Poco::DateTimeParser::parse(it->first, zero);
//            ofLog() << "Succeeded: " << ofxTime::Utils::format(nextDateTime);
//            ofLog() <<  ofxTime::Utils::format(currentTime);
    } catch(const Poco::SyntaxException& exc) {
        ofLogError() << "Syntax Exception: \n" << exc.displayText();
    }
    if(currentTime == nextDateTime) {
        deque<Incident> deq = it->second;
        for(int i = 0; i < deq.size(); i++) {
            Incident inc = (Incident) deq.front();
            deq.pop_front();
            incidents.push_front(inc);
            ofLog() << it->first;
        }
        it++;
    }
    //Increment the minutes - 60 does 1 minute for some reason
    currentTime = ofxTime::Utils::addMinutes(currentTime, 60);

    if(incidents.size() > 0 && incidents[incidents.size() - 1].readyToDie()) incidents.pop_back();
    
    for(auto& i: incidents) {
        
        i.radInc = radInc;
        i.alphaDec = alphaDec;
        i.lifeDec = lifeDec;
        i.update();
    }
    
    fbo.begin();
        ofClear(0,0,0,0);
        ofPushMatrix();
        ofTranslate(0,-100);

            for(auto& i: incidents) {
                i.display();
            }
        
            ofSetColor(100);
            ofSetLineWidth(15);
            alleghenyOhioLine.draw();
            monLine.draw();
        ofPopMatrix();
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
    if(bShowAnim) fbo.draw(topLeft.x, topLeft.y);
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

