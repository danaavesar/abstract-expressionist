//
//  Circle-pollock.h
//  example-openni-of093
//
//  Created by Dana Avesar on 4/24/16.
//
//

#pragma once

#include "ofMain.h"


class Circle : public ofBaseApp{
    
public:
    void setup(int x, int y, float _radius, bool splash);
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
    Boolean splash;
    
    
};