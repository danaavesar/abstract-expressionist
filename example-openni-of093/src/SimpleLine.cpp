//
//  SimpleLine.cpp
//  example-openni-of093
//
//  Created by Dana Avesar on 4/29/16.
//
//

#include "SimpleLine.h"

#include "ofMain.h"

void SimpleLine::setup(ofPoint _shoulderPos, ofPoint _handPos ){
    posBeg = _shoulderPos;
    posEnd = _handPos;
    vel = posEnd*.02 - posBeg*.02;
    posBeg += vel;
    //vel.normalize();
    line.addVertex(posBeg.x, posBeg.y);
    line.addVertex(posEnd.x, posEnd.y);
    damping = 0.06f;
    
}
void SimpleLine::update(){
    frc.set(0,0); //reset force
    frc.x = frc.x - vel.x * damping; // add damping force
    frc.y = frc.y - vel.y * damping; // add damping force damping is a force operating in the oposite direction of the
    // velocity vector
    
    vel += frc;
    //posBeg += vel;
    posEnd += vel*1.5;
    line.clear();
    line.addVertex(posBeg.x, posBeg.y);
    line.addVertex(posEnd.x, posEnd.y);
    
}

void SimpleLine::draw(){
    
    ofNoFill();
    //    ofSetColor(0,0,0);
  
    ofSetLineWidth(3);
  
    line.draw();
   
    
   
}