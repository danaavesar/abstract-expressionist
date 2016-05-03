//
//  Stroke.cpp
//  pollock-strokes-kinect-new
//
//  Created by Dana Avesar on 2/16/16.
//
//

#include "Stroke.h"

//--------------------------------------------------------------
void Stroke::setup(int _blobNumber){
    blobNumber = _blobNumber;
//    color.r = ofRandom(0,255);
//    color.g = ofRandom(0,255);
//    color.b = ofRandom(0,255);
//    ofColor red(255, 0, 0);
//    color = red;
    selectedBlack = false;
    selectedRed = false;
    selectedYellow = false;
    selectedWhite = false;
    selectedBlue = false;
    
}

//--------------------------------------------------------------
void Stroke::setColor(ofColor _color){
    color = _color;
    
//    if(color == red){
//        color.r = 255;
//        color.g = 0;
//        color.b = 0;
//    }
}
//--------------------------------------------------------------
void Stroke::draw(){
    ofPushStyle();
    ofSetColor(color);
    
    ofFill();
    for(int i=0; i<circles.size(); i++){
        circles[i].draw();
    }
    
    ofNoFill();
    
    
    for(int i=0; i<weirdos.size(); i++){
        weirdos[i].draw();
    }
    if(lines.size() > 1){
        for(int i=0; i<lines.size()-2; i++){
            ofSetLineWidth(lines[i+1].width/2); //line width dependent on last point in line (determined by how far it is from first point, variable labeled wrong, posEnd is actually beginning point in each line)
            //            cout << lines[i].width << endl;
            ofBeginShape();
            //im actually drawing a bunch of little lines after one another so that i can manipulate the stroke width of each segement deoending on how fast the mouse is moving
            
            ofCurveVertex( lines[i].posEnd.x, lines[i].posEnd.y);
            ofCurveVertex( lines[i].posEnd.x, lines[i].posEnd.y);
            ofCurveVertex( lines[i+1].posEnd.x, lines[i+1].posEnd.y);
            
            ofCurveVertex( lines[i+2].posEnd.x, lines[i+2].posEnd.y);
            
            ofEndShape();
        }
    }
    ofPopStyle();
    
    
}

//--------------------------------------------------------------
void Stroke::loadPoints(int x, int y, int z){
    //find the previous mouseX and the current mouseX
    //get the distance between those points
    //the bigger the distance the smaller the circle
    //divide the distance by constant int
    //im using "z" as the "y" value in 2d space. because the kinect is in the front of the room and the projection is on the ground, so back of the room == y of the 2d space
    center.set(x,y,z);
    float dist  = ofDist(prevX, prevY, x, y); //  float dist  = ofDist(prevX, prevZ, x, z);
    
    float radius;
    if(dist != 0){
        radius = divider/dist;
    }else{
        radius = 0;
    }
    
    
    Line myLine;
    curveProbability = ofRandom(0, 10);         //random probability for adding curvies in the line
    myLine.posEnd.x = x;
    myLine.posEnd.y = y; //myLine.posEnd.y = z;
    if(curveProbability < 2){
        myLine.posEnd.x = ofRandom(x-20, x);
        myLine.posEnd.y = ofRandom(y+20, y); //myLine.posEnd.y = ofRandom(z+20, z);
    }
    myLine.width = radius;
    lines.push_back(myLine);
    
    
    //if the distance is small then start adding circles
    weirdoCircleProbability = ofRandom(0,10);
    
    
    if(weirdoCircleProbability > 1){
        Circle tempCircle;
        tempCircle.setup(x,y, radius); //tempCircle.setup(x,z, radius);
        circles.push_back(tempCircle);
    }else{
        Weirdo myWeirdo;
        myWeirdo.setup(x,y, prevX,prevY, radius); //myWeirdo.setup(x,z, prevX,prevZ, radius);
        weirdos.push_back(myWeirdo);
    }
    
    prevX = x;
    prevZ = z;
    prevY = y;
    prevYValues.push_back(y);
    
    
    
}
