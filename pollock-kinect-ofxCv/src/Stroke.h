//
//  Stroke.h
//  pollock-strokes-kinect-new
//
//  Created by Dana Avesar on 2/16/16.
//
//


#pragma once

#include "ofMain.h"
#include "Circle.h"
#include "Line.h"
#include "Weirdo.h"


class Stroke : public ofBaseApp{
    
public:
    void setup(int blobNumber);
    void setColor(ofColor color);
    void draw();
    void loadPoints(int x, int y, int z);
    
    int blobNumber;
    int divider = 90;
    vector <Circle> circles;
    vector <Line> lines;
    vector <Weirdo> weirdos;
    vector <int> prevYValues;
    int prevX;
    int prevZ;
    int prevY;
    float weight;
    int curveProbability;
    int weirdoCircleProbability;
    ofColor color;
    ofPoint center;
    
    bool selectedBlue;
    bool selectedRed;
    bool selectedBlack;
    bool selectedWhite;
    bool selectedYellow;
    
};