#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "Stroke.h"
#include "ofxSyphon.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void selectColor();
    void merged(int label1, int label2);
    void makeShape(int i);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxKinect kinect;       //Kinect
    ofImage kinectImage;    // Kinect
    ofImage depthImage;     // Kinect
    // GUI
    ofxPanel gui;
    ofxFloatSlider thresh;
    ofxFloatSlider minRadius;
    ofxFloatSlider maxRadius;
    
    
    //blob tracking
    float threshold;
    ofxCv::ContourFinder contourFinder;
    bool showLabels;
    
    ofxCv::RunningBackground background;
   	ofImage thresholded;
    
    vector<ofPolyline> positions;
    vector<int> strokeNumbers;
    vector<int> newlySeenLabels;
    vector<int> wasJustDeadBefore;
    vector<ofPolyline> holeShapes;
    
    cv::Mat thresholded_mat;
    
    ofxCv::RectTracker tracker;
    ofImage canWhite;
    ofImage canBlack;
    ofImage canYellow;
    ofImage canRed;
    ofImage canBlue;
    
    //painting
    vector <Stroke> strokes;
//    vector <unsigned int> & newLabels;
    vector <double> prevLabelAreas;
    vector <int> lastLabels;
    int prevNumBlobs;
   // const vector<unsigned int>& prevDeadLabels;
    stringstream data;
    int strokeSize;
    int tilt;
    
    //color selection variables
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

    
    int newLabelsSize;
    int deadLabelsSize;
    
    int frameNumber;
    
    ////syphon
    
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    
    ofxSyphonClient mClient;
    ofTexture tex;
    
};