#include "GuiCreator.h"
#include "GuiButton.h"

GuiCreator::GuiCreator(){
}

bool GuiNode::isInside(int _x, int _y){
    cout << name << " chacking insides against - " << _x << ", " << _y << endl;
    if((_x > pos.x && _x < (pos.x + size.x)) &&
       (_y > pos.y && _y < (pos.y + size.y))){
           return true;
       }
    return false;
}

void GuiNode::initialize(){
    type = attrs["type"];
    name = attrs["name"];
    pos = stringToVec2f(attrs["pos"]);
    //size = stringToVec2f(attrs["size"]);
}

void GuiSheet::addNode(GuiNode* _node){
    nodes.push_back(_node);
}

void GuiSheet::pushToStack(){
}

void GuiSheet::popFromStack(){
}

void GuiSheet::draw(){
	vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); nIter++){
		(*nIter)->draw();
	}
}

Menu::Menu(map<string, string> &_attrs) : GuiNode(){
    isOpen = false;
    attrs = _attrs;
    transition = new Transition(attrs["transition-type"]);
    transitionTime = ofToInt(attrs["transition-time"]);
    initialize();
}

void Menu::addNode(GuiCreator *_gc, map<string, string> &_attrs){
    //menuItems.push_back(_gc->makeNode(_attrs));
}

void Menu::deleteNode(){
}

void Menu::execute(){
}

void Menu::draw(){
}

GuiButton::GuiButton(map<string, string> &_attrs) : GuiNode(){
    attrs = _attrs;
    bg.loadImage(attrs["image"]);
    size.x = bg.getWidth();
    size.y = bg.getHeight();
    initialize();
	SceneManager::Instance()->addInactiveNode(this);
}

void GuiButton::execute(){
    cout << "BANG!!!" << endl;
}

void GuiButton::draw(){
    bg.draw(pos.x,pos.y);
}


Decoration::Decoration(map<string,string> &_attrs): GuiNode(){
    color = stringToVec3f(_attrs["color"]);
    attrs = _attrs;
    initialize();
}

void Decoration::execute(){
    //probably won't ever do anything.

}

void Decoration::draw(){
    //will eventually recieve an engine reference as a
    //ofSetColor(0,255,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
}

Image::Image(map<string,string> &_attrs): GuiNode(){
    img.loadImage(_attrs["image"]);
    attrs = _attrs;
    initialize();
}

void Image::execute(){
}

void Image::draw(){
    //ofSetColor(0,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
	img.draw(pos.x,pos.y);
}

MediaPreview::MediaPreview(map<string,string> &_attrs): GuiNode(){
    attrs = _attrs;
    initialize();
}

void MediaPreview::execute(){
}

void MediaPreview::draw(){
    //ofSetColor(255,255,0);
    //ofRect(pos.x,pos.y,size.x, size.y);
}

Timeline::Timeline(map<string,string> &_attrs): GuiNode(){
    attrs = _attrs;
    initialize();
}

void Timeline::execute(){
}

void Timeline:: draw(){
    //ofSetColor(255,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
}

ofVec2f stringToVec2f(string _in){
    cout << "string to 2f with " << _in << endl;
    vector<string> res = ofSplitString(_in, ",");
    vector<string>::iterator sIter;
    for(sIter = res.begin(); sIter < res.end(); sIter++){
        cout << (*sIter) << endl;
    }
    return ofVec2f(ofToFloat(res[0]),ofToFloat(res[1]));
}

ofVec3f stringToVec3f(string _in){
    cout << "string to 3f with " << _in << endl;
    vector<string> res = ofSplitString(_in, ",");
    vector<string>::iterator sIter;
    for(sIter = res.begin(); sIter < res.end(); sIter++){
        cout << (*sIter) << endl;
    }
    return ofVec3f(ofToFloat(res[0]), ofToFloat(res[1]), ofToFloat(res[2]));
}


