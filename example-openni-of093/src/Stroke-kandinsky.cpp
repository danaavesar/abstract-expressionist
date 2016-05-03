//
//  Stroke-kandinsky.cpp
//  example-openni-of093
//
//  Created by Dana Avesar on 4/25/16.
//
//

#include "Stroke-kandinsky.h"
//
//  Stroke2.cpp
//  kandinsky-stroke
//
//  Created by Dana Avesar on 4/22/16.
//
//




//--------------------------------------------------------------
void Stroke2::setup(ofColor _color, int _blobNumber){
    
    blobNumber = _blobNumber;
        color.r = ofRandom(0,255);
        color.g = ofRandom(0,255);
        color.b = ofRandom(0,255);
    selectedBlack = false;
    selectedRed = false;
    selectedYellow = false;
    selectedWhite = false;
    selectedBlue = false;
    numLines = 124;
    int random = ofRandom (0,10);
    int alpha;
    if(random <8){
        alpha = 255;
    }else{
        alpha = 20;
    }
    for(int i=0; i < numLines; i++){
        Line2 line;
        line.setup(color, alpha);
        lines.push_back(line);
        
    }
    
}

//--------------------------------------------------------------
void Stroke2::setColor(ofColor _color){
    color = _color;
  
    //numSpirals = 100;

}
//--------------------------------------------------------------
void Stroke2::draw(){

   
//   float spiralAmountX = ofMap(velocityMean.x, -16, 16, -159,  159 );
//    float spiralAmountY = ofMap(velocityMean.z, -16,16, -360,360);
   
//    ofPoint spiralAmount;
//    
//    spiralAmount.set(center.x + spiralAmountX, center.y + spiralAmountY);
   
    //radiusAdder.x = 1;
   
    //cur.addVertex(spiralAmount);
//    cur.getSmoothed(106);
    //ofSetLineWidth(3);
//        ofSetColor(233,149,0);
//        ofNoFill();
    ofPushMatrix();
   
    for(int i=0; i<numLines; i++){
        lines[i].draw();
    }
    ofPopMatrix();

    ofFill();
    ofSetColor(235,95,57);
    ofEllipse(center.x, center.y, 20, 15);
    
}

//--------------------------------------------------------------
void Stroke2::loadPoints(ofPoint pos, ofPoint _velocityMean){
    
    center = pos;
    velocityMean = _velocityMean;
    float spiralAmountX = ofMap(velocityMean.x, -16, 16, -159,  159 );
    float spiralAmountY = ofMap(velocityMean.z, -16,16, -360,360);
    ofPoint spiralAmount;
    spiralAmount.set(center.x + spiralAmountX, center.y + spiralAmountY);
    for(int i=0; i < numLines; i++){
        lines[i].update(spiralAmount);
        
    }
    
   
    
    
    
}
