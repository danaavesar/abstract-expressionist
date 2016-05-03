//
//  Line.cpp
//  pollock-stroke-new
//
//  Created by Dana Avesar on 2/15/16.
//
//

#include "line.h"
#include "ofMain.h"

void Line::setup(float _mouseX, float _mouseY, float _prevMouseX, float _prevMouseY, float _weight ){
    random.x = ofRandom(posBeg.x, posEnd.x);
    random.y = ofRandom(posBeg.y, posEnd.y);
}
void Line::update(){
}

void Line::draw(){
    
    ofNoFill();
    //    ofSetColor(0,0,0);
    ofBeginShape();
    ofSetLineWidth(width);
    //    ofCurveVertex(posBeg.x, posBeg.y); //begining
    //    ofCurveVertex(posBeg.x, posBeg.y); //beginning duplicated
    //    ofCurveVertex(random.x, random.y);
    //    ofCurveVertex(posEnd.x, posEnd.y); //end
    //    ofCurveVertex(posEnd.x, posEnd.y); //end duplicated
    //
    ofVertex(posBeg.x, posBeg.y); //begining
    
    ofVertex(posEnd.x, posEnd.y); //end duplicated
    
    ofEndShape();
}