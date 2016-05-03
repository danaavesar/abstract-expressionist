//
//  Weirdo-pollock.h
//  example-openni-of093
//
//  Created by Dana Avesar on 4/24/16.
//
//

#pragma once
#include "ofMain.h"

class Weirdo : public ofBaseApp{
public:
    
    void setup(float _mouseX, float _mouseY, float _prevMouseX, float _prevMouseY, float _weight );
    void draw();
    void update();
    ofPoint posBeg;
    ofPoint posEnd;
    float scaleAmount = 1;
    float rotation;
    
};