//
//  Circle.h
//  pollock-stroke-new
//
//  Created by Dana Avesar on 2/15/16.
//
//

#pragma once

#include "ofMain.h"


class Circle : public ofBaseApp{
    
public:
    void setup(int x, int y, float _radius);
    void update();
    void draw();
    
    int width;
    int radius;
    ofPoint pos;
    int random;
    int randomRadi;
    int offset;
    int ranPosX;
    int ranPosY;
    
    
};