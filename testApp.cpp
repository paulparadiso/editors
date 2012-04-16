#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    string file = "sheets.xml";
    gc = new GuiConfigurator(file);
    //gCr = new GuiCreator();
    gc->getTags();
    gc->print();
    gc->makeGUI();
    img.loadImage("cuts/pb_bg.png");
    ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    img.draw(0,0);
    //gc->draw();
	SceneManager::Instance()->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'f')
        ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    gc->click(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
