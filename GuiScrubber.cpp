#include "GuiScrubber.h"

GuiScrubber::GuiScrubber(map<string,string> &_attrs){
    attrs = _attrs;
    cout << "new scrubber. pos = " << attrs["pos"] << endl;
    initialize();
    ball.loadImage("cuts/ball_clipped.png");
    stick.loadImage("cuts/scrubber_stick.png");
    size.x = ball.getWidth();
    size.y = ball.getHeight();
    homeX = pos.x;
    lastX = homeX;
    maxX = 1446;
    lastPct = 0.0;
    SubObMediator::Instance()->addObserver("timeline-changed",this);
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

void GuiScrubber::executeDrag(int _x, int _y){
    //cout << "scrubber moving..." << endl;
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
        lastPct = pct;
    }
}

void GuiScrubber::update(){
    //float pct = Compositor::Instance()->getScrubberPosition();
    //pct = lastPct = Compositor::Instance()->getScrubberPosition();
    pos.x = homeX + (maxX - homeX) * pct;
}

void GuiScrubber::update(Subject *_sub){
    string sPct = _sub->getAttr("time-pos");
    pct = lastPct = ofToFloat(sPct);
}

void GuiScrubber::draw(){
    stick.draw(pos.x - stick.getWidth() / 2, pos.y);
    ball.draw(pos.x - ball.getWidth() / 2, pos.y);
}
