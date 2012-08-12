#include "timeline.h"
#include "MediaCabinet.h"
#include "Compositor.h"
#include "sndfile.h"

ClipContainer::ClipContainer(Clip* _clip){
    clip = _clip;
    SubObMediator::Instance()->addObserver("new-effect", this);
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

void ClipContainer::update(string _subName, Subject* _sub){
    if(_subName == "new-effect"){
        string id = _sub->getAttr("name");
        string fx = _sub->getAttr("effect");
        //cout << clipID << " --- " << id << " --- " << fx << endl;
        int oldEffect = effectStatus;
        if(clipID == id){
            effectStatus = ofToInt(fx);
            clip->setEffectStatus(effectStatus);
            /*
            if(effectStatus == 2 || effectStatus == 4){
                cout << "turning on transition effect.";
                bHaveTransition = true;
                if(effectStatus == 4){
                    bCrossfade = true;
                } else {
                    cout << "this effect will not blend" << endl;
                    bCrossfade = false;
                }
                SubObMediator::Instance()->update("transition-effect-on", this);
            }
            if(effectStatus == 0 && (oldEffect == 2 || oldEffect == 4)){
                bHaveTransition = true;
                SubObMediator::Instance()->update("transition-effect-off", this);
            }
            */
        }
    }
}

Timeline::Timeline(float _totalTime, string _name, string _mode){
    totalTime = _totalTime; //Most likely 60.0
    timeRemaining = totalTime;
    mode = _mode;
    runningTime = 0.0;
    isPlaying = false;
    clipIndex = 0;
    numFrames = 0;
    currentClip = NULL;
    isTransitioning = false;
    setScrubTime(0.0);
    SubObMediator::Instance()->addObserver("timeline-clip-removed", this);
    SubObMediator::Instance()->addObserver("transition-effect-on", this);
    SubObMediator::Instance()->addObserver("transition-effect-off", this);
    frameBuffer = new unsigned char[854 * 480 * 3];
    output = new unsigned char[854 * 480 * 3];
    memset(frameBuffer, 0, 854 * 480 *3);
    if(mode == "video"){
        bHaveNewFrame = true;
    } else {
        bHaveNewFrame = false;
    }
    attrs["name"] = _name;
    bCompositing = false;
    if(mode == "audio"){
        stream = new ofRtAudioSoundStream;
        stream->setup(this,1,0,44100,256, 32);
        sndBuffer = new float[44100 / 24];
    }
    previousFrame = 0;
    samplePosition = 0;
}

unsigned char * Timeline::getPixels(){
    bHaveNewFrame = false;
    return output;
}

void Timeline::addClip(Clip *_clip){
    cout << "adding a clip of type - " << _clip->getType() << endl;
    clips.push_back(new ClipContainer(_clip));
    //clips.back()->setStartFrame(numFrames);
    //numFrames += clips.back()->getTotalNumFrames();
}

void Timeline::audioRequested(float * output, int bufferSize, int nChannels){
    //cout << "audio requested" << endl;
    for(int i = 0; i < bufferSize; i++){
        //if(bHaveNewFrame){
        if(isPlaying && activeClips.size() > 0){
            output[i] = (*activeClips.begin())->getSample(samplePosition++);
            if(samplePosition > currentMaxFrames){
                samplePosition = 0;
                //updateAudio();
                bHaveNewFrame = false;
            }
        } else {
            output[i] = 0.0;
        }
    }
}

void Timeline::addClip(Clip* _clip, string _id){
    clips.push_back(new ClipContainer(_clip));
    clips.back()->setStartFrame(numFrames);
    clips.back()->setID(_id);
    numFrames += clips.back()->getTotalNumFrames();
    //cout << "Timeline numFrames = " << numFrames << endl;
    int timeRemaining = (int)(60 - (numFrames / 24));
    attrs.erase("time");
    attrs["time"] = ofToString(timeRemaining);
    SubObMediator::Instance()->update("time-remaining",this);
}

void Timeline::removeClip(Clip *_clip){
    cout << "timeline asked to remove clip" << endl;
    if(currentClip->getClip() == _clip){
        currentClip = NULL;
    }
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        if((*cIter)->getClip() == _clip){
            (*cIter)->stop();
            cout << "timeline removing clip" << endl;
            //MediaCabinet::Instance()->removeClip((*cIter));
            clips.erase(cIter);
            if((*cIter)->getType() == "video" && clips.size() < 1){
                Compositor::Instance()->blackOut();
            }
            break;
        }
    }
}

void Timeline::setStartFrames(){
    int sFrame = 0;
    int tFrames = 0;
    vector<ClipContainer*>::iterator cIter;
    vector<ClipContainer*>::iterator prevIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        if((*cIter)->needsNudge() && cIter != clips.begin()){
            sFrame -= 24;
        }
        if(cIter != clips.begin()){
            prevIter = cIter;
            prevIter--;
            if((*cIter)->hasTransition()){
                (*prevIter)->setFadeOut(true);
            } else {
                (*prevIter)->setFadeOut(false);
            }
        }
        (*cIter)->setStartFrame(sFrame);
        sFrame = (*cIter)->getStopFrame();
        tFrames += (*cIter)->getTotalNumFrames();
    }
    numFrames = sFrame;
}

void Timeline::adjustStartFrames(int _amt){
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        (*cIter)->adjustStartFrame(_amt);
    }
}

void Timeline::update(string _subName, Subject* _sub){
    if(_subName == "timeline-clip-removed"){
        //string clipID = _sub->getAttr("clip-to-remove");
        cout << "need to remove " << _sub->getAttr("clip-to-remove") << endl;
        removeClip(_sub->getAttr("clip-to-remove"));
        SubObMediator::Instance()->update("update-timeline-framecount",this);
    }
}

void Timeline::adjustForTransition(bool _on){
    /*
    int framesToAdjust = 0;
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        if((*cIter)->hasTransition()){
            if(cIter != clips.begin()){
                cout << "found transition." << endl;
                if(_on){
                    framesToAdjust = -24;
                } else {
                    framesToAdjust = 24;
                }
                (*cIter)->adjustStartFrame(framesToAdjust);
                if(cIter != clips.begin()){
                    vector<ClipContainer*>::iterator dIter = cIter;
                    dIter--;
                    if((*cIter)->hasCrossfade()){
                        (*dIter)->setTransitionOut(true);
                    } else {
                        (*dIter )->setTransitionOut(false);
                    }
                }
            }
        } else {
            (*cIter)->adjustStartFrame(framesToAdjust);
        }
    }
    numFrames += framesToAdjust;
    */
}

void Timeline::setFrame(int _frame){
    if(mode == "video"){
        setVideoFrame(_frame);
    }
    if(mode == "audio"){
        setAudioFrame(_frame);
    }
}

void Timeline::setVideoFrame(int _frame){
    //cout << "timeline setting frame to - " << _frame << endl;
    bool bNewFrame = false;
    vector<ClipContainer*>::iterator cIter;
    activeClips.clear();
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        int clipStartFrame = (*cIter)->getStartFrame();
        int clipStopFrame = (*cIter)->getStopFrame();
        //cout << "startFrame = " << clipStartFrame << ", clipStopFrame = " << clipStopFrame << endl;
        if((clipStartFrame < _frame) && (_frame < clipStopFrame)){
            int frameToSet = _frame - clipStartFrame;
            //cout << "advancing frame to - " << frameToSet << endl;
            (*cIter)->setFrame(frameToSet);
            (*cIter)->update();
            activeClips.push_back((*cIter));
            bNewFrame = true;
        }
    }
    if(bNewFrame){
        if(activeClips.size() > 0){
            memcpy(frameBuffer, (*activeClips.begin())->getPixels(), 854 * 480 * 3);
            for(cIter = activeClips.begin() + 1; cIter != activeClips.end(); ++cIter){
                compositeFrames(frameBuffer, (*cIter)->getPixels(), 854, 480, 3, (*cIter)->hasCrossfade());
            }
            memcpy(output, frameBuffer, 854 * 480 * 3);
            bHaveNewFrame = true;
        }
    }
}

void Timeline::setAudioFrame(int _frame){
    //cout << "audio requested" << endl;
    bool bNewFrame = false;
    //if(_frame != previousFrame + 1 || _frame == 0){
        vector<ClipContainer*>::iterator cIter;
        activeClips.clear();
        for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
            int clipStartFrame = (*cIter)->getStartFrame();
            int clipStopFrame = (*cIter)->getStopFrame();
            //cout << "startFrame = " << clipStartFrame << ", clipStopFrame = " << clipStopFrame << endl;
            if((clipStartFrame < _frame) && (_frame < clipStopFrame)){
                int frameToSet = (_frame - clipStartFrame) * (44100 / 24);
                //(*cIter)->setFrame(frameToSet);
                //(*cIter)->update();
                currentMaxFrames = (*cIter)->getTotalNumSamples();
                activeClips.push_back((*cIter));
                if(_frame != previousFrame + 1 || _frame == 0){
                    samplePosition = frameToSet;
                }
                if(abs(frameToSet - samplePosition) > 10000){
                    samplePosition = frameToSet;
                }
                bNewFrame = true;
            }
        }
    //}
    if(bNewFrame){
        updateAudio();
        /*
        if(activeClips.size() > 0){
            memcpy(sndBuffer, (*activeClips.begin())->getSamples(), 44100 / 24 * sizeof(float));
            /*
            for(cIter = activeClips.begin() + 1; cIter != activeClips.end(); ++cIter){
                compositeFrames(frameBuffer, (*cIter)->getPixels(), 854, 480, 3, (*cIter)->hasCrossfade());
            }
            */
            //memcpy(output, frameBuffer, 854 * 480 * 3);
            /*
            bHaveNewFrame = true;
            samplePosition = 0;
        }
        */
    }
    previousFrame = _frame;
}

void Timeline::updateAudio(){
    //memcpy(sndBuffer, (*activeClips.begin())->getSamples(), 44100 / 24 * sizeof(float));
    bHaveNewFrame = true;
}

void Timeline::compositeFrames(unsigned char* _one, unsigned char* _two, int _w, int _h, int _bd, bool _blend){
    for(int y = 0; y < _h; y++){
        //cout << "y = " << y << endl;
        for(int x = 0; x < _w * _bd; x++){
            int index = y * (_w * _bd) + x;
            if(_blend){
                _one[index] = (_one[index] + _two[index]) / 2;
                _one[index + 1] = (_one[index + 1] + _two[index + 1]) / 2;
                _one[index + 2] = (_one[index + 2] + _two[index + 2]) / 2;
            } else {
                int sum = _two[index] + _two[index + 1] + _two[index + 3];
                if(sum != 0){
                    _one[index] = _two[index];
                    _one[index + 1] = _two[index + 1];
                    _one[index + 2] = _two[index + 2];
                }
            }
        }
    }
}

void Timeline::removeClip(string _id){
    cout << "timeline asked to remove clip - " << _id << endl;
    cout << "currently have " << clips.size() << " items" << endl;
    if(currentClip != NULL){
        if(currentClip->getID() == _id){
            currentClip = NULL;
            cout << "blacking out compositor." << endl;
            Compositor::Instance()->blackOut();
        }
    }
    int framesToAdjust = 0;
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end();){
        string tmpID = (*cIter)->getID();
        cout << "checking id against " << tmpID << endl;
        if(tmpID == _id){
            //(*cIter)->stop();
            activeClips.clear();
            cout << "timeline removing clip" << endl;
                //MediaCabinet::Instance()->removeClip((*cIter));
            int tmpFrameCnt = numFrames;
            numFrames -= (*cIter)->getTotalNumFrames();
            int timeRemaining = (int)(60 - (numFrames / 24));
            attrs["time"] = ofToString(timeRemaining);
            SubObMediator::Instance()->update("time-remaining",this);
            framesToAdjust = numFrames - tmpFrameCnt;
            delete (*cIter);
            cIter = clips.erase(cIter);
            //MediaCabinet::Instance()->decreaseClipHold((*cIter)->getClip());
            //break;
        } else {
            (*cIter)->adjustStartFrame(framesToAdjust);
            cIter++;
        }
    }
    if(clips.size() == 0){
        Compositor::Instance()->blackOut();
    }
}

void Timeline::setupRun(){
    currentClip = clips[0];
}

unsigned char* Timeline::getNextFrame(){
    //cout << "timeline getting next frame" << endl;
    if(currentClip){
        return currentClip->getNextFrame();
    }
}

void Timeline::setScrubTime(float _pct){
    float timeToSet = totalTime * _pct;
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end();){
        float startTime = (*cIter)->getStartTime();
        float stopTime = (*cIter)->getStopTime();
        if(timeToSet > stopTime){
            ++cIter;
        } else {
            float newTime = timeToSet - startTime;
            float newPct = 1.0 / ((stopTime - startTime) / newTime);
            //cout << "setting clip scrubtime to " << newPct << " from " << startTime << " to " << stopTime << endl;
            (*cIter)->setPosition(newPct);
            break;
        }
    }
}

int Timeline::getEffectStatus(string _name){
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        if((*cIter)->getID() == _name){
            return (*cIter)->getEffectStatus();
        }
    }
    return -1;
}

void Timeline::setEffectStatus(string _name, int _effectStatus){
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        if((*cIter)->getID() == _name){
            (*cIter)->setEffectStatus(_effectStatus);
        }
    }
}

void Timeline::start(){
    isPlaying = true;
    /*
    if( clips.size() > 0){
        startTime = ofGetElapsedTimef();
        isPlaying = true;
        clipIndex = 0;
        currentClip = clips[clipIndex];
        cout << "timeline starting" << endl;
        currentClip->play(0.0);
    }
    */
}

void Timeline::stop(){
    isPlaying = false;
}

void Timeline::pause(){
    isPlaying = false;
    /*
    if(currentClip){
        currentClip->pause();
    }
    */
}

void Timeline::rewind(){
    isPlaying = true;
    setFrame(0);
}

void Timeline::unpause(){
    if(currentClip){
        currentClip->unpause();
    }
}

void Timeline::restart(){
    startTime = ofGetElapsedTimef();
    isPlaying = true;
}

void Timeline::reset(){
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end();){
        //(*cIter)->reset();
        delete (*cIter);
        cIter = clips.erase(cIter);
    }
    currentClip = NULL;
}

void Timeline::clear(){
    currentClip = NULL;
    clips.clear();
}

bool Timeline::getIsEmpty(){
    return clips.empty();
}

string Timeline::getType(){
    return clips.front()->getType();
}

void Timeline::update(){
    if(mode == "video"){
        setStartFrames();
    }
}

void Timeline::update(float _runningTime){
}

void Timeline::updateRun(){
    if(currentClip->isDone()){
        if(clipIndex < clips.size() - 1){
            clipIndex++;
            currentClip = clips[clipIndex];
        }
    }
}

void Timeline::draw(int _x, int _y, int _sx, int _sy){
    /*
    vector<Clip*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); cIter++){
        //if(runningTime > (*cIter)->getStartTime() && runningTime < (*cIter)->getStopTime()){
        (*cIter)->draw(_x,_y,_sx,_sy);
        //}
    }
    */
    if(currentClip){
        currentClip->draw(_x,_y,_sx,_sy);
        if(isTransitioning){
            clips[clipIndex + 1]->draw(_x,_y,_sx,_sy);
        }
    }
}

int Timeline::getTotalLength(){
    vector<ClipContainer*>::iterator cIter;
    totalLength = 0;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        totalLength += (*cIter)->getDataLength();
    }
    return totalLength;
}

void Timeline::assembleData(){
    //delete[] allData;
    cout << "writing from total length " << totalLength << endl;
    /*
    allData = (float*)malloc(totalLength * sizeof(float));
    memset(allData,0,totalLength * sizeof(float));
    */
    allData = (float*)malloc(totalLength * sizeof(float));
    float* allDataPosition = allData;
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        //int length = (*cIter)->getDataLength();
        //memcpy(allData, (*cIter)->getData(), length * sizeof(float));
        //allData += length;
        //cout << "allData appended by " << length << endl;
        float* ptr = (*cIter)->getData();
        int len = (*cIter)->getDataLength();
        memcpy(allDataPosition, ptr, len * sizeof(float));
        allDataPosition += len;
    }
}


