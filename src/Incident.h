//
//  Incident.hpp
//  ULRDataVisPrototype
//
//  Created by Sam Nosenzo on 11/17/17.
//

#ifndef Incident_h
#define Incident_h

#include "ofMain.h"

class Incident {
    
    public:
    
    Incident();
    void setup(float, float);
    bool readyToDie();
    void update();
    void display();
    float x;
    float y;
    float radius;
    float alpha;
    int life;
    
    private:
};


#endif /* Incident_h */
