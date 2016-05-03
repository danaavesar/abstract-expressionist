//
//  Weirdo2.h
//  kandinsky-stroke
//
//  Created by Dana Avesar on 4/22/16.
//
//

#pragma once
#include "ofMain.h"

class Line2 : public ofBaseApp{
public:
    
    void setup(ofColor _color, int alpha);
    void draw();
    void update(ofPoint _pos);
    ofPoint pos;
    
    float scaleAmount = 1;
    ofPoint rotation;
    int width;
    int opacity;
    int green;
    int blue;
    ofPoint vel;
    ofPoint accel;
    ofPolyline line;
    ofPolyline smoothedLine;
    ofPoint deviant;
    ofColor color;
    int randomNumber;
    int randomNumber2;
};
