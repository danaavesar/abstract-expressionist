//
//  Stroke-pollock.h
//  example-openni-of093
//
//  Created by Dana Avesar on 4/24/16.
//
//


#pragma once

#include "ofMain.h"
#include "Circle-pollock.h"
#include "Line-pollock.h"
#include "Weirdo-pollock.h"


class Stroke : public ofBaseApp{
    
public:
    void setup(int ID);
    void setColor(ofColor color);
    void draw();
    void loadPoints(int x, int y, int z, bool splash);
    
    int blobNumber;
    int divider = 70;
    vector <Circle> circles;
    vector <Line> lines;
    vector <Weirdo> weirdos;
    ofPolyline line;
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
