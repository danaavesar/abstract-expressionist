#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
   // thisFbo.allocate(1280,720,GL_RGB);
    kinect.setup();
    kinect.addDepthGenerator();
    kinect.addImageGenerator();
    kinect.setRegister(false);
    kinect.setMirror(true);
    kinect.addUserGenerator();
    kinect.setMaxNumUsers(3);
    kinect.setSkeletonProfile(XN_SKEL_PROFILE_ALL);
    kinect.start();
    hadUsers = false;
    frameNumber = 0;
    
    counter = 0;
    loop = 1;
    for(int i =0; i<3; i++){
        numJumps[i] = 0;
        timerForJump[i] = 0; //this is to detect double jump within 80 frames
        timer[i] = 0; //this one is also for jumping
    }
    //////ofxkinect
    
    boxSize = 113;
//    y = ofGetHeight()-60 - boxSize;
    x = ofGetWidth()-100-boxSize;
    int buffer = 0;
    int heightPallet = ofGetHeight() - buffer;
    int spaceBetween = (heightPallet - (boxSize*5))/5+1;
    blackPos.set(x, ofGetHeight() - buffer - boxSize);
    whitePos.set(x, blackPos.y - spaceBetween - boxSize);
    redPos.set(x, whitePos.y - spaceBetween - boxSize);
    bluePos.set(x, redPos.y - spaceBetween - boxSize);
    yellowPos.set(x, bluePos.y - spaceBetween - boxSize);

    canWhite.load("can-white.png");
    canBlack.load("can-black.png");
    canYellow.load("can-yellow.png");
    canRed.load("can-red.png");
    canBlue.load("can-blue.png");
    featExtractors.push_back(featExtractor1);
    featExtractors.push_back(featExtractor2);
    featExtractors.push_back(featExtractor3);
    
   // prevTorsoPos = {prevTorsoPos1, prevTorsoPos2, prevTorsoPos3};
    
    for(int i=0; i<3; i++){
        featExtractors[i].mixColor = false;
    }
    
    featExtractor1.setup(JOINT_HEAD, JOINT_TORSO);
featExtractor2.setup(JOINT_HEAD, JOINT_TORSO);
   featExtractor3.setup(JOINT_HEAD, JOINT_TORSO);
    
    ofAddListener(kinect.userEvent, this, &ofApp::userEvent);
    ofAddListener(MocapMaxEvent::events, this, &ofApp::mocapMax);
    
    //ofSetWindowShape(1920, 1080);
    
    font.loadFont("verdana.ttf", 24);
    
    j = JOINT_RIGHT_HAND;
    leftHand = JOINT_LEFT_HAND;
    leftHip = JOINT_LEFT_HIP;
    rightHip = JOINT_RIGHT_HIP;
    rightShoulder = JOINT_RIGHT_SHOULDER;
    leftShoulder = JOINT_LEFT_SHOULDER;
    torso = JOINT_TORSO;
    f = VELOCITY_MEAN;
   

    // GUI
    gui.setup();
    gui.add(thresh.setup("Threshhold", 103, 0, 255));
    gui.add(minRadius.setup("Min Radius", 40, 0, 400));
    gui.add(maxRadius.setup("Max Radius", 200, 0, 400));

    
    //Tracker
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(200);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(123);
    showLabels = true;
    ofSetVerticalSync(true);
     whiteCanSplash.setPixelFormat(OF_PIXELS_RGBA);
    blackCanSplash.setPixelFormat(OF_PIXELS_RGBA);
    yellowCanSplash.setPixelFormat(OF_PIXELS_RGBA);
    redCanSplash.setPixelFormat(OF_PIXELS_RGBA);
    blueCanSplash.setPixelFormat(OF_PIXELS_RGBA);
   
    
    whiteCanSplash.load("paint-splash-white.mov");
    blackCanSplash.load("paint-splash-black.mov");
    yellowCanSplash.load("paint-splash-yellow.mov");
    redCanSplash.load("paint-splash-red.mov");
    blueCanSplash.load("paint-splash-blue.mov");

    whiteCanSplash.setLoopState(OF_LOOP_NONE);
    blackCanSplash.setLoopState(OF_LOOP_NONE);
    blueCanSplash.setLoopState(OF_LOOP_NONE);
    redCanSplash.setLoopState(OF_LOOP_NONE);
    yellowCanSplash.setLoopState(OF_LOOP_NONE);
   

}

//--------------------------------------------------------------
void ofApp::update(){

    kinect.update();
    frameNumber ++;
    for(int i=0; i<3; i++){
        timer[i] ++;
    }
    whiteCanSplash.update();
    blackCanSplash.update();
    yellowCanSplash.update();
    redCanSplash.update();
    blueCanSplash.update();
    //---------contourFInder tracker
//    if(kinect.isNewFrame()){
//        contourFinder.setMinAreaRadius(minRadius);//44
//        contourFinder.setMaxAreaRadius(maxRadius);
//         cv::Mat mat = cv::Mat(kinect.getHeight(), kinect.getWidth(), CV_8UC1, kinect.getDepthPixels().getChannel(0), 0);//
//        contourFinder.setThreshold(thresh);//178.5
//        contourFinder.findContours(mat);
//        contourFinder.setFindHoles(true);
//    }
//    tracker = contourFinder.getTracker();
    
  

    
    //This is a trick to reset the user generator if all users are lost
    if (kinect.getNumTrackedUsers()) {
        hadUsers = true;
    } else if (!kinect.getNumTrackedUsers() && hadUsers){
        hadUsers = false;
        kinect.setPaused(true);
        kinect.removeUserGenerator();
        kinect.addUserGenerator();
        kinect.setPaused(false);
    }
    
    
    //update prevusers
   // if(kinect.getNumTrackedUsers() > 0){
    prevUsers.clear();
    for(int i=0; i<kinect.getNumTrackedUsers(); i++){
         ofxOpenNIUser user = kinect.getTrackedUser(i);
        
        prevUsers.push_back(user);
        
    }
    //----------------------------------------------
    if(kinect.getNumTrackedUsers() >0){
        for (int i = 0; i < kinect.getNumTrackedUsers(); i++) {
            ofxOpenNIUser user = kinect.getTrackedUser(i);
            int userId = user.getXnID();
            if(userId == 1){
                if (user.getJoint((Joint)0).getWorldPosition() != ofPoint(0,0,0) &&
                    user.getJoint((Joint)0).getWorldPosition() != featExtractor1.getPosition(0) ) {
                    map<int, ofPoint> joints;
                    for (int j = 0; j < user.getNumJoints(); j++) {
                       
                        joints[j] = user.getJoint((Joint)j).getWorldPosition();
                    }
                    featExtractor1.update(joints);
                }
            }
            if(userId == 2){
                if (user.getJoint((Joint)0).getWorldPosition() != ofPoint(0,0,0) &&
                    user.getJoint((Joint)0).getWorldPosition() != featExtractor2.getPosition(0) ) {
                    map<int, ofPoint> joints;
                    for (int j = 0; j < user.getNumJoints(); j++) {
                        
                        joints[j] = user.getJoint((Joint)j).getWorldPosition();
                    }
                    featExtractor2.update(joints);
                }
            }
            if(userId == 3){
                if (user.getJoint((Joint)0).getWorldPosition() != ofPoint(0,0,0) &&
                    user.getJoint((Joint)0).getWorldPosition() != featExtractor3.getPosition(0) ) {
                    map<int, ofPoint> joints;
                    for (int j = 0; j < user.getNumJoints(); j++) {
                        
                        joints[j] = user.getJoint((Joint)j).getWorldPosition();
                    }
                    featExtractor3.update(joints);
                }
            }
        }//end of for loop
    } //end of if statement
    
//------------------------------- Pollock -----------------------------//
   if(kinect.getNumTrackedUsers() >0){

            //----------------- check if hands are moving fast for Pollock
       float rightHandVelocities[3] = {featExtractor1.getVelocityMagnitude(j), featExtractor2.getVelocityMagnitude(j), featExtractor3.getVelocityMagnitude(j)};
        ofPoint rightHandPositions[3] = {kinect.worldToProjective(featExtractor1.getPosition(j)), kinect.worldToProjective(featExtractor2.getPosition(j)), kinect.worldToProjective(featExtractor3.getPosition(j))};
       for(int i = 0; i<3; i++){
           if(rightHandVelocities[i] > 5){
                featExtractors[i].pollock = true;
                bool splash;
                if(rightHandVelocities[i] > 65){
                    splash = true;
                }else{
                    splash = false;
                }
                for(int s=0; s<pollockStrokes.size(); s++){
                    if(pollockStrokes[s].blobNumber == i+1){ //i starts at 0, user ids start at 1, so 0 in the array corr user id 1
                        pollockStrokes[s].loadPoints(rightHandPositions[i].x, rightHandPositions[i].y, rightHandPositions[i].z, splash);
                    }
                }
            }else{
                featExtractors[i].pollock = false;
            }
            
            if(featExtractors[i].prevPollock == false && featExtractors[i].pollock == true){
                Stroke newPollockStroke;
                
                newPollockStroke.setup(i+1,featExtractors[i].color );
                //newPollockStroke.setColor(featExtractors[i].color);
                //if there is already a stroke with this num don't associate it with this id anymore
                for(int s=0; s<pollockStrokes.size(); s++){
                    if(pollockStrokes[s].blobNumber == i+1){
                        pollockStrokes[s].blobNumber = -pollockStrokes[s].blobNumber;
                    }
                }
                pollockStrokes.push_back(newPollockStroke);
            };

            featExtractors[i].prevPollock = featExtractors[i].pollock;
           
        } //end of for loop featextractors
    }// end of if
    

    //const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
   
    //----------log previous labels, first clear them
    
    if(frameNumber%5 == 0){
        lastHoleLabels.clear();
    }
    
//    for(int c=0; c<contourFinder.size(); c++){
//        if(contourFinder.getHole(c)){
//            int label = contourFinder.getLabel(c);
//            lastHoleLabels.push_back(label);
//        }
//    }
//   
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(ofColor::white);
    float w = ofGetWidth(); //w
    float h = ofGetHeight(); //h
    
 
    ofPushMatrix();
    ofScale(ofGetWidth() / float(kinect.getWidth()), ofGetHeight() / float(kinect.getHeight()));
    kinect.drawImage();
    kinect.drawSkeletons();
   // contourFinder.draw();
    
    
  
   
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
    //------------------------------- shapes -----------------------------//
    //----------drawing the shapes live
    const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
    ofSetColor(255, 0, 0);
    ofFill();
    ofPushMatrix();
    ofScale(ofGetWidth() / float(kinect.getWidth()), ofGetHeight() / float(kinect.getHeight()));
    for(int i = 0; i < contourFinder.size(); i++) {
        unsigned int label = contourFinder.getLabel(i);
        
        if(contourFinder.getHole(i)){
            ofPolyline shape = contourFinder.getPolyline(i);
            ofPolyline smoothedShape = shape.getSmoothed(20);
            holesPolylines[label].clear();
            holesPolylines[label] = smoothedShape;
           // shape.close();
            smoothedShape.draw();
            vector <ofPoint> curveVertices;
            curveVertices = smoothedShape.getVertices();
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
    //----------figure out if the shape has died
    for(int l=0; l<deadLabels.size(); l++){
        int deadLabel = deadLabels[l];
        for(int f = 0; f<lastHoleLabels.size(); f++){
            if(deadLabel == lastHoleLabels[f]){
                //hole died
                createdShapes.push_back(holesPolylines[deadLabel]);
            }
        }
    }
    //----------draw the created shapes
    for(int i =0; i<createdShapes.size(); i++){
        ofBeginShape();
        vector <ofPoint> curveVertices;
        curveVertices = createdShapes[i].getVertices();
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
    gui.draw();
    
    counter ++;

    
//------------------------------- arm lines -----------------------------//
    if(kinect.getNumTrackedUsers() >0){
            for(int s=0; s< handJointNames.size(); s++){ //these joints are just hands
                int  jointName = handJointNames[s];
                handJointProjectedPos[jointName] = kinect.worldToProjective(featExtractor1.getPosition(jointName));
              
                ofPoint shoulderPos = kinect.worldToProjective(featExtractor1.getPosition(jointName-2));
//                float dist = ofDist(jointProjectedPos[jointName].x, jointProjectedPos[jointName].y, prevJointProjectedPos[jointName].x,
                //prevJointProjectedPos[jointName].y);
                ofPoint vel = handJointProjectedPos[jointName] - prevHandJointProjectedPos[jointName];
                
                if(vel.x < 0){
                    vel.x *= -1;
                }
                if(vel.y < 0){
                    vel.y *= -1;
                }
                if(vel.z < 0){
                    vel.z*=-1;
                }
                //font.drawString(ofToString(vel.x ),10, 60);
                if(featExtractor1.lines[jointName] == false){ // each joint in each featExtractor has a bool
                    

                    if(vel.x > 200 || vel.y > 200){
                       //font.drawString(ofToString(vel), handJointProjectedPos[jointName].x, handJointProjectedPos[jointName].y);
                        counter = 0;
                        featExtractor1.lines[jointName] = true;
                        
                        ofSetColor(255, 255, 255);
                        //if 30 frames have passed since the lase rectangle. this is in order to not make repeating shapes when you make a move
                        //font.drawString("new line" ,10, 60);
                        SimpleLine line;
                        line.setup(shoulderPos, handJointProjectedPos[jointName]);
                        simpleLines.push_back(line);
                        
                    }
                }else if(featExtractor1.lines[jointName] == true && counter >= 60){
                
                        featExtractor1.lines[jointName] = false;
                    
                }
                
                if(frameNumber %50 == 0){
                  
                    prevHandJointProjectedPos[jointName] = handJointProjectedPos[jointName];
                }
            }//end loop for jointname (hands)
            
//------------------------------- hip spirals -----------------------------//
        ofPoint leftHipPosition[3] = {kinect.worldToProjective(featExtractor1.getPosition(leftHip)), kinect.worldToProjective(featExtractor2.getPosition(leftHip)), kinect.worldToProjective(featExtractor3.getPosition(leftHip))};
        ofPoint leftHipPosTorso[3] = {featExtractor1.getRelativePositionToTorso(leftHip), featExtractor2.getRelativePositionToTorso(leftHip), featExtractor3.getRelativePositionToTorso(leftHip)};
        ofPoint leftHipVelocity[3] = {featExtractor1.getVelocityMean(leftHip),featExtractor2.getVelocityMean(leftHip), featExtractor3.getVelocityMean(leftHip)} ;
        for(int i=0; i< 3; i++){
            //int changeHipY = leftHipPosition.y - prevLeftHipPos.y;
            ofPoint changeLeftHip = leftHipPosTorso[i] - prevLeftHipPos[i];
            if(changeLeftHip.z < 0){ changeLeftHip.z *= -1;};
            if(changeLeftHip.y < 0){ changeLeftHip.y *= -1;};
            if(changeLeftHip.x < 0){ changeLeftHip.x *= -1;};
           
            //font.drawString(ofToString(leftHipPosTorso ),10, 60);
            //font.drawString(ofToString(changeLeftHip ),10, 90);
          
             if(frameNumber %10 == 0){
            if(changeLeftHip.x  > .05  && featExtractors[i].spiral == false  ){
                ofSetColor(255, 0, 0);
             
                 ofDrawCircle(10, 10, 50);
                featExtractors[i].spiral = true;
                Stroke2 newSpiral;
                newSpiral.setup(featExtractors[i].color, i+1);
                spirals.push_back(newSpiral);
                
            }
            }
            ofSetColor(255,255, 255);
            if(frameNumber %10 == 0){
            if(changeLeftHip.x <= .006){
                featExtractors[i].spiral = false;
                for(int s=0; s<spirals.size(); s++){
                    if( spirals[s].blobNumber == i+1){
                        spirals[s].blobNumber = -1*spirals[s].blobNumber ;
                    }
                }
                ofDrawCircle(10, 10, 50);
            }
            }
            if(featExtractors[i].spiral == true){
            for(int s=0; s<spirals.size(); s++){
                if( spirals[s].blobNumber == i+1){
                    spirals[s].loadPoints(leftHipPosition[i], leftHipVelocity[i]);
                }
            }
            }
            
            if(frameNumber %10 == 0){
               
                prevLeftHipPos[i] = leftHipPosTorso[i];
            }
            
        }//end for loop
        
    }
   
//------------------------------- draw to screen -----------------------------//
    ofPushMatrix();
    ofScale(ofGetWidth() / float(kinect.getWidth()), ofGetHeight() / float(kinect.getHeight()));

 
    
    if(pollockStrokes.size() > 0){
        for(int i=0; i<pollockStrokes.size(); i++){
            pollockStrokes[i].draw();
        }
    }
    if(spirals.size() > 0){
        for(int i=0; i<spirals.size(); i++){
            //   spirals[i].draw();
        }
    }
    for(int i=0; i< simpleLines.size(); i++){
        ofSetColor(0, 0, 0);
        simpleLines[i].update();
       // simpleLines[i].draw();
    }
    ofPopMatrix();

 //   ofDrawBitmapString(os.str(), 20, 30);
    
     //----------paint cans
    ofPushStyle();
    ofSetColor(255);
//    canWhite.draw(whitePos.x, whitePos.y, boxSize, boxSize);
    ofFill();
//    canBlack.draw(blackPos.x, blackPos.y, boxSize, boxSize);
//    canYellow.draw(yellowPos.x, yellowPos.y, boxSize, boxSize);
//    canRed.draw(redPos.x, redPos.y, boxSize, boxSize);
//    canBlue.draw(bluePos.x, bluePos.y, boxSize, boxSize);
    
    
    
    //------------------------------- color selection -----------------------------//
    int vidWidth = 200;
    int vidHeight = 113;
    
    blackCanSplash.draw(blackPos.x,blackPos.y, vidWidth, vidHeight);
    whiteCanSplash.draw(whitePos.x, whitePos.y, vidWidth, vidHeight);
    redCanSplash.draw(redPos.x, redPos.y, vidWidth, vidHeight);
    blueCanSplash.draw(bluePos.x, bluePos.y, vidWidth, vidHeight);
    yellowCanSplash.draw(yellowPos.x, yellowPos.y, vidWidth, vidHeight);
    ofPopStyle();
    
    if(mouseX > ofGetWidth()/2){
        blackCanSplash.play();
    }else{
        blackCanSplash.stop();
    }
    if(kinect.getNumTrackedUsers() >0){
        ofPoint torsoCenterPos[3] = {kinect.worldToProjective(featExtractor1.getPosition(torso)), kinect.worldToProjective(featExtractor2.getPosition(torso)), kinect.worldToProjective(featExtractor3.getPosition(torso))};
        
        for(int i = 0; i<3; i++){
            
            ////
            torsoCenterPos[i].x = ofMap(torsoCenterPos[i].x, 0, 640, 0, 1024);
            torsoCenterPos[i].z = ofMap(torsoCenterPos[i].z, 0, 4100, 0, 768);
            //min of z currently is 1000 and max 3100
            font.drawString(ofToString(torsoCenterPos[0].y), 350, 50);
           
            ofSetHexColor(0xFFFFFF);
            if((torsoCenterPos[i].z > blackPos.y) && (torsoCenterPos[i].z < blackPos.y + boxSize) && (torsoCenterPos[i].x > blackPos.x) && (torsoCenterPos[i].x < blackPos.x + boxSize)){
                blackCanSplash.play();
                
                font.drawString("selected black!!!", 50, 50);
                featExtractors[i].selectedBlack = true;
                featExtractors[i].color =  ofColor::black;
            }
            if((torsoCenterPos[i].z > whitePos.y) && (torsoCenterPos[i].z < whitePos.y + boxSize) && (torsoCenterPos[i].x > whitePos.x) && (torsoCenterPos[i].x < whitePos.x + boxSize)){
                whiteCanSplash.play();
                
                font.drawString("selected white!!!", 100, 50);
                featExtractors[i].selectedWhite = true;
                featExtractors[i].color =  ofColor::white;
            }
            if((torsoCenterPos[i].z > redPos.y) && (torsoCenterPos[i].z < redPos.y + boxSize) && (torsoCenterPos[i].x > redPos.x) && (torsoCenterPos[i].x < redPos.x + boxSize)){
                redCanSplash.play();
                
                font.drawString("selected red!!!", 150, 50);
                featExtractors[i].selectedRed = true;
                featExtractors[i].color =  ofColor::magenta;
            }
            if((torsoCenterPos[i].z > bluePos.y) && (torsoCenterPos[i].z < bluePos.y + boxSize) && (torsoCenterPos[i].x > bluePos.x) && (torsoCenterPos[i].x < bluePos.x + boxSize)){
                blueCanSplash.play();
                
                font.drawString("selected blue!!!", 250, 50);
                featExtractors[i].selectedBlue = true;
                featExtractors[i].color =  ofColor::cyan;
            }
            if((torsoCenterPos[i].z > yellowPos.y) && (torsoCenterPos[i].z < yellowPos.y + boxSize) && (torsoCenterPos[i].x > yellowPos.x) && (torsoCenterPos[i].x < yellowPos.x + boxSize)){
                yellowCanSplash.play();
                
                font.drawString("selected yellow!!!", 350, 50);
                featExtractors[i].selectedYellow = true;
                featExtractors[i].color =  ofColor::yellow;
            }
        } // end of for loop color selection

        
//------------------------------- detect jump -----------------------------//
        
        font.drawString(ofToString(torsoCenterPos[0].y),torsoCenterPos[0].x, torsoCenterPos[0].z);
            if(frameNumber % 50 == 0){ //every 50 frames its erasing the first 29. There are always 20 in there
               // cout << "prevTorsoPos1 size " << prevTorsoPos1.size() << endl;
           
                //loop ++;
//                prevTorsoPos1.erase(prevTorsoPos1.begin(), prevTorsoPos1.begin()+20*loop);
//                prevTorsoPos2.erase(prevTorsoPos1.begin(), prevTorsoPos1.begin()+29);
//                prevTorsoPos3.erase(prevTorsoPos1.begin(), prevTorsoPos1.begin()+29);
            }
        
        
            //every frame check if the users pos is ___ amount different than it was 40 fraames ago
            //within 120 frames has it been different at least three times? Did the user jump three times?
        for(int i=0; i<3; i++){
            if(prevTorsoPos[i].size() > 40 && kinect.getNumTrackedUsers() >0 ){
                cout<<"checking for jump user " << i+1;
                int twentyFramesAgo = prevTorsoPos[i].size() - 40;
                int diff = torsoCenterPos[i].y - prevTorsoPos[i][twentyFramesAgo].y;
               
                if(diff < -40 && timer[i] >5){
                    timer[i] = 0;
                    timerForJump[i] = frameNumber;
                    cout << "jumped" << endl;
                    numJumps[i] ++;
                    //user1 jumped
                    ofSetColor(255, 0, 0);
                    ofDrawCircle(300,300,50);
                    if(numJumps[i] == 1){
                        //add point to shape
                        int lastShape = shapes.size() -1;
                        shapes[lastShape].push_back(torsoCenterPos[0]);
                    }
                   
                }
    
                
                if(frameNumber - timerForJump[i] < 80){//one period of jump detection
                    
                    if(numJumps[i] == 2){
                        numJumps[i] = 0;
                        //user has jumped twice
                        cout << "user has jumped twice in 80 frames" << endl;
                        //start new shape
                        vector <ofPoint> newShape;
                        newShape.push_back(torsoCenterPos[i]);
                        shapes.push_back(newShape); //push it into the big vector of all the shapes
                        drawShape(torsoCenterPos[i]);
                    }
                }else{
                    numJumps[i] = 0;
                }//end if for period detection
            }
        
        }//end for loop
        prevTorsoPos1.push_back(torsoCenterPos[0]);
        prevTorsoPos2.push_back(torsoCenterPos[1]);
        prevTorsoPos3.push_back(torsoCenterPos[2]);
        cout << "size " << prevTorsoPos[0].size() << endl;
      
    } // end kinect.getnumtracked users if
    

    
//------------------------------- color mixing -----------------------------//
//check if any of the hands in one skeleton are touching the hands in another skeleton
    if(kinect.getNumTrackedUsers() >0){
        ofPoint rightHandPos[3] = {kinect.worldToProjective(featExtractor1.getPosition(j)),kinect.worldToProjective(featExtractor2.getPosition(j)), kinect.worldToProjective(featExtractor3.getPosition(j))};
        ofPoint leftHandPos[3] = {kinect.worldToProjective(featExtractor1.getPosition(leftHand)),kinect.worldToProjective(featExtractor2.getPosition(leftHand)), kinect.worldToProjective(featExtractor3.getPosition(leftHand))};
        int buffer = 10;
        for(int i=0; i<3; i++){
            for(int f=0; f<3; f++){
                if(i != f){
                    float dist = ofDist(rightHandPos[i].x, rightHandPos[i].y, rightHandPos[i].z, rightHandPos[f].x, rightHandPos[f].y, rightHandPos[f].z);
                    float dist2 = ofDist(rightHandPos[i].x, rightHandPos[i].y, rightHandPos[i].z, leftHandPos[f].x, leftHandPos[f].y, leftHandPos[f].z);
                    float dist3 = ofDist(leftHandPos[i].x, leftHandPos[i].y, leftHandPos[i].z, leftHandPos[f].x, leftHandPos[f].y, leftHandPos[f].z);
                    ofSetColor(255, 0, 0);
                    //i is first user, f is second user. have to add 1 to get the usernumber which starts at 1 and not 0
                    if(dist > 150 && dist2 > 150 && dist3 > 150){
                        //set mixColor to false
                        featExtractors[i].mixColor = false;
                        featExtractors[f].mixColor = false;
                    }
                    if(dist < 100 || dist2 < 100 || dist3 < 100){
                        if(featExtractors[i].prevMixColor ==false  && featExtractors[f].prevMixColor ==false ){
                            //set mixColor to true
                            featExtractors[i].mixColor = true;
                            featExtractors[f].mixColor = true;
                            //ofDrawCircle(500, 500, 100);
                            ofColor userColor1 = featExtractors[i].color;
                            ofColor userColor2 = featExtractors[f].color;
                            ofColor newColor;
                            newColor = mixColor(userColor1, userColor2);
                            featExtractors[i].color = newColor;
                            featExtractors[f].color  = newColor;
                        }
                    }
                    featExtractors[i].prevMixColor =  featExtractors[i].mixColor;
                    featExtractors[f].prevMixColor =  featExtractors[f].mixColor;
                  
                }//end of if
            }//end second loop
        }//end first loop
    }//end if
    
    ofSetColor(0, 0, 0);
    
    font.drawString(ofToString(featExtractor1.color), 30, 700);
    
    //draw the shapes
    for(int i =0; i< shapes.size(); i++){
        ofBeginShape();
        for (int s = 0; s < shapes[i].size(); s++){
            ofVertex(shapes[i][s].x, shapes[i][s].y);
        }
        ofEndShape(true);
    }
    //if the distance between posTorso.y in 2 se
} // end draw
//--------------------------------------------------------------
void ofApp::drawShape(ofPoint _position){
    //user has jumped
    
}
//--------------------------------------------------------------
ofColor ofApp::mixColor(ofColor color1, ofColor color2){
     cout << "mix colors function started" <<endl;
    float hue = 0;  // The hue value to set.
    float saturation = 0; // The saturation value to set.
    float brightness = 0; // The brightness value to set.
    ofColor newColor;
    if(color1 == ofColor::white || color2 == ofColor::white ){
        cout << "mixing " << color1 << " with " << color2 << endl;
        if(color1 == ofColor::white){
            color2.getHsb(hue, saturation, brightness);
            color2.setHsb(hue, saturation-200, brightness);
            newColor = color2;
            cout << "new color is " << newColor << endl;
        }else{
            color1.getHsb(hue, saturation, brightness);
            color1.setHsb(hue, saturation-200, brightness);
            newColor = color1;
             cout << "new color is " << newColor << endl;
        }
    
    }else if(color1 == ofColor::black || color2 == ofColor::black){
        cout << "mixing " << color1 << " with " << color2 << endl;
        if(color1 == ofColor::black){
            color2.getHsb(hue, saturation, brightness);
            color2.setHsb(hue, saturation, brightness-100);
            newColor = color2;
             cout << "new color is " << newColor << endl;
        }else{
            color1.getHsb(hue, saturation, brightness);
            color1.setHsb(hue, saturation, brightness-100);
            newColor = color1;
             cout << "new color is " << newColor << endl;
        }
    }else{
        cout << "mixing " << color1 << " with " << color2 << endl;
        //converting them from rgb to cmyk
        color1.r = 255 - color1.r; color1.g = 255 - color1.g; color1.b = 255 - color1.b;
        color2.r = 255 - color2.r; color2.g = 255 - color2.g; color2.b = 255 - color2.b;
        cout << "color1 converted to " << color1 << endl;
        cout << "color2 converted to " << color2 << endl;
        //adding the colors
        newColor.r = color1.r + color2.r;
        newColor.g = color1.g + color2.g;
        newColor.b = color1.b + color2.b;
        cout << "newColor cmyk is " << newColor << endl;
        //converting them back to rgb
        newColor.r = 255 - newColor.r; newColor.g = 255 - newColor.g; newColor.b = 255 - newColor.b;
        cout << "newColor rgb is " << newColor << endl;
        newColor.r = clamp(newColor.r);
        newColor.g = clamp(newColor.g);
        newColor.b = clamp(newColor.b);
        
        cout << "newColor clamped is " << newColor << endl;
        
    }
    return newColor;
   
}

int ofApp::clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_RIGHT:
            if (j < JOINT_COUNT-1) {
                j++;
            }
            break;
        case OF_KEY_LEFT:
            if (j > 0){
                j--;
            }
            break;
        case OF_KEY_UP:
            if (f < RELPOSTOTORSO_X) {
                f++;
            }
            break;
        case OF_KEY_DOWN:
            if (f > 0){
                f--;
            }
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::userEvent(ofxOpenNIUserEvent &event){
    //    if (event.userStatus == USER_TRACKING_STOPPED) {
    //        featExtractor.removeSkeleton(0);
    //    }
}

void ofApp::mocapMax(MocapMaxEvent &e){
    
    
    if (e.feature == FEAT_QOM && e.value > 20.0){
        cout << "max in QOM!" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
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