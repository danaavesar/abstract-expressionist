//
//  Circle-pollock.cpp
//  example-openni-of093
//
//  Created by Dana Avesar on 4/24/16.
//
//

#include "Circle-pollock.h"


//--------------------------------------------------------------
void Circle::setup(int x, int y, float _radius, bool _splash){
    //    radius = ofRandom(5,20);
    pos.set(x,y);
    radius = _radius;
    random = ofRandom(0,16);
    randomRadi= ofRandom(1,7);
    offset = 50;
    ranPosX = ofRandom(pos.x - offset,pos.x + offset );
    ranPosY = ofRandom(pos.y-offset, pos.y+offset);
    splash = _splash;
}

//--------------------------------------------------------------
void Circle::update(){
    
}

//--------------------------------------------------------------
void Circle::draw(){
    
    ofDrawEllipse(pos.x, pos.y, radius, radius);
    cout << radius << endl;
    //    random splashes
    if(random <= 3){
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
    }
    
    if(splash){
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
        ofDrawEllipse(ranPosX, ranPosY, randomRadi, randomRadi);
     
        
       
      
    }
}


