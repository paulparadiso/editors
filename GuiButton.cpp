#include "GuiButton.h"

GuiButton::GuiButton(map<string, string> &_attrs) : GuiNode(){
    attrs = _attrs;
    if(attrs["image"] != "none"){
        inactive.loadImage(attrs["image"]);
        haveImage = true;
        size.x = inactive.getWidth();
        size.y = inactive.getHeight();
    } else {
        haveImage = false;
        size = stringToVec2f(attrs["size"]);
    }
    haveActive = false;
    if(attrs["clicked"] != "none"){
        active.loadImage(attrs["clicked"]);
        haveActive = true;
    }
    drawActive = false;
    initialize();
    setName("button");
}

GuiButton::GuiButton(string _img) : GuiNode(){
    inactive.loadImage(_img);
    size.x = inactive.getWidth();
    size.y = inactive.getHeight();
    haveImage = true;
    drawActive = false;
}

bool GuiButton::processMouse(int _x, int _y, int _state){
    cout << name << " being checked" << endl;
    if(_state == MOUSE_STATE_DOWN){
        if(isInside(_x,_y)){
            execute();
            return true;
        }
    }
    return false;
}

void GuiButton::execute(){
    cout << name << " is executing" << endl;
    SubObMediator::Instance()->update(name, this);
}

void GuiButton::update(Subject* _sub){
}

void GuiButton::draw(){
    if(haveImage){
        if(!drawActive){
            inactive.draw(pos.x,pos.y);
        } else {
            active.draw(pos.x,pos.y);
        }
    }
}
