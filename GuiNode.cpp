#include "GuiNode.h"
#include "Utils.h"

bool GuiNode::isInside(int _x, int _y){
    //cout << name << " checking insides." << pos.x << ", " << size.x << endl;
    if((_x > pos.x && _x < (pos.x + size.x)) &&
       (_y > pos.y && _y < (pos.y + size.y))){
           return true;
       }
    return false;
}

GuiNode::GuiNode(){
    attrs.get_allocator().allocate(100);
}

void GuiNode::initialize(){
    type = attrs["type"];
    name = attrs["name"];
    pos = stringToVec2f(attrs["pos"]);
    haveDelay = false;
    checkDelay = false;
    map<string,string>::iterator mIter;
    mIter = attrs.find("delay");
    if(mIter != attrs.end()){
        haveDelay = true;
        checkDelay = false;
        delayTime = ofToInt(attrs["delay"]);
        cout << "Setting Timer to " << delayTime << endl;
    }
    //size = stringToVec2f(attrs["size"]);
}

