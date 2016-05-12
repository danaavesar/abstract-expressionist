#pragma once

#include "ofMain.h"
#include "ofxKinectFeatures.h"
#include "ofxOpenNI.h"
#include "Stroke-pollock.h"
#include "Stroke-kandinsky.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "SimpleLine.h"
#include "ofxKinect.h"

enum {
    VELOCITY_MEAN,
    ACCELERATION_Y,
    RELPOSTOTORSO_X
};

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    ofColor mixColor(ofColor color1, ofColor color2);
    int clamp(int _value);
    void drawShape(ofPoint _position);
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void userEvent(ofxOpenNIUserEvent & event);
    void mocapMax(MocapMaxEvent &e);
   // map<int,ofxKinectFeatures> featExtractors;
    ofxKinectFeatures featExtractor1;
    ofxKinectFeatures featExtractor2;
    ofxKinectFeatures featExtractor3;
    vector <ofxKinectFeatures> featExtractors;
    vector <ofPoint> prevTorsoPos1;
    vector <ofPoint> prevTorsoPos2;
    vector <ofPoint> prevTorsoPos3;
   
    vector<ofxOpenNIUser> prevUsers;
    
    vector<vector<ofPoint>> shapes;
    //vector<vector<ofPoint>> prevTorsoPos;
    vector <ofPoint> prevTorsoPos[3] = {prevTorsoPos1, prevTorsoPos2, prevTorsoPos3};
    ofxOpenNI kinect;
    bool hadUsers;
   // ofxKinectFeatures featExtractor;
    int j, f;
    int leftHip, rightHip;
    int leftShoulder, rightShoulder;
    int leftHand;
    int torso;
    int loop; //for counting when to multiply the num in delete prevTorsoArray
    ofPoint prevLeftHandPosTorso;
    vector <int> handJointNames = {JOINT_LEFT_HAND, JOINT_RIGHT_HAND};
    map <int,ofPoint> handJointProjectedPos; //ints representing the joints
    map <int,ofPoint> prevHandJointProjectedPos;
    ofPoint prevLeftHipPos[3];
    ofTrueTypeFont font;
    
    //painting
    vector <Stroke> pollockStrokes;
    vector <Stroke2> spirals;
    vector <SimpleLine> simpleLines;
    //color selection variables
    ofImage canWhite;
    ofImage canBlack;
    ofImage canYellow;
    ofImage canRed;
    ofImage canBlue;
    ofVideoPlayer whiteCanSplash;
    ofVideoPlayer blackCanSplash;
    ofVideoPlayer yellowCanSplash;
    ofVideoPlayer redCanSplash;
    ofVideoPlayer blueCanSplash;
    
    ofPoint blackPos;
    ofPoint whitePos;
    ofPoint redPos;
    ofPoint bluePos;
    ofPoint yellowPos;
    bool colorSelected;
    bool isInside;
    bool selectedBlack;
    int boxSize;
    int y;
    int x;
    
    //contourFInder
    ofxCv::ContourFinder contourFinder;
    ofxCv::RectTracker tracker;
     bool showLabels;
    vector <int> lastHoleLabels;
    int frameNumber;
    int timer[3];
    int timerForJump[3]; 
   map<int,ofPolyline> holesPolylines;
    vector <ofPolyline> createdShapes;
    int counter;
    int numJumps[3]; //count for jumps for each user
    // GUI
    ofxPanel gui;
    ofxFloatSlider thresh;
    ofxFloatSlider minRadius;
    ofxFloatSlider maxRadius;
    
   
    //right hand
    //int rightHandVelocity;
    //ofPoint rightHandPosition;
//    Boolean pollock, prevPollock;
};
