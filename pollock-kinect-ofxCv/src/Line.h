//
//  Line.h
//  pollock-stroke-new
//
//  Created by Dana Avesar on 2/15/16.
//
//


#pragma once
#include "ofMain.h"

class Line : public ofBaseApp{
public:
    
    void setup(float _mouseX, float _mouseY, float _prevMouseX, float _prevMouseY, float _weight );
    void draw();
    void update();
    ofPoint posBeg;
    ofPoint posEnd;
    float width;
    ofPoint random;
    int randomR = ofRandom(0,255);
    int randomG = ofRandom(0,255);
    int randomB = ofRandom(0,255);
    
    
};
