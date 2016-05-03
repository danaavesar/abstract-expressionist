//
//  Stroke-kandinsky.h
//  example-openni-of093
//
//  Created by Dana Avesar on 4/25/16.
//



#pragma once

#include "ofMain.h"
#include "Line.h"



class Stroke2 : public ofBaseApp{
    
public:
    void setup(ofColor _color, int blobNumber);
    void setColor(ofColor color);
    void draw();
    void loadPoints(ofPoint pos, ofPoint poTorso);
    
//    int blobNumber;
//    int divider = 30;
//    vector <int> prevYValues;
//    //    int prevX;
//    //    int prevZ;
//    //    int prevY;
//    float weight;
//    
//    ofColor color;
//    ofPoint center;
//    ofPoint prev;
//    ofPoint radius;
//    ofPoint radiusAdder;
    ofPoint velocityMean;
//    float angleStep;
//    int numSpirals;
    ofPolyline cur;
    
    bool selectedBlue;
    bool selectedRed;
    bool selectedBlack;
    bool selectedWhite;
    bool selectedYellow;
    
    int blobNumber;
    int divider = 90;
    
    vector <int> prevYValues;
    vector <Line2> lines;
    //    int prevX;
    //    int prevZ;
    //    int prevY;
    float weight;
    
    ofColor color;
    ofPoint center;
    ofPoint prev;
    ofPolyline polyline;
    
 
    
    ofPoint rotation;
    //    ofPoint deviant;
    
    int width;
    int opacity;
    int green;
    int blue;
    int numLines;
    
    
};