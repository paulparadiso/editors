#include "testApp.h"
#include "Compositor.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    string file = "sheets.xml";
    Compositor::Instance()->update();
    GuiConfigurator::Instance()->addFile(file);
    //gCr = new GuiCreator();
    GuiConfigurator::Instance()->getTags();
    GuiConfigurator::Instance()->makeGUI();
    img.loadImage("cuts/main_bg_20.png");
    ofToggleFullscreen();
    ofSetFrameRate(30);
    sfxPlayer = new SoundEffectsPlayer();
}

//--------------------------------------------------------------
void testApp::update(){
    //Compositor::Instance()->update();
    SceneManager::Instance()->update();
    //cout << "Frame rate = " << ofGetFrameRate() << endl;
}

//--------------------------------------------------------------
void testApp::draw(){
    //img.draw(0,0);
    //gc->draw();
	SceneManager::Instance()->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == 'r'){
        Compositor::Instance()->saveVideo("newvid");
    }
    if(key == 'a'){
        Compositor::Instance()->saveAudio("test.wav");
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
    updateMouseState("down", x, y, button);
    timeOfLastInteraction = ofGetElapsedTimef();
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
