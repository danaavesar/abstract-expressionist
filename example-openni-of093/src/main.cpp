#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.width = 1024;//1024
    settings.height = 768;//768
    settings.setPosition(ofVec2f(300,0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    settings.width = 840;
    settings.height = 1080;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;
    // uncomment next line to share main's OpenGL resources with gui
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> floorWindow = ofCreateWindow(settings);
    floorWindow->setVerticalSync(false);
    
    shared_ptr<ofApp> mainApp(new ofApp);
   
    ofAddListener(floorWindow->events().draw,mainApp.get(),&ofApp::drawFloorWindow);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

}
