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
    
//    background.setDifferenceMode(background.ABSDIFF);
//    background.setIgnoreForeground(false);
//    background.setLearningTime(80);
//    background.setThresholdValue(50);
    positions.resize(10000);
    strokeSize = 0;
    tilt = 10;
    kinect.setCameraTiltAngle(tilt);
    
    boxSize = 70;
    y = ofGetWindowHeight()-30 - boxSize;
    int buffer = 100;
    int widthPallet = ofGetWindowWidth()-buffer;
    int spaceBetween = (widthPallet-(boxSize*5))/5+1;
    blackPos.set(ofGetWindowWidth() - buffer - boxSize, y);
    whitePos.set(blackPos.x - spaceBetween - boxSize, y);
    redPos.set(whitePos.x - spaceBetween - boxSize, y);;
    bluePos.set(redPos.x - spaceBetween - boxSize, y);;
    yellowPos.set(bluePos.x - spaceBetween - boxSize, y);;
    
    isInside = false;
    selectedBlack = false;
    colorSelected = false;
    newLabelsSize = 0;
    deadLabelsSize = 0;
    frameNumber = 0;
    
 
    canWhite.load("can-white.png");
    canBlack.load("can-black.png");
    canYellow.load("can-yellow.png");
    canRed.load("can-red.png");
    canBlue.load("can-blue.png");
    
    //syphon
    ofSetWindowTitle("ofxSyphon Example");
    
    mainOutputSyphonServer.setName("Screen Output");
    individualTextureSyphonServer.setName("Texture Output");
    
    mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("","Simple Server");
    tex.allocate(200, 100, GL_RGBA);

}

//--------------------------------------------------------------
void ofApp::update(){
    //fbobegin
    //kinect.drawdepth(0,0)
    //kinect2.drawdepth(width,0)
    //fbo.end()
    // cout << "new frame" << endl;
    frameNumber ++;
    //previously update
    // CV
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
    
    //load the points into the strokes
    if(strokes.size()>0 ){
        for(int i = 0; i < contourFinder.size(); i++) {
            int label = contourFinder.getLabel(i); //check which label == which stroke
            for( int j=0; j<strokes.size(); j++ ) { //loop through all strokes
                
                if(label == strokes[j].blobNumber){
                    ofPoint center = ofxCv::toOf(contourFinder.getCenter(i));
                    //for the y we need the depth (or z axis in the kinect) and map z to the y axis
                    int z = kinect.getDistanceAt(center.x,center.y);
                    int zMap = ofMap(z, 0, 4000, 768, 0);
                    ofPoint blobCenter;
                    blobCenter.x = ofMap(center.x,0, kinect.width, 0, ofGetWidth());
                    blobCenter.y = ofMap(center.y,0, kinect.width, 0, ofGetWidth());
                    blobCenter.z =  zMap;

                    //only if not in the paintbucket place
                    if(blobCenter.y < y - 60){
                        strokes[j].loadPoints(blobCenter.x,blobCenter.y, blobCenter.z);
                    }
                    
                    ////black
                    if((blobCenter.x> blackPos.x) && (blobCenter.x < blackPos.x + boxSize)&& (blobCenter.y > blackPos.y) && (blobCenter.y < blackPos.y + boxSize)){
                        
                        if(strokes[j].selectedBlack == false){
                            colorSelected = true;
                            strokes[j].selectedBlack = true;
                            ofColor black(0, 0, 0);
                            Stroke newStroke; //new stroke with this color, but same lable(blob num)
                            newStroke.setup(label);
                            newStroke.setColor(black);
                            newStroke.selectedBlack = true;
                            strokes[j].blobNumber = -1 * strokes[j].blobNumber; //change the blobnumber to not match tracker label
                            if(strokes[j].blobNumber == 0){ //incase its 0 subtract one
                                strokes[j].blobNumber = strokes[j].blobNumber -1;
                            }
//                            cout <<  strokes[j].blobNumber << " " << label << endl;
                            strokes.push_back(newStroke);
                            cout << label << " selected black!!!!" << endl;
                        }
                    }else{
                        
                        strokes[j].selectedBlack = false;
                    }
                    ////white
                    if((blobCenter.x> whitePos.x) && (blobCenter.x < whitePos.x + boxSize) && (blobCenter.y > whitePos.y) && (blobCenter.y < whitePos.y + boxSize)){
                        
                        if(strokes[j].selectedWhite == false){
                            colorSelected = true;
                            strokes[j].selectedWhite = true;
                            ofColor white(255, 255, 255);
                            Stroke newStroke; //new stroke with this color, but same lable(blob num)
                            newStroke.setup(label);
                            newStroke.setColor(white);
                            newStroke.selectedWhite = true;
                            strokes[j].blobNumber = -1 * strokes[j].blobNumber; //change the blobnumber to not match tracker label
                            if(strokes[j].blobNumber == 0){ //incase its 0 subtract one
                                strokes[j].blobNumber = strokes[j].blobNumber -1;
                            }
                            //                            cout <<  strokes[j].blobNumber << " " << label << endl;
                            strokes.push_back(newStroke);
                            cout << label << " selected White!!!!" << endl;
                        }
                    }else{
                        
                        strokes[j].selectedWhite = false;
                    }
                    //yellow
                    if((blobCenter.x> yellowPos.x) && (blobCenter.x < yellowPos.x + boxSize) && (blobCenter.y > yellowPos.y) && (blobCenter.y < yellowPos.y + boxSize)){
                        
                        if(strokes[j].selectedYellow == false){
                            colorSelected = true;
                            strokes[j].selectedYellow = true;
                            ofColor yellow(255, 255, 0);
                            Stroke newStroke; //new stroke with this color, but same lable(blob num)
                            newStroke.setup(label);
                            newStroke.setColor(yellow);
                            newStroke.selectedYellow = true;
                            strokes[j].blobNumber = -1 * strokes[j].blobNumber; //change the blobnumber to not match tracker label
                            if(strokes[j].blobNumber == 0){ //incase its 0 subtract one
                                strokes[j].blobNumber = strokes[j].blobNumber -1;
                            }
                            //                            cout <<  strokes[j].blobNumber << " " << label << endl;
                            strokes.push_back(newStroke);
                            cout << label <<" selected Yellow!!!! " << newStroke.color << endl;
                        }
                       
                    }else{
                        
                        strokes[j].selectedYellow = false;
                    }
                    //blue
                    if((blobCenter.x> bluePos.x) && (blobCenter.x < bluePos.x + boxSize) && (blobCenter.y > bluePos.y) && (blobCenter.y < bluePos.y + boxSize)){
                        
                        if(strokes[j].selectedBlue == false){
                            colorSelected = true;
                            strokes[j].selectedBlue = true;
                            ofColor blue(0, 0, 255);
                            Stroke newStroke; //new stroke with this color, but same lable(blob num)
                            newStroke.setup(label);
                            newStroke.setColor(blue);
                            newStroke.selectedBlue = true;
                            strokes[j].blobNumber = -1 * strokes[j].blobNumber; //change the blobnumber to not match tracker label
                            if(strokes[j].blobNumber == 0){ //incase its 0 subtract one
                                strokes[j].blobNumber = strokes[j].blobNumber -1;
                            }
                            //                            cout <<  strokes[j].blobNumber << " " << label << endl;
                            strokes.push_back(newStroke);
                            cout << label << " selected blue!!!!" << endl;
                        }
                    }else{
                        
                        strokes[j].selectedBlue = false;
                    }
                    //red
                    if((blobCenter.x> redPos.x) && (blobCenter.x < redPos.x + boxSize) && (blobCenter.y > redPos.y) && (blobCenter.y < redPos.y + boxSize)){
                        
                        if(strokes[j].selectedRed == false){
                            colorSelected = true;
                            strokes[j].selectedRed = true;
                            
                            Stroke newStroke; //new stroke with this color, but same lable(blob num)
                            newStroke.setup(label);
                            ofColor red(255, 0, 0);
                            newStroke.setColor(red);
                            newStroke.selectedRed = true;
                            strokes[j].blobNumber = -1 * strokes[j].blobNumber; //change the blobnumber to not match tracker label
                            if(strokes[j].blobNumber == 0){ //incase its 0 subtract one
                                strokes[j].blobNumber = strokes[j].blobNumber -1;
                            }
                            //                            cout <<  strokes[j].blobNumber << " " << label << endl;
                            strokes.push_back(newStroke);
                            cout << label<< " selected red!!!!" << endl;
                            cout << label << " color " << newStroke.color << endl;
                        }
                    }else{
                        
                        strokes[j].selectedRed = false;
                    }



                }//end if label == strokes[j].blobNumber
            }//end the stroke loop
        }// end contourFinderloop
    }// end of if strokes>0
    
    //////check if blobs have merged
    
    //for each new label in this frame

    
//    for(int i=0; i <currentLabels.size();i++){
//        if(
//       
//   }
    
    newlySeenLabels.clear();
 
    //check if any of the currentLabels was dead inthe last frame
    //cout << "size " << wasJustDeadBefore.size() << endl;
    for(int j=0; j< wasJustDeadBefore.size(); j++){
        for(int i=0; i< currentLabels.size(); i++){
            if(wasJustDeadBefore[j] == currentLabels[i]){
                newlySeenLabels.push_back(wasJustDeadBefore[j]);
//                cout << currentLabels[i] << endl;
            }
        }
    }
  
    
    //now we need to check merge for newLabels
    for(int i = 0; i < newLabels.size(); i++){
        int NewLabel = newLabels[i];
        //check if there are at least two that died in this frame
        if(deadLabels.size() >= 2){
            vector <double> deadBlobAreas;
            deadBlobAreas.clear();
            double newBlobArea;
            //get the area of the new label, loop through blobs to match blob with the label
            for(int c = 0; c < contourFinder.size(); c++) {
                int label = contourFinder.getLabel(c);
                if (NewLabel == label){ //this is the right blob with matching new label
                    newBlobArea = contourFinder.getContourArea(c);
                    //cout << label <<" newBlobArea " << newBlobArea << endl;
                }
            }
            
            //            //get each dead label
            //            cout << "lastLabels: " << endl;
            //            for(int p=0; p<lastLabels.size();p++){
            //                cout << lastLabels[p] << endl;
            //            }
            for(int d=0; d<deadLabels.size(); d++){
                int DeadLabel = deadLabels[d];
                bool found = false;
                //cout << "DeadLabel " << DeadLabel << endl;
                //get the area of the dead labels stored in the previousLabelsArea vector
                vector <int> repeats;
                for(int p=0; p<lastLabels.size();p++){
                    //                    cout << "prevLabels " << prevLabels[p] << endl;
                    if(DeadLabel == lastLabels[p]){
                        repeats.push_back(p); //pushing the number it is in the array
                        //cout << DeadLabel << " found in prevLabels" << endl;
                        found = true;
                    }
                }
                
                if(found == true){
                    int lastOne = repeats.back();
                    double deadBlobArea =  prevLabelAreas[lastOne];
//                    cout << "DeadLabel " << DeadLabel << " area " << deadBlobArea << endl;
                    deadBlobAreas.push_back(deadBlobArea);
                }
                
                
            }
            
            
            //does area of newLabel == areaof two deadlabels?
            for(int j = 0; j< deadBlobAreas.size(); j++){
                //cout << "deadBlobArea " << deadBlobAreas[j] << endl;
                for(int f = j+1; f< deadBlobAreas.size(); f++){
                    
                    if((deadBlobAreas[j] + deadBlobAreas[f]) >= (newBlobArea -800) && (deadBlobAreas[j] + deadBlobAreas[f]) <= (newBlobArea + 800)){
                        //cout << deadLabels[j] << " and " <<deadLabels[f]<< " merged!!!!" << endl;
                        merged(deadLabels[j] ,deadLabels[f]);
                    }
                    
                }
            }
            
            
        }//end if(deadLabels.size() >= 2)
    }//end for newLabels
    
    //we also need to check merge for newlyEmergedLabels that used to be dead
    for(int i = 0; i < newlySeenLabels.size(); i++){
        int NewLabel = newlySeenLabels[i];
        //check if there are at least two that died in this frame
        if(deadLabels.size() >= 2){
            vector <double> deadBlobAreas;
            deadBlobAreas.clear();
            double newBlobArea;
            //get the area of the new label, loop through blobs to match blob with the label
            for(int c = 0; c < contourFinder.size(); c++) {
                int label = contourFinder.getLabel(c);
                if (NewLabel == label){ //this is the right blob with matching new label
                    newBlobArea = contourFinder.getContourArea(c);
//                    cout << label <<" newBlobArea " << newBlobArea << endl;
                }
            }
            
//            //get each dead label
//            cout << "lastLabels: " << endl;
//            for(int p=0; p<lastLabels.size();p++){
//                cout << lastLabels[p] << endl;
//            }
            for(int d=0; d<deadLabels.size(); d++){
                int DeadLabel = deadLabels[d];
                bool found = false;
                //cout << "DeadLabel " << DeadLabel << endl;
                    //get the area of the dead labels stored in the previousLabelsArea vector
                vector <int> repeats;
                for(int p=0; p<lastLabels.size();p++){
//                    cout << "prevLabels " << prevLabels[p] << endl;
                    if(DeadLabel == lastLabels[p]){
                        repeats.push_back(p); //pushing the number it is in the array
                        //cout << DeadLabel << " found in prevLabels" << endl;
                        found = true;
                    }
                }
                
                if(found == true){
                    int lastOne = repeats.back();
                    double deadBlobArea =  prevLabelAreas[lastOne];
//                   cout << "DeadLabel " << DeadLabel << " area " << deadBlobArea << endl;
                    deadBlobAreas.push_back(deadBlobArea);
                }


            }
            
            
            //does area of newLabel == areaof two deadlabels?
            for(int j = 0; j< deadBlobAreas.size(); j++){
                //cout << "deadBlobArea " << deadBlobAreas[j] << endl;
                for(int f = j+1; f< deadBlobAreas.size(); f++){
                   
                        if((deadBlobAreas[j] + deadBlobAreas[f]) >= (newBlobArea -800) && (deadBlobAreas[j] + deadBlobAreas[f]) <= (newBlobArea + 800)){
                            //cout << deadLabels[j] << " and " <<deadLabels[f]<< " merged!!!!" << endl;
                            merged(deadLabels[j] ,deadLabels[f]);
                        }
                    
                }
            }
            
            
        }//end if(deadLabels.size() >= 2)
    }//end for newlySeenLabels
    
  
    


    //has died vector, log which ones have died before
    wasJustDeadBefore.clear();
    for(int i=0; i < deadLabels.size(); i++){
        wasJustDeadBefore.push_back(deadLabels[i]);
    }
    
    if(frameNumber%10 == 0){
        lastLabels.clear();
        prevLabelAreas.clear();
    }
    // cout << "# Current Labels " << currentLabels.size() << endl;
    // cout <<"# Contours " <<  contourFinder.size() << endl;
    //cout << "contourFinder labels" << endl;
        for(int c=0; c<contourFinder.size(); c++){
            int label = contourFinder.getLabel(c);
    //        cout << label << endl;
            double area= contourFinder.getContourArea(c);
            
            prevLabelAreas.push_back(area);
            lastLabels.push_back(label);
            // cout << "label " << label << " area " << area << endl;
            // cout << "currentLabel " << currentLabels[i] << endl;
            
            // int label = contourFinder.getLabel(c);
            //if(currentLabels[i] == label){
            //cout << "label " << label << endl;
            
            //prevLabelAreas.push_back( contourFinder.getContourArea(c));
            //}
            //}
        }
    
    //cout << "currentLabels labels" << endl;
    //for(int i=0; i< currentLabels.size(); i++){
        //int label = currentLabels[i];
       // cout << currentLabels[i] << endl;
        
    //}
    

   
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
void ofApp::merged(int label1, int label2){

    //color variables
    ofColor newColor;
    ofColor label1Color;
    ofColor label2Color;
    
    ofColor white(255,255,255);
    ofColor red(255,0,0);
    ofColor blue(0,0,255);
    ofColor yellow(255, 255, 0);
    ofColor black(0,0,0);
    //temp
    ofColor purple(161, 0, 186);
    ofColor orange(227, 108, 39);
    ofColor green(57, 187, 39);
    
    
    float hue = 0;  // The hue value to set.
    float saturation = 0; // The saturation value to set.
    float brightness = 0; // The brightness value to set.
    
    for(int i=0; i< strokes.size(); i++){
        if(strokes[i].blobNumber == label1){
            label1Color = strokes[i].color;
            //cout << label1 << " has a color of " << label1Color << endl;
           
        }
        if(strokes[i].blobNumber == label2){
            label2Color = strokes[i].color;
            //cout << label2 << " has a color of " << label2Color << endl;

        }
    }
    //white
    if(label1Color == white){
        label2Color.getHsb(hue, saturation, brightness);
        label2Color.setHsb(hue, saturation-200, brightness);
        newColor = label2Color;
    }
    if(label2Color == white){
        label1Color.getHsb(hue, saturation, brightness);
        label1Color.setHsb(hue, saturation-200, brightness);
        newColor = label1Color;
    }
    //black
    if(label1Color == black){
        label2Color.getHsb(hue, saturation, brightness);
        label2Color.setHsb(hue, saturation, brightness-100);
        newColor = label2Color;
    }
    if(label2Color == black){
        label1Color.getHsb(hue, saturation, brightness);
        label1Color.setHsb(hue, saturation-80, brightness-100);
        newColor = label1Color;
    }
    //red for now!!! i need to change this to suntractive color mizing substitution
    if(label1Color == red || label2Color == red){
        if(label1Color == blue || label2Color == blue){
            newColor = purple;
        }
        if(label1Color == yellow || label2Color == yellow){
            newColor = orange;
        }
    }
    //yellow for now!!! i need to change this to suntractive color mizing substitution
    if(label1Color == yellow || label2Color == yellow){
        if(label1Color == blue || label2Color == blue){
            newColor = green;
        }
        if(label1Color == red || label2Color == red){
            newColor = orange;
        }
    }
    //blue for now!!! i need to change this to suntractive color mizing substitution
//    if(label1Color == blue || label2Color == blue){
//        if(label1Color == blue || label2Color == blue){
//            newColor = green;
//        }
//        if(label1Color == red || label2Color == red){
//            newColor = orange;
//        }
//    }
    
 

//    //make two new strokes
    Stroke newStroke; //new stroke with this color, but same lable(blob num)
    Stroke newStroke2; //new stroke with this color, but same lable(blob num)
    //set them up with the same blobNumbers as the previous blobs
    newStroke.setup(label1);
    newStroke2.setup(label2);
    newStroke.setColor(newColor);
    newStroke2.setColor(newColor);
   // cout << "now the color is " <<newColor << endl;
    for(int i=0; i< strokes.size(); i++){ //kill the current strokes for these blobs by changing their num
        if(label1 == strokes[i].blobNumber || label2 == strokes[i].blobNumber){
            strokes[i].blobNumber = -1 * strokes[i].blobNumber; //change the blobnumber to not match tracker label
            strokes[i].blobNumber = -1 * strokes[i].blobNumber; //change the blobnumber to not match tracker label
            if(strokes[i].blobNumber == 0){ //incase its 0 subtract one
                strokes[i].blobNumber = strokes[i].blobNumber -1;
            }
        }
    }
    
    strokes.push_back(newStroke);
    strokes.push_back(newStroke2);
}
//--------------------------------------------------------------
void ofApp::draw(){
  //syphon
    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //--------------------------------------------------------------------
    ofSetColor(255);
   kinect.drawDepth(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 0);
    ofPushMatrix();
    ofScale(ofGetWidth() / float(kinect.width), ofGetHeight() / float(kinect.height));
   contourFinder.draw();
    ofPopMatrix();
    // GUI
    gui.draw();
    
    ///////color selectors
//    ofPushStyle();
//    ofSetColor(0,0,0);
//    ofDrawRectangle(blackPos.x, blackPos.y, boxSize, boxSize);
//    ofSetColor(255, 255, 255);
//    ofDrawRectangle(whitePos.x, whitePos.y, boxSize, boxSize);
//    ofSetColor(255, 0, 0);
//    ofDrawRectangle(redPos.x, redPos.y, boxSize, boxSize);
//    ofSetColor(0, 0, 255);
//    ofDrawRectangle(bluePos.x, bluePos.y, boxSize, boxSize);
//    ofSetColor(255, 255, 0);
//    ofDrawRectangle(yellowPos.x, yellowPos.y, boxSize, boxSize);
//    ofPopStyle();
    ofPushStyle();
    ofSetColor(255);
    canWhite.draw(whitePos.x, whitePos.y, boxSize, boxSize);
    canBlack.draw(blackPos.x, blackPos.y, boxSize, boxSize);
    canYellow.draw(yellowPos.x, yellowPos.y, boxSize, boxSize);
    canRed.draw(redPos.x, redPos.y, boxSize, boxSize);
    canBlue.draw(bluePos.x, bluePos.y, boxSize, boxSize);
    ofPopStyle();

    
    
    //----strokes

    //draw the points
    ofPushMatrix();
    //ofScale(1.6 , 1.6, 0.0 );
   // ofScale(ofGetWidth() / float(kinect.width), ofGetHeight() / float(kinect.height));
    
     
    for( int i=0; i<strokes.size(); i++ ) {
       strokes[i].draw();
        ofSetHexColor(0x00FFFF);
        //ofDrawCircle(strokes[i].center.x, strokes[i].center.z, 5, 5);
        //ofSetHexColor(0xff00ff);
        //ofDrawCircle(strokes[i].center.x, strokes[i].center.y, 5, 5);
        //////????????????????
        //int prevY = strokes[i].prevYValues[strokes[i].prevYValues.size() -1];
        //cout << "centerY " << strokes[i].center.y << " prevY " << prevY << endl;
//        if(strokes[i].center.y < prevY){
//            cout << "jumped" << endl;
//        }
        
    }
    

    ofPopMatrix();
    //TRACKER -------------------------------------
    tracker = contourFinder.getTracker();
    ofPushMatrix();
    ofScale(ofGetWidth() / float(kinect.width), ofGetHeight() / float(kinect.height));
    for(int i = 0; i < contourFinder.size(); i++) {
        ofPoint center = ofxCv::toOf(contourFinder.getCenter(i)); //center of blobs?
        ofPushMatrix();
        ofTranslate(center.x, center.y);
        int label = contourFinder.getLabel(i);
        string msg = ofToString(label); //label is the id i guess
        ofDrawBitmapStringHighlight(msg, 0, 0);
        ofVec2f velocity = ofxCv::toOf(contourFinder.getVelocity(i));
        ofScale(5, 5);
        // ofDrawLine(0, 0, velocity.x, velocity.y);
        ofPopMatrix();
        
    }
    ofPopMatrix();
    
    
    ofPushStyle();
    ofSetColor(255,0,0);

    //if(strokes.size() > strokeSize){
        //for(int i= strokeSize; i< strokes.size(); i++){
        for(int i= 0; i< strokes.size(); i++){
            //ofDrawBitmapString(strokes[i].blobNumber, 20 + i*20, 652);
        }
   // }
    
    ofPopStyle();

    strokeSize = strokes.size();
    
    //drawing the shapes
    
    ofSetColor(255, 0, 0);
    ofPushMatrix();
    ofScale(ofGetWidth() / float(kinect.width), ofGetHeight() / float(kinect.height));
    for(unsigned int i = 0; i < holeShapes.size(); i++) {
        
        ofSetColor(255, 0, 0);
        ofFill();
//        ofBeginShape();
//            ofVertex(holeShapes[i].getVertex(0));
//            ofBez
//            holeShapes[i].getVertices();
//        ofEndShape();
        holeShapes[i].draw();
//    ofSetColor(0,255,0); //green
//    ofFill();
//    ofBeginShape();
//    vector<ofPoint> holeShapes[i].getVertices();
//    
//    
//    ofEndShape();
    }
    ofPopMatrix();
    //syphon
    
    mClient.draw(50, 50);
    
    mainOutputSyphonServer.publishScreen();
    
    individualTextureSyphonServer.publishTexture(&tex);
    ofPushMatrix();
    ofScale(ofGetWidth() / float(kinect.width), ofGetHeight() / float(kinect.height));
    ofFill();
    //shapes
    for(int i = 0; i < contourFinder.size(); i++) {
        unsigned int label = contourFinder.getLabel(i);
        if(contourFinder.getHole(i)){
            cout << label << " hole area is " <<  contourFinder.getContourArea(i) << endl;
            ofPolyline shape = contourFinder.getPolyline(i);
          
            shape.getSmoothed(10);
              shape.close();
            shape.draw();
            vector <ofPoint> curveVertices;
            curveVertices = shape.getVertices();
            ofBeginShape();
            for(int i=0; i<curveVertices.size(); i++){
                if(i ==0){
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y);
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y);
                }else if(i == curveVertices.size()-1){
                    ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
                }else{
                    ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
                }
            }
            ofEndShape();
        }
    }
    ofPopMatrix();
    
    
}

//--------------------------------------------------------------
void ofApp::selectColor(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
case OF_KEY_UP:
            
    tilt++;
    if(tilt>30) tilt=30;
    kinect.setCameraTiltAngle(tilt);
    break;
    
case OF_KEY_DOWN:
    tilt--;
    if(tilt<-30) tilt=-30;
    kinect.setCameraTiltAngle(tilt);
    break;
}
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}