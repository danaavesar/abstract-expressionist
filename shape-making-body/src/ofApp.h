#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxCv.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
void makeShape(int i);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
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
    
    vector<int> strokeNumbers;
    vector<int> newlySeenLabels;
    vector<int> wasJustDeadBefore;
    vector<ofPolyline> holeShapes;
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
    
};
