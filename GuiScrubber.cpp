#include "GuiScrubber.h"

GuiScrubber::GuiScrubber(map<string,string> &_attrs){
    attrs = _attrs;
    cout << "new scrubber. pos = " << attrs["pos"] << endl;
    initialize();
    cout << "scrubber pos = " << pos.x << ", " << pos.y << endl;
    ball.loadImage("cuts/ball_clipped.png");
    stick.loadImage("cuts/scrubber_stick.png");
    size.x = ball.getWidth();
    size.y = ball.getHeight();
    homeX = pos.x;
    lastX = homeX;
    maxX = 1446;
    lastPct = 0.0;
    SubObMediator::Instance()->addObserver("timeline-position-changed",this);
    bActivated = false;
    attrs["action"] = "control";
}

void GuiScrubber::execute(){
    Compositor::Instance()->pause();
}

bool GuiScrubber::isDragging(int _x, int _y){
    ofVec2f actualPos(pos.x - ball.getWidth() / 2, pos.y);
    if((_x > actualPos.x && _x < actualPos.x + size.x) &&
       (_y > actualPos.y && _y < actualPos.y + size.y)){
        return true;
    }
    return false;
}

bool GuiScrubber::processMouse(int _x, int _y, int _state){
    if(bActivated && _state == MOUSE_STATE_UP){
        cout << "scrubber off" << endl;
        bActivated = false;
        attrs.erase("name");
        attrs["name"] = "play";
        SubObMediator::Instance()->update("button", this);
        return true;
    }
    if(bActivated && _state == MOUSE_STATE_DRAG){
        executeDrag(_x, _y);
        return true;
    }
    if(isInside(_x, _y)){
        if(!bActivated && _state == MOUSE_STATE_DOWN){
            cout << "scrubber on" << endl;
            bActivated = true;
            attrs.erase("name");
            attrs["name"] = "pause";
            SubObMediator::Instance()->update("button", this);
            return true;
        }
    } else {
        return false;
    }
}

bool GuiScrubber::isInside(int _x, int _y){
    //cout << name << " checking insides." << pos.x << ", " << size.x << endl;
    if((_x > pos.x - 20 && _x < ((pos.x - 20) + size.x + 40)) &&
       (_y > pos.y - 20 && _y < ((pos.y  - 20) + size.y + 40))){
           return true;
       }
    return false;
}

void GuiScrubber::executeDrag(int _x, int _y){
    pos.x += _x - pos.x;
    if(pos.x < homeX){
        pos.x = homeX;
    }
    if(pos.x > maxX){
        pos.x = maxX;
    }
    pct = (pos.x - homeX) / (maxX - homeX);
    if(abs(pct - lastPct) > 0.01){
        //Compositor::Instance()->setScrubberPosition(pct);
        attrs["time-pos"] = ofToString(pct);
        SubObMediator::Instance()->update("scrubber-position-changed", this);
        lastPct = pct;
    }
}

void GuiScrubber::update(){
    //float pct = Compositor::Instance()->getScrubberPosition();
    //pct = lastPct = Compositor::Instance()->getScrubberPosition();
    //pos.x = homeX + (maxX - homeX) * pct;
}

void GuiScrubber::update(string _subName, Subject *_sub){
    string sPct = _sub->getAttr("time-pos");
    pct = lastPct = ofToFloat(sPct);
    //cout << "scrubber set position to " << pct << endl;
    pos.x = homeX + (maxX - homeX) * pct;
}

void GuiScrubber::draw(){
    //cout << "drawing scrubber from - " << pos.x << ", " << pos.y << endl;
    stick.draw(pos.x - stick.getWidth() / 2, pos.y);
    ball.draw(pos.x - ball.getWidth() / 2, pos.y);
}
