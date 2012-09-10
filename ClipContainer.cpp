#include "ClipContainer.h"

ClipContainer::ClipContainer(Clip* _clip){
    clip = _clip;
    SubObMediator::Instance()->addObserver("new-effect", this);
    bDead = false;
    bHaveTransition = false;
    bCrossfade = false;
    bNeedsNudge = false;
    buttonLength = (float)clip->getTotalNumFrames() / 24.0;
    timelineButton = new GuiTimelineButton(buttonLength, clip->getType(), this);
    effectStatus = 0;
    startFrame = 0;
    stopFrame = 0;
    bHaveNewEffect = false;
    bHasTransition = false;
}

void ClipContainer::setStartFrame(int _startFrame){
    startFrame = _startFrame;
    stopFrame = startFrame + clip->getTotalNumFrames();
}

void ClipContainer::adjustStartFrame(int _amt){
    cout << "old start frame = " << startFrame << endl;
    startFrame += _amt;
    stopFrame += _amt;
    cout << "new start frame = " << startFrame << endl;
}

unsigned char* ClipContainer::getPixels(){
    clip->setEffectStatus(effectStatus);
    return clip->getPixels();
}

float ClipContainer::getSample(int _samplePosition){
    //clip->setEffectStatus(effectStatus);
    return clip->getSample(_samplePosition);
}
void ClipContainer::update(string _subName, Subject* _sub){
    if(_subName == "new-effect"){
        string id = _sub->getAttr("name");
        string fx = _sub->getAttr("effect");
        //cout << clipID << " --- " << id << " --- " << fx << endl;
        int oldEffect = effectStatus;
    }
}

void ClipContainer::setEffectStatus(int _effectStatus){
    if(clip->getType() == "video"){
        if(effectStatus != _effectStatus){
            effectStatus = _effectStatus;
            if(effectStatus == 2 || effectStatus == 4){
                bNeedsNudge = true;
            } else {
                bNeedsNudge = false;
            }
            if(effectStatus == 3 || effectStatus == 4){
                bHasTransition = true;
            } else {
                bHasTransition = false;
            }
            if(effectStatus == 4){
                bHasCrossfade = true;
            } else {
                bHasCrossfade = false;
            }
            bHaveNewEffect = true;
        }
    } else {
        if(_effectStatus == 6){
            bHasTransition = true;
        } else {
            bHasTransition = false;
        }
        clip->setEffectStatus(_effectStatus);
        bHaveNewEffect = true;
    }
}

bool ClipContainer::hasNewEffect(){
    if(bHaveNewEffect){
        bHaveNewEffect = false;
        return true;
    }
    return false;
}

void ClipContainer::resetButton(){
    timelineButton->setLength(buttonLength);
}

void ClipContainer::adjustButton(){
    timelineButton->setLength(buttonLength - 0.5);
}

void ClipContainer::draw(){
    timelineButton->draw();
}
