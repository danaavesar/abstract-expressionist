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
    timer = 0;
    counter = 0;
    
    //////ofxkinect
    
    boxSize = 70;
//    y = ofGetHeight()-60 - boxSize;
    x = ofGetWidth()-60-boxSize;
    int buffer = 100;
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
//    for(int i=0; i< featExtractors.size(); i++){
//        featExtractors[i].setup(JOINT_HEAD, JOINT_TORSO);
//    }
    
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
   
   
    //booleans for strokes
//    for(int i=0; i < featExtractors.size(); i++){
//        featExtractors[i].setup(JOINT_HEAD, JOINT_TORSO);
//        featExtractors[i].pollock = false;
//        featExtractors[i].prevPollock= false;
//        featExtractors[i].spiral = false;
//        
//    }
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
}

//--------------------------------------------------------------
void ofApp::update(){

    kinect.update();
    frameNumber ++;
    //---------contourFInder tracker
    if(kinect.isNewFrame()){
        contourFinder.setMinAreaRadius(minRadius);//44
        contourFinder.setMaxAreaRadius(maxRadius);
         cv::Mat mat = cv::Mat(kinect.getHeight(), kinect.getWidth(), CV_8UC1, kinect.getDepthPixels().getChannel(0), 0);//
        contourFinder.setThreshold(thresh);//178.5
        contourFinder.findContours(mat);
        contourFinder.setFindHoles(true);
    }
    tracker = contourFinder.getTracker();
    
    //---------fill the feature exctractor vector with skeleton number
    if(kinect.getNumTrackedUsers() > 0){
        for(int i=0; i< kinect.getNumTrackedUsers(); i++){
            ofxOpenNIUser user = kinect.getTrackedUser(i);
              //cout << "muber of users " << kinect.getNumTrackedUsers() << "user id " << user.getXnID() << endl;
            featExtractors[user.getXnID()].setup(JOINT_HEAD, JOINT_TORSO);
        }
    }
  

    
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
    
//-------------- now checking which user has left and then deleting it from the featExtractors array

        for (int j=0; j< prevUsers.size(); j++){
            //cout << "prevUser " << prevUsers[j].getXnID() << endl;
            int prevUserId = prevUsers[j].getXnID();
            if(kinect.getNumTrackedUsers() > 0){
                for(int p=0; p<kinect.getNumTrackedUsers(); p++){
                      int currentUserId = kinect.getTrackedUser(p).getXnID();
                    if(prevUserId != currentUserId ){
                        //has left
                        featExtractors.erase(prevUserId);
                        cout << "erased user " << prevUserId;
                    }
                }
            }else{
                featExtractors.erase(prevUserId);
                cout << "erased user " << prevUserId;
            }
        }
    
    for (map<int,ofxKinectFeatures>::iterator ii=featExtractors.begin(); ii!=featExtractors.end(); ++ii){
        
        //cout << "ids in kinectFeaturesArray " << (*ii).first << endl;
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

                if (user.getJoint((Joint)0).getWorldPosition() != ofPoint(0,0,0) &&
                    user.getJoint((Joint)0).getWorldPosition() != featExtractors[userId].getPosition(0) ) {
                    map<int, ofPoint> joints;
                     //cout << "user " << userId << " numJoints " << user.getNumJoints() << endl;
                    for (int j = 0; j < user.getNumJoints(); j++) {
                       
                        joints[j] = user.getJoint((Joint)j).getWorldPosition();
                    }
                   // featExtractors[userId].update(joints);
                }
            
        }//end of for loop
    } //end of if statement
    
    //---------------- apply strokes
   if(kinect.getNumTrackedUsers() >0){
        for(int i = 0; i < kinect.getNumTrackedUsers(); i++){
            
            //----------------- check if hands are moving fast for Pollock
            int userId = kinect.getTrackedUser(i).getXnID();
            int rightHandVelocity = featExtractors[userId].getVelocityMagnitude(j);
            ofPoint rightHandPosition = kinect.worldToProjective(featExtractors[userId].getPosition(j));
            if(rightHandVelocity > 5){
                featExtractors[userId].pollock = true;
                bool splash;
                if(rightHandVelocity > 65){
                    splash = true;
                }else{
                    splash = false;
                }
                for(int s=0; s<pollockStrokes.size(); s++){
                    if(pollockStrokes[s].blobNumber == userId){
                        pollockStrokes[s].loadPoints(rightHandPosition.x, rightHandPosition.y, rightHandPosition.z, splash);
                    }
                }
            }else{
                featExtractors[userId].pollock = false;
            }
            
            if(featExtractors[userId].prevPollock == false && featExtractors[userId].pollock == true){
                Stroke newPollockStroke;
                newPollockStroke.setup(userId);
                newPollockStroke.setColor(featExtractors[userId].color);
                //if there is already a stroke with this num don't associate it with this id anymore
                for(int s=0; s<pollockStrokes.size(); s++){
                    if(pollockStrokes[s].blobNumber == userId){
                        pollockStrokes[s].blobNumber = -pollockStrokes[s].blobNumber;
                    }
                }
                pollockStrokes.push_back(newPollockStroke);
            };

            featExtractors[userId].prevPollock = featExtractors[userId].pollock;
            

           
            
           
        } //end of kinect.getNumTrackedUsers() loop
    }// end of if
    

    const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
   
    //----------log previous labels, first clear them
    
    if(frameNumber%5 == 0){
        lastHoleLabels.clear();
    }
    
    for(int c=0; c<contourFinder.size(); c++){
        if(contourFinder.getHole(c)){
            int label = contourFinder.getLabel(c);
            lastHoleLabels.push_back(label);
        }
    }
   
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(ofColor::white);
    float w = ofGetWidth(); //w
    float h = ofGetHeight(); //h
    
 
    ofPushMatrix();
    //ofScale(ofGetWidth() / float(kinect.getWidth()), ofGetHeight() / float(kinect.getHeight()));
    //kinect.drawImage();
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

    //------------------------------- color selection -----------------------------//
    if(kinect.getNumTrackedUsers() >0){
        for(int i = 0; i < kinect.getNumTrackedUsers(); i++){
            int userId = kinect.getTrackedUser(i).getXnID();
            ofPoint torsoCenterPos = kinect.worldToProjective(featExtractors[userId].getPosition(torso));
            ////black
            torsoCenterPos.z = ofMap(torsoCenterPos.z, 0, 4100, 0, 768);
            //min of z currently is 1000 and max 3100
            font.drawString(ofToString(torsoCenterPos.z), torsoCenterPos.x, torsoCenterPos.z);
            if((torsoCenterPos.z > blackPos.y) && (torsoCenterPos.z < blackPos.y + boxSize) && (torsoCenterPos.x > blackPos.x) && (torsoCenterPos.x < blackPos.x + boxSize)){
                
                font.drawString("selected black!!!", 50, 50);
                featExtractors[userId].selectedBlack = true;
                featExtractors[userId].color =  ofColor::black;
            }
            if((torsoCenterPos.z > whitePos.y) && (torsoCenterPos.z < whitePos.y + boxSize) && (torsoCenterPos.x > whitePos.x) && (torsoCenterPos.x < whitePos.x + boxSize)){
                
                font.drawString("selected white!!!", 100, 50);
                featExtractors[userId].selectedWhite = true;
                featExtractors[userId].color =  ofColor::white;
            }
            if((torsoCenterPos.z > redPos.y) && (torsoCenterPos.z < redPos.y + boxSize) && (torsoCenterPos.x > redPos.x) && (torsoCenterPos.x < redPos.x + boxSize)){
                
                font.drawString("selected red!!!", 150, 50);
                featExtractors[userId].selectedRed = true;
                featExtractors[userId].color =  ofColor::red;
            }
            if((torsoCenterPos.z > bluePos.y) && (torsoCenterPos.z < bluePos.y + boxSize) && (torsoCenterPos.x > bluePos.x) && (torsoCenterPos.x < bluePos.x + boxSize)){
                
                font.drawString("selected blue!!!", 250, 50);
                featExtractors[userId].selectedBlue = true;
                featExtractors[userId].color =  ofColor::blue;
            }
            if((torsoCenterPos.z > yellowPos.y) && (torsoCenterPos.z < yellowPos.y + boxSize) && (torsoCenterPos.x > yellowPos.x) && (torsoCenterPos.x < yellowPos.x + boxSize)){
                
                font.drawString("selected yellow!!!", 350, 50);
                featExtractors[userId].selectedYellow = true;
                featExtractors[userId].color =  ofColor::yellow;
            }
//------------------------------- arm lines -----------------------------//
            for(int s=0; s< handJointNames.size(); s++){ //these joints are just hands
                int  jointName = handJointNames[s];
                handJointProjectedPos[jointName] = kinect.worldToProjective(featExtractors[userId].getPosition(jointName));
                ofPoint shoulderPos = kinect.worldToProjective(featExtractors[userId].getPosition(jointName-2));
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
                
                if(featExtractors[userId].lines[jointName] == false){ // each joint in each featExtractor has a bool
                    
                    if(vel.x > 200 || vel.y > 200){
                       font.drawString(ofToString(vel), handJointProjectedPos[jointName].x, handJointProjectedPos[jointName].y);
                        counter = 0;
                        featExtractors[userId].lines[jointName] = true;
                        
                        ofSetColor(255, 255, 255);
                        //if 30 frames have passed since the lase rectangle. this is in order to not make repeating shapes when you make a move
                    
                        SimpleLine line;
                        line.setup(shoulderPos, handJointProjectedPos[jointName]);
                        simpleLines.push_back(line);
                        
                    }
                }else if(featExtractors[userId].lines[jointName] == true && counter >= 60){
                
                        featExtractors[userId].lines[jointName] = false;
                    
                }
                
                if(frameNumber %50 == 0){
                  //  cout << "prev " << prevLeftHandPosTorso << " now " << leftHandPosTorso << endl;
                    prevHandJointProjectedPos[jointName] = handJointProjectedPos[jointName];
                }
            }//end loop for jointname (hands)
            
//------------------------------- hip spirals -----------------------------//
            ofPoint leftHipPosition = kinect.worldToProjective(featExtractors[userId].getPosition(leftHip));
            ofPoint leftHipPosTorso = featExtractors[userId].getRelativePositionToTorso(leftHip);
            ofPoint leftHipVelocity = featExtractors[userId].getVelocityMean(leftHip);
            float qm = featExtractors[userId].getQom();
            //int changeHipY = leftHipPosition.y - prevLeftHipPos.y;
            ofPoint changeLeftHip = leftHipPosTorso - prevLeftHipPos;
            if(changeLeftHip.z < 0){ changeLeftHip.z *= -1;};
            if(changeLeftHip.y < 0){ changeLeftHip.y *= -1;};
            if(changeLeftHip.x < 0){ changeLeftHip.x *= -1;};
           
            //font.drawString(ofToString(leftHipPosTorso ),10, 60);
            //font.drawString(ofToString(changeLeftHip ),10, 90);
           // cout << leftHipPosTorso << endl;
             if(frameNumber %10 == 0){
            if(changeLeftHip.x  > .05  && featExtractors[userId].spiral == false  ){
                ofSetColor(255, 0, 0);
                //cout << "new stroke "<< userId << endl;
                 ofDrawCircle(10, 10, 50);
                featExtractors[userId].spiral = true;
                Stroke2 newSpiral;
                newSpiral.setup(featExtractors[userId].color, userId);
                spirals.push_back(newSpiral);
                
            }
            }
            ofSetColor(255,255, 255);
//            cout << changeLeftHip.x << ", " << changeLeftHip.z << endl;
            if(frameNumber %10 == 0){
            if(changeLeftHip.x <= .006){
                //cout << "stop drawing "<< userId << endl;
                featExtractors[userId].spiral = false;
                for(int s=0; s<spirals.size(); s++){
                    if( spirals[s].blobNumber == userId){
                        spirals[s].blobNumber = -1*spirals[s].blobNumber ;
                    }
                }
                ofDrawCircle(10, 10, 50);
            }
            }
            if(featExtractors[userId].spiral == true){
            for(int s=0; s<spirals.size(); s++){
                if( spirals[s].blobNumber == userId){
                    spirals[s].loadPoints(leftHipPosition, leftHipVelocity);
                }
            }
            }
            
            if(frameNumber %10 == 0){
               
                prevLeftHipPos = leftHipPosTorso;
            }
            
        }//end kinect get users loop
        
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
               spirals[i].draw();
        }
    }
    for(int i=0; i< simpleLines.size(); i++){
        ofSetColor(0, 0, 0);
        simpleLines[i].update();
        simpleLines[i].draw();
    }
    ofPopMatrix();

 //   ofDrawBitmapString(os.str(), 20, 30);
    
     //----------paint cans
    ofPushStyle();
    ofSetColor(255);
    canWhite.draw(whitePos.x, whitePos.y, boxSize, boxSize);
    ofFill();
    canBlack.draw(blackPos.x, blackPos.y, boxSize, boxSize);
    canYellow.draw(yellowPos.x, yellowPos.y, boxSize, boxSize);
    canRed.draw(redPos.x, redPos.y, boxSize, boxSize);
    canBlue.draw(bluePos.x, bluePos.y, boxSize, boxSize);
   
    ofPopStyle();
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
    //    if (e.joint == JOINT_RIGHT_HAND && e.value > 5.0) {
    //        cout << "Max in right hand axis " << ofGetTimestampString() << endl;
    //    }
    
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