#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    //ofBackground(0);
    // Kinect
    kinect.init();
    kinect.open();
    // GUI
    gui.setup();
    gui.add(thresh.setup("Threshhold", 103, 0, 255));
    gui.add(minRadius.setup("Min Radius", 40, 0, 400));
    gui.add(maxRadius.setup("Max Radius", 200, 0, 400));
    
    //TRACKER
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(200);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(123);
    showLabels = true;
     positions.resize(10000);

}

//--------------------------------------------------------------
void ofApp::update(){
    contourFinder.setMinAreaRadius(minRadius);//44
    contourFinder.setMaxAreaRadius(maxRadius);
    //    contourFinder.setMinAreaRadius(38);//44
    //    contourFinder.setMaxAreaRadius(130);
    // Kinect
    kinect.update();
    if (kinect.isFrameNew()) {
        // Kinect cv::Mat
        //feed in fbo
        cv::Mat mat = cv::Mat(kinect.height, kinect.width, CV_8UC1, kinect.getDepthPixels(), 0);
        // ofxCv
        contourFinder.setThreshold(thresh);//178.5
        contourFinder.findContours(mat);
        contourFinder.setFindHoles(true);
    }
    
    //check when new blobs appear, check when blobs disappear
    //create strokes for every new blob
    
    const vector<unsigned int>& newLabels = tracker.getNewLabels();
    const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
    const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
    const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
    
    
    // making strokes based off new labels
    for(int i = 0; i < contourFinder.size(); i++) {
        unsigned int label = contourFinder.getLabel(i);
        if(contourFinder.getHole(i)){
            cout << label << " hole area is " <<  contourFinder.getContourArea(i) << endl;
            makeShape(i);
        }
        if(!tracker.existsPrevious(label)){
            bool inAlready = false;
            if(strokes.size() > 0){ //check to see if this label has been added before (the existPrevious doesnt work well
                for(int i=0; i<strokes.size(); i++){
                    if(label == strokes[i].blobNumber){
                        inAlready = true;
                    }
                }
                
                if(inAlready == false){
                    //check if contours are holes
                    //if they are holes than go to the make shape function
                    if(contourFinder.getHole(i)){
                        //                        cout << label << " hole area is " <<  contourFinder.getContourArea(i) << endl;
                        //                        makeShape(i);
                    }else{
                        cout << label << " stroke area is " <<  contourFinder.getContourArea(i) << endl;
                        Stroke newStroke;
                        newStroke.setup(label);
                        strokes.push_back(newStroke);
                    }
                }
            }else if(strokes.size() == 0){
                Stroke newStroke;
                newStroke.setup(label);
                strokes.push_back(newStroke);
            }
            
        }


}

//--------------------------------------------------------------
void ofApp::draw(){

}
    //--------------------------------------------------------------
    void ofApp::makeShape(int _i){
        int i= _i;
        ofPolyline shape = contourFinder.getPolyline(i);
        shape.close();
        shape.draw();
        //holeShapes.push_back(shape.getSmoothed(8));
        
    }
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
