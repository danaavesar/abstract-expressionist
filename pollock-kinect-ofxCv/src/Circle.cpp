//
//  Circle.cpp
//  pollock-stroke-new
//
//  Created by Dana Avesar on 2/15/16.
//
//

#include "Circle.h"

//--------------------------------------------------------------
void Circle::setup(int x, int y, float _radius){
    //    radius = ofRandom(5,20);
    pos.set(x,y);
    radius = _radius;
    random = ofRandom(0,16);
    randomRadi= ofRandom(3,7);
    offset = 50;
    ranPosX = ofRandom(pos.x - offset,pos.x + offset );
    ranPosY = ofRandom(pos.y-offset, pos.y+offset);
}

//--------------------------------------------------------------
void Circle::update(){
    
}

//--------------------------------------------------------------
void Circle::draw(){
    
    ofDrawEllipse(pos.x, pos.y, radius, radius);
    
    //    random splashes
    if(random <= 3){
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
    }
}



