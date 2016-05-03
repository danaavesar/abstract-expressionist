//
//  Weirdo.cpp
//  pollock-stroke-new
//
//  Created by Dana Avesar on 2/15/16.
//
//

#include "Line.h"
#include "ofMain.h"

void Line2::setup(ofColor _color, int alpha){
    //    int deviant = 5;
    //    int x = ofRandom(_mouseX +deviant, _mouseX -deviant);
    //    int y = ofRandom(_mouseY +deviant, _mouseY -deviant);
    float hue = 0;  // The hue value to set.
    float saturation = 0; // The saturation value to set.
    float brightness = 0; // The brightness value to set.
    color = _color;
    
    randomNumber = ofRandom(0, 10);
    if(randomNumber < 6){
        width = ofRandom(.1,3);
        deviant.x = ofRandom(-15, 15);
        deviant.y = ofRandom(-15, 15);
        // color.a = 20; //ofRandom(20-155);
        color.getHsb(hue, saturation, brightness);
        color.setHsb(hue, saturation, brightness + ofRandom(0,10), alpha);
    }else{
        
        width = 1;
        //         width =.1;
        
        deviant.x = ofRandom(-30, 0);
        deviant.y = ofRandom(-30, 0);
        
        //            deviant.x = ofRandom(0, 25);
        //            deviant.y = ofRandom(0, 25);
        
        color.getHsb(hue, saturation, brightness);
        color.setHsb(hue + ofRandom(-5,5), saturation, brightness + ofRandom(-30,30), alpha);
    
        
        
    }
 
    
    
    
}

void Line2::update(ofPoint _pos){
    ofPoint point;
    //     int deviant = 5;
    //    int x = ofRandom(_mouseX +deviant, _mouseX -deviant);
    //    int y = ofRandom(_mouseY +deviant, _mouseY -deviant);
    point.set(_pos.x + deviant.x , _pos.y + deviant.y);
    line.addVertex(point);
    smoothedLine = line.getSmoothed(3);
}


void Line2::draw(){
    ofSetColor(color);
    
    ofSetLineWidth(width);
    
    smoothedLine.draw();
    
    
    
    
}
