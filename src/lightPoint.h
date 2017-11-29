//
//  lightPoint.h
//  CLP_Knoxville_Wall
//
//  Created by Jakob Marsico on 4/8/16.
//
//

#ifndef __CLP_Knoxville_Wall__lightPoint__
#define __CLP_Knoxville_Wall__lightPoint__

#include <stdio.h>
#include "ofMain.h"

class LightPoint {
    
public:
    LightPoint(){}
    
    void setup(ofVec2f _loc);
    
    void setAvgSamplingSize(int _numSamples);
    void setCurrentVal(int _val);
    uint8_t getAvgVal();
    ofVec2f getLoc() { return loc; }
    void setPixelRad(float _rad) { rad = _rad; };
    
    void draw();
    
    
protected:
    
    int numSamples;
    vector<uint8_t> samples;
    int index;
    
    
    ofVec3f loc;
    uint8_t avgVal;
    int currentVal;
    float rad;
};

#endif /* defined(__CLP_Knoxville_Wall__lightPoint__) */
