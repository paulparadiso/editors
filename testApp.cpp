#include "testApp.h"
#include "Compositor.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    string file = "../sheets.xml";
    GuiConfigurator::Instance()->addFile(file);
    GuiConfigurator::Instance()->getTimelines();
    GuiConfigurator::Instance()->getTags();
    GuiConfigurator::Instance()->makeGUI();
    ofToggleFullscreen();
    //ofSetFrameRate(30);
    sfxPlayer = new SoundEffectsPlayer();
    timeOfLastInput = ofGetElapsedTimef();
    timeOfLastInteraction = ofGetElapsedTimef();
    inputDelayTime = 0.2;
    checkTimeOut = true;
    timeOutCounter = 30.0;
    SubObMediator::Instance()->addObserver("sheet-changed", this);
}

//--------------------------------------------------------------
void testApp::update(){
    SceneManager::Instance()->update();
    if(checkTimeOut){
        if(SceneManager::Instance()->getIsPlaying()){
            timeOfLastInteraction = ofGetElapsedTimef();
        }
        if(ofGetElapsedTimef() > timeOfLastInteraction + timeOutCounter){
            cout << "Inactivity time triggered.  Resetting." << endl;
            SubObMediator::Instance()->update("reset", this);
            MediaCabinet::Instance()->reset();
        }
    }
}

void testApp::update(string _subName, Subject *_sub){
    /*
    if(_subName == "sheet-changed"){
        string topSheet = _sub->getAttr("top-sheet");
        if(topSheet == "main"){
            cout << "starting inactivity timer." << endl;
            checkTimeOut = true;
            timeOfLastInteraction = ofGetElapsedTimef();
        } else {
            cout << "stopping inactivity timer." << endl;
            checkTimeOut = false;
        }
    }
    */
}

//--------------------------------------------------------------
void testApp::draw(){
	SceneManager::Instance()->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'f'){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    /*
    GuiSheet* topSheetPtr = SceneManager::Instance()->getTopSheet();
    if(topSheetPtr->checkNodesForDrag(x,y)){
        GuiNode* activatedNodePtr = topSheetPtr->getActivatedNode();
        activatedNodePtr->executeDrag(x,y);
    }
    */
    updateMouseState("drag", x, y, button);
    timeOfLastInteraction = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //gc->click(x,y);
    /*
    GuiSheet* topSheetPtr = SceneManager::Instance()->getTopSheet();
    if(topSheetPtr->checkNodes(x,y)){
        GuiNode* activatedNodePtr = topSheetPtr->getActivatedNode();
        activatedNodePtr->execute();
    }
    timeOfLastInteraction = ofGetElapsedTimef();
    */
    if(ofGetElapsedTimef() - timeOfLastInput > inputDelayTime){
        updateMouseState("down", x, y, button);
        timeOfLastInteraction = ofGetElapsedTimef();
        timeOfLastInput = ofGetElapsedTimef();
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    updateMouseState("up", x, y, button);
    timeOfLastInteraction = ofGetElapsedTimef();
}

void testApp::updateMouseState(const char * _state, int _x, int _y, int _button){
    attrs["mouseX"] = ofToString(_x);
    attrs["mouseY"] = ofToString(_y);
    attrs["mouseButton"] = ofToString(_button);
    attrs["mouseState"] = _state;
    SubObMediator::Instance()->update("mouse-changed",this);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
