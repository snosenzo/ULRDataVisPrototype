//
//  Incident.cpp
//  ULRDataVisPrototype
//
//  Created by Sam Nosenzo on 11/17/17.
//

#include "Incident.h"


Incident::Incident() {
};

void Incident::setup(float xloc, float yloc) {
    x = xloc;
    y = yloc;
    radius = 3;
    alpha = 255;
}

bool Incident::readyToDie() {
    if(alpha <= 0) {
        return true;
    }
    return false;
}

void Incident::update() {
    radius += radInc;
    alpha -= alphaDec;
    
}

void Incident::display(){
    
    ofSetColor(255, alpha);
    ofDrawEllipse(x, y, radius*2, radius*2);
}
