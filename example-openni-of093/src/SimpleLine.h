//
//  SimpleLine.h
//  example-openni-of093
//
//  Created by Dana Avesar on 4/29/16.
//
//
#include "ofMain.h"

class SimpleLine : public ofBaseApp{
public:
    
    void setup(ofPoint _shoulderPos, ofPoint _handPos);
    void draw();
    void update();
    ofPoint posBeg;
    ofPoint posEnd;
    ofPoint vel;
    ofPoint frc;
    ofPolyline line;
    float damping;
    
};
