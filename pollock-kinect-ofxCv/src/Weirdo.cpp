//
//  Weirdo.cpp
//  pollock-stroke-new
//
//  Created by Dana Avesar on 2/15/16.
//
//

#include "Weirdo.h"
#include "ofMain.h"

void Weirdo::setup(float _mouseX, float _mouseY, float _prevMouseX, float _prevMouseY, float _size){
    //    posBeg.x= _prevMouseX;
    //    posBeg.y = _prevMouseY;
    posEnd.x = _mouseX;
    posEnd.y = _mouseY;
    scaleAmount= _size/100;
    rotation = ofRandom(-60,90);
}

void Weirdo::update(){
}

void Weirdo::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(posEnd.x, posEnd.y);
    ofRotate(rotation);
    ofScale(scaleAmount, scaleAmount);
    ofFill();
    ofBeginShape();
    ofVertex(0,0);
    ofBezierVertex(-5,-2,-13,0,-20,7);
    ofBezierVertex(-22,-7,-27,-18,-35,-18);
    ofBezierVertex(-45,-19,-55,0,-57,26);
    ofBezierVertex(-60,51,-54,72,-44,74);
    ofBezierVertex(-43,76,-42,77,-40,78);
    ofBezierVertex(-30,83,-14,70,-2,48);
    ofBezierVertex(8,26,9,4,0,0);
    ofEndShape();
    
    ofPopMatrix();
    ofPopStyle();
    
    
    
}
