#include "Timeline.h"
#include "MediaCabinet.h"
#include "Compositor.h"
//#include "sndfile.h"
#include "ClipContainer.h"
#include "SceneManager.h"

Timeline::Timeline(float _totalTime, string _name, string _mode){
}

Timeline::Timeline(float _totalTime,
                   string _name,
                   string _mode,
                   string _pos)
{
    /*
    Necessary video buffers.
    */
    frameBuffer = new unsigned char[854 * 480 * 3];
    output = new unsigned char[854 * 480 * 3];
    memset(frameBuffer, 0, 854 * 480 *3);

    /*
    Set the Timeline mode.
    */

    mode = _mode;

    /*
    Set have new frame to false for video. True for audio.
    */

    if(mode == "video"){
        bHaveNewFrame = true;
    } else {
        bHaveNewFrame = false;
    }

    /*
    Setup audio system.
    */

    if(mode == "audio"){
        stream = new ofRtAudioSoundStream;
        stream->setup(this,1,0,44100,256, 32);
    }

    /*
    Initialize member variables.
    */

    previousFrame = 0;
    samplePosition = 0;
    isPlaying = false;
    numFrames = 0;
    timeRemaining = _totalTime;
    totalTime = _totalTime;
    basePos = stringToVec2f(_pos);
    bHaveDeadClips = false;

    /*
    Observer
    */

    SubObMediator::Instance()->addObserver("button",this);
    SubObMediator::Instance()->addObserver("mouse-changed",this);

    bRecentChange = false;
    bCompositing = false;
}

unsigned char * Timeline::getPixels(){
    bHaveNewFrame = false;
    return output;
}

bool Timeline::haveClips(){
    if(clips.size() > 0){
        return true;
    } else {
        return false;
    }
}

void Timeline::addClip(Clip *_clip){
    cout << "adding a clip of type - " << _clip->getType() << endl;
    clips.push_back(new ClipContainer(_clip));
    MediaCabinet::Instance()->increaseClipHold(_clip);
    clips.back()->setStartFrame(numFrames);
    numFrames += clips.back()->getTotalNumFrames();
    int timeRemaining = (int)(60 - (numFrames / 24));
    attrs["time"] = ofToString(timeRemaining);
    SubObMediator::Instance()->update("time-remaining",this);
    bRecentChange = true;
    setStartFrames();
}

void Timeline::audioRequested(float * output, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; i++){
        if(isPlaying && activeClips.size() > 0){                                            // Is the timeline currently?  And are there clips to play?
            output[i] = (*activeClips.begin())->getSample(samplePosition++);                // Set the output buffer to the appropriate sample.
            if(samplePosition > currentMaxFrames){                                          // If end turn over.
                samplePosition = 0;
                bHaveNewFrame = false;
            }
        } else {
            output[i] = 0.0;
        }
    }
}

void Timeline::removeClip(ClipContainer *_clip){
    //out << "timeline asked to remove clip" << endl;
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        if((*cIter) == _clip){
            //(*cIter)->stop();
            //cout << "timeline removing clip" << endl;
            //MediaCabinet::Instance()->removeClip((*cIter));
            numFrames -= clips.back()->getTotalNumFrames();
            int timeRemaining = (int)(60 - (numFrames / 24));
            attrs["time"] = ofToString(timeRemaining);
            SubObMediator::Instance()->update("time-remaining",this);
            MediaCabinet::Instance()->decreaseClipHold((*cIter)->getClip());
            clips.erase(cIter);
            activeClips.clear();
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
        //cout << "setting start frames." << endl;
        if((*cIter)->needsNudge() && cIter != clips.begin()){
            prevIter = cIter;
            prevIter--;
            sFrame -= 24;
            (*cIter)->adjustButton();
            (*prevIter)->adjustButton();
        } else {
            (*cIter)->resetButton();
            (*cIter)->resetButton();
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
        //removeClip(_sub->getAttr("clip-to-remove"));
        SubObMediator::Instance()->update("update-timeline-framecount",this);
    }
    if(_subName == "button"){
        if(_sub->getAttr("name") == "trash"){
            bHaveDeadClips = true;
        }
    }
    if(_subName == "mouse-changed" && _sub->getAttr("mouseState") == "down"){
        if(SceneManager::Instance()->getTopSheet()->getName() == "main"){
            if(!bRecentChange){
                cout << "checking for mouse interaction." << endl;
                vector<ClipContainer*>::iterator cIter;
                for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
                    if((*cIter)->isInside(ofToInt(_sub->getAttr("mouseX")), ofToInt(_sub->getAttr("mouseY")))){
                        (*cIter)->execute();
                    }
                }
            } else {
                bRecentChange = false;
            }
        }
    }
}

void Timeline::clean(){
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        if((*cIter)->getIsDead()){
            //removeClip((*cIter));
            numFrames -= (*cIter)->getTotalNumFrames();
            MediaCabinet::Instance()->decreaseClipHold((*cIter)->getClip());
            delete (*cIter);
            clips.erase(cIter);
            int timeRemaining = (int)(60 - (numFrames / 24));
            attrs["time"] = ofToString(timeRemaining);
            SubObMediator::Instance()->update("time-remaining",this);
            setStartFrames();
            break;
        }
    }
    //bHaveDeadClips = false;
}

void Timeline::setFrame(int _frame){
    //cout << "mode = " << mode << endl;
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
            //frameBuffer = (*activeClips.begin())->getPixels();
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
                if(abs(frameToSet - samplePosition) > 1000){
                    samplePosition = frameToSet;
                }
                bNewFrame = true;
            }
        }
    //}
    if(bNewFrame){
        bHaveNewFrame = true;
        (*activeClips.begin())->setFrame(samplePosition);
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

void Timeline::draw(){
    ofVec2f drawPos = basePos;
    drawPos.y += 5;
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        (*cIter)->setPosition(drawPos);
        drawPos.x += (*cIter)->getSize().x - 3;
        (*cIter)->draw();
    }
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

/*
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
*/

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

void Timeline::reset(){
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end();){
        //(*cIter)->reset();
        delete (*cIter);
        cIter = clips.erase(cIter);
    }
}

bool Timeline::getIsEmpty(){
    return clips.empty();
}

string Timeline::getType(){
    return clips.front()->getType();
}

void Timeline::update(){
    /*
    if(mode == "video"){
        setStartFrames();
    }
    */
    if(bHaveDeadClips){
        bHaveDeadClips = false;
        cout << "cleaning timeline. " << endl;
        clean();
    }
    vector<ClipContainer*>::iterator cIter;
    for(cIter = clips.begin(); cIter != clips.end(); ++cIter){
        //(*cIter)->reset();
        if((*cIter)->hasNewEffect()){
            setStartFrames();
        }
    }
    //setStartFrames();
    //setStartFrames();
}



