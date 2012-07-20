#include "timeline.h"
#include "MediaCabinet.h"
#include "Compositor.h"
#include "sndfile.h"

Timeline::Timeline(float _totalTime){
    totalTime = _totalTime; //Most likely 60.0
    timeRemaining = totalTime;
    runningTime = 0.0;
    isPlaying = false;
    clipIndex = 0;
    currentClip = NULL;
    isTransitioning = false;
    setScrubTime(0.0);
}

void Timeline::addClip(Clip *_clip){
    cout << "adding a clip of type - " << _clip->getType() << endl;
    /*
    float lastStopTime = 0.0;
    if(!clips.empty()){
        lastStopTime = clips.back()->getStopTime();
    }
    */
    /*
    if(_clip->getType() == "video"){
        _clip->addEffect("wipe");
    }
    */
    clips.push_back(new ClipContainer(_clip));
    MediaCabinet::Instance()->increaseClipHold(_clip);
    //clips.back()->setStartTime(lastStopTime);
    /*  Commented out to test initialization on start.
    if(!currentClip){
        currentClip = clips[0];
    }
    */
    //totalTime += clips.back()->getDuration();
}

void Timeline::addClip(Clip* _clip, string _id){
    if(clips.size() > 0){
        unsigned char * holder = clips.back()->getLastFrame();
        addClip(_clip);
        MediaCabinet::Instance()->increaseClipHold(_clip);
        clips.back()->setID(_id);
        clips.back()->setTransitionFrame(holder);
    } else {
        addClip(_clip);
        MediaCabinet::Instance()->increaseClipHold(_clip);
        clips.back()->setID(_id);
    }
    //timeRemaining -= clips.back()->getDuration;
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

void Timeline::removeClip(string _id){
    cout << "timeline asked to remove clip" << endl;
    if(currentClip != NULL){
        if(currentClip->getID() == _id){
            currentClip = NULL;
            cout << "blacking out compositor." << endl;
            Compositor::Instance()->blackOut();
        }
    }
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        string tmpID = (*cIter)->getID();
        if(tmpID == _id){
            (*cIter)->stop();
            cout << "timeline removing clip" << endl;
            //MediaCabinet::Instance()->removeClip((*cIter));
            clips.erase(cIter);
            if((*cIter)->getType() == "video" && clips.size() < 1){
                cout << "clips empty.  blacking out video" << endl;
                Compositor::Instance()->blackOut();
            }
            MediaCabinet::Instance()->decreaseClipHold((*cIter)->getClip());
            break;
        }
    }
    for(int i = 0; i < clips.size(); i++){
        clips[i]->releaseTransitionFrame();
        if(i > 0){
            clips[i]->setTransitionFrame(clips[i - 1]->getLastFrame());
        }
    }
    //clips.front()->releaseTransitionFrame();
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
    if(/*currentClip*/ clips.size() > 0){
        startTime = ofGetElapsedTimef();
        isPlaying = true;
        clipIndex = 0;
        currentClip = clips[clipIndex];
        cout << "timeline starting" << endl;
        currentClip->play(0.0);
    }
}

void Timeline::stop(){
    isPlaying = false;
}

void Timeline::pause(){
    if(currentClip){
        currentClip->pause();
    }
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
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        (*cIter)->reset();
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
    //cout << "running time = " << runningTime << endl;
    /*
    if(isPlaying){
        runningTime = ofGetElapsedTimef() - startTime;
        if(runningTime > totalTime){
            startTime = ofGetElapsedTimef();
            this->update();
        }
        vector<Clip*>::iterator cIter;
        for(cIter = clips.begin(); cIter != clips.end(); cIter++){
            (*cIter)->update(runningTime);
        }
    }
    */
    if(currentClip){
        //currentClip->update(runningTime);
        currentClip->update();
        /*
        if(currentClip->isDone()){
            currentClip->stop();
            clipIndex = clipIndex + 1 % clips.size();
            currentClip = clips[clipIndex];
            currentClip->play();
            //currentClip->play();
        }
        */
        if(currentClip->getStopTime() < ofGetElapsedTimef()){
            cout << "current clip done, advancing." << endl;
            currentClip->stop();
            if(clipIndex <= clips.size() - 1){
                cout << "increasing currentClip." << endl;
                clipIndex++;
                currentClip = clips[clipIndex];
                currentClip->play();
            } else {
                currentClip = NULL;
                cout << "end of timeline.  blacking out compositor" << endl;
                Compositor::Instance()->blackOut();
            }
        }
    }
}

void Timeline::update(float _runningTime){
    if(currentClip){
        currentClip->update();
        //cout << "time remaining on clip = " << currentClip->getTimeRemaining() << endl;
        if(!isTransitioning){
            /*
            if(currentClip->getTimeRemaining() < 0.5){
                if(clipIndex < clips.size() - 1){
                    cout << "turning on transition" << endl;
                    isTransitioning = true;
                    clips[clipIndex + 1]->play();
                }
            }
            */
        } else {
            //clips[clipIndex + 1]->update();
        }
        /*if(currentClip->getStopTime() < _runningTime){*/
        if(currentClip->isDone()){
            cout << "current clip done, advancing." << endl;
            currentClip->stop();
            if(clipIndex < clips.size() - 1){
                clipIndex++;
                currentClip = clips[clipIndex];
                if(!isTransitioning){
                    currentClip->play(_runningTime);
                }
                isTransitioning = false;
            } else {
                currentClip = NULL;
                Compositor::Instance()->blackOut();
            }
        }
    }
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


