//
//  lightPoint.cpp
//  CLP_Knoxville_Wall
//
//  Created by Jakob Marsico on 4/8/16.
//
//

#include "lightPoint.h"

void LightPoint::setup(ofVec2f _loc){
    numSamples = 1;
    index = 0;
    loc = _loc;
    
    uint8_t start;
    start = 0;
    samples.push_back(start);
    currentVal = 0;
    rad = 2.0;
}


void LightPoint::setAvgSamplingSize(int _numSamples){
    if(numSamples != _numSamples){
        numSamples = _numSamples;
        samples.clear();
        for(size_t i = 0; i < numSamples; i++){
            samples.push_back(0);
        }
        index = 0;
    }
    
    
    
}



void LightPoint::setCurrentVal(int _val){
    index ++;
    if(index >= numSamples) index = 0;
    
    samples[index] = _val;
    
    currentVal = _val;
//    ofLog() << "set: " << currentVal;
    
}

uint8_t LightPoint::getAvgVal(){
    int total = 0;
    
    for(size_t i = 0; i < numSamples; i++){
        total += samples[i];
    }
    
    avgVal = total/numSamples;
    
    return (uint8_t)avgVal;
}


void LightPoint::draw(){
    
//    ofLog() << "draw: " << currentVal;
    ofFill();
    //ofSetLineWidth(8);
    ofSetColor(getAvgVal());
    ofDrawSphere(loc, rad);
    

}

