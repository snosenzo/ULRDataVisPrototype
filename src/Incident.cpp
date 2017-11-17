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
    life = 500;
    radius = 3;
    alpha = 255;
}

bool Incident::readyToDie() {
    if(life < 0) {
        return true;
    }
    return false;
}

void Incident::update() {
    
    
}

void Incident::display(){
    life--;
    radius++;
    alpha-=.4;
    ofSetColor(255, alpha);
    ofDrawEllipse(x, y, radius*2, radius*2);
}
