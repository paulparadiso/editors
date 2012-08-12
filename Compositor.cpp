#include "Compositor.h"
#include "GuiConfigurator.h"

Compositor* Compositor::mInstance = NULL;

Compositor* Compositor::Instance(){
    if(!mInstance){
        mInstance = new Compositor();
    }
    return mInstance;
}

Compositor::Compositor(){
    playState = PLAY_STATE_STOPPED;
    masterTex = new ofTexture;
    masterTex->allocate(854,480,GL_RGB);
    black = new unsigned char[854 * 480 * 3];
    masterBuffer = new unsigned char[854 * 480 * 3];
    memset(black,0,854 * 480 * 3);
    blackOut();
    isRecording = false;
    saver.listCodecs();
    scrubberPosition = 0.0;
    maxRecordBuffer = 44100 * 30;
    playPosition = 0;
    recordPosition = 0;
    recordState = RECORD_STATE_STOPPED;
    recordBuffer = new float[maxRecordBuffer];
    stream.setup(this,1,1,44100,256,4);
    stream.stop();
    bRunningTimeline = false;
    clipCounter = 0;
    framerate = 24;
    maxNumFrames = 24 * 60;
    numFrames = 0;
    currentFrame = 0;
    timer = new Timer(framerate);
    SubObMediator::Instance()->addObserver("clip-selected", this);
    SubObMediator::Instance()->addObserver("button", this);
    SubObMediator::Instance()->addObserver("timer-tick", this);
    SubObMediator::Instance()->addObserver("update-timeline-framecount", this);
    SubObMediator::Instance()->addObserver("sheet-changed", this);
    SubObMediator::Instance()->addObserver("scrubber-position-changed", this);
    SubObMediator::Instance()->addObserver("reset", this);
    videoTimeline = NULL;
    bSetPause = false;
    bHaveVideoTimeline = false;
}

Compositor::~Compositor(){
}

void Compositor::vRender(){
    pause();
    //GuiConfigurator::Instance()->openSheet("info");
    for(int i = 0; i < numFrames; i++){
        ofSleepMillis(10);
    }
    GuiConfigurator::Instance()->closeSheet("info");
    if(bHaveVideoTimeline){
        videoTimeline->reset();
    }
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->reset();
    }
    SubObMediator::Instance()->update("reset",this);
    SceneManager::Instance()->reset();
    currentFrame = 0;
}

void Compositor::update(string _subName, Subject* _sub){
    if(_subName == "clip-selected"){
        string clipName = _sub->getAttr("path");
        cout << "Compositor asked to add " << clipName << " to " << activeTimeline << endl;
        addClipToTimeline(activeTimeline, MediaCabinet::Instance()->getClip(clipName));
    }
    if(_subName == "reset"){
        if(bHaveVideoTimeline){
        videoTimeline->reset();
        }
        map<string,Timeline*>::iterator tIter;
        for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
            tIter->second->reset();
        }
        SceneManager::Instance()->reset();
        currentFrame = 0;
        blackOut();
    }
    if(_subName == "button"){
        cout << "compositor got an action of " << _sub->getAttr("action") << endl;
        if(_sub->getAttr("target-type") == "timeline"){
            activeTimeline = _sub->getAttr("name");
        }
        if(_sub->getAttr("action") == "control"){
            string cmd = _sub->getAttr("name");
            if(cmd == "rewind"){
                cout << "Compositor rewinding" << endl;
                rewind();
            }
            if(cmd == "play"){
                cout << "Compositor playing." << endl;
                start();
            }
            if(cmd == "pause"){
                cout << "Compositor pausing." << endl;
                pause();
            }
            if(cmd == "recorder-record"){
                cout << "Compositor recording." << endl;
                startRecording();
            }
            if(cmd == "recorder-play"){
                cout << "Compositor playing recording." << endl;
                playRecording();
            }
            if(cmd == "recorder-stop"){
                cout << "Compositor stopping recording." << endl;
                stopRecording();
            }
            if(cmd == "recorder-save"){
                cout << "Compositor saving recording." << endl;
                saveRecording();
            }
        }
        if(_sub->getAttr("name") == "render-button"){
                cout << "rendering" << endl;
                vRender();
            }
    }
    if(_subName == "timer-tick"){
        //cout << "Timer Tick" << endl;
        if(numFrames > 0){
            currentFrame = (currentFrame + 1) % numFrames;
            if(bHaveVideoTimeline){
                if(!videoTimeline->isCompositing()){
                    videoTimeline->setFrame(currentFrame);
                }
            }
            map<string,Timeline*>::iterator tIter;
            for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
                tIter->second->setFrame(currentFrame);
            }
        }
        float timePos = (float)currentFrame / (float)maxNumFrames;
        setAttr("time-pos",ofToString(timePos));
        SubObMediator::Instance()->update("timeline-position-changed",this);
    }
    if(_subName == "update-timeline-framecount"){
        /*
        numFrames = videoTimeline->getNumFrames();
        cout << "updated numFrames to " << numFrames << endl;
        */
        updateFrameCount();
    }
    if(_subName == "sheet-changed"){
        pause();
    }
    if(_subName == "scrubber-position-changed"){
        float inPct = ofToFloat(_sub->getAttr("time-pos"));
        currentFrame = maxNumFrames * inPct;
        if(bHaveVideoTimeline){
            if(currentFrame < numFrames && numFrames > 0){
                videoTimeline->setFrame(currentFrame);
            } else {
                blackOut();
            }
        }
        map<string,Timeline*>::iterator tIter;
        for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
            tIter->second->setFrame(currentFrame);
        }
    }
}

void Compositor::updateFrameCount(){
    if(bHaveVideoTimeline){
        numFrames = videoTimeline->getNumFrames();
    } else {
        numFrames = 0;
    }
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        //tIter->second->setScrubTime(_pct);
        int aNumFrames = tIter->second->getNumFrames();
        if(aNumFrames > numFrames){
            numFrames = aNumFrames;
        }
    }
    //cout << "updated num frames to " << numFrames << endl;
}

void Compositor::runTimeline(string _movFile){
    rewind();
    //numFrames = timelines["video-timeline"]->getTotalLength();
    frameCounter = 0;
    //timelines["video-timeline"]->setupRun();
    bRunningTimeline = true;
    saveVideo(_movFile);
    //blackOut();

}

void Compositor::audioReceived(float* input, int bufferSize, int nChannels){
    if(recordState == RECORD_STATE_RECORDING){
        for(int i = 0; i < bufferSize; i++){
            /*
            memcpy(rcdPosition,input,bufferSize * sizeof(float));
            rcdPosition += bufferSize;
            samplesRecorded += bufferSize;
            */
            if(recordPosition < maxRecordBuffer){
                recordBuffer[recordPosition++] = input[i];
            } else {
                recordState = RECORD_STATE_STOPPED;
            }
        }
    }
}

void Compositor::audioRequested(float * output, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; i++){
        if(recordState == RECORD_STATE_PLAYING){
            output[i] = recordBuffer[playPosition++];
            if(playPosition >= recordPosition){
                recordState = RECORD_STATE_STOPPED;
            }
        } else {
            output[i] = 0.0;
        }
    }
}

float Compositor::getRecordingPosition(){
    if(recordState == RECORD_STATE_RECORDING){
        return (float)recordPosition / (float)maxRecordBuffer;
    } else if(recordState == RECORD_STATE_PLAYING){
        return (float)playPosition / (float)recordPosition;
    } else {
        return 0.0;
    }
}

void Compositor::addTimeline(string _name, string _mode){
    if(_mode == "video"){
        bHaveVideoTimeline = true;
        videoTimeline = new Timeline(60.0, _name, _mode);
        videoTimelineName = _name;
    }
    if(_mode == "audio"){
        audioTimelines[_name] = new Timeline(60.0, _name, _mode);
    }
}

void Compositor::blackOut(){
    masterTex->loadData(black, 854, 480, GL_RGB);
    //memset(masterBuffer,0,854 * 480 * 3);
}

void Compositor::addClipToTimeline(string _timeline, Clip* _clip){
    string clipID = _clip->getName() + ofToString(clipCounter++);
    if(bHaveVideoTimeline){
        if(_timeline == videoTimelineName){
            videoTimeline->addClip(_clip, clipID);
        //numFrames = videoTimeline->getNumFrames();
            cout << "numFrames = " << numFrames << endl;
        } else {
        audioTimelines[_timeline]->addClip(_clip, clipID);
        }
    } else {
        audioTimelines[_timeline]->addClip(_clip, clipID);
    }
    setAttr("new-clip-id", clipID);
    setAttr("new-clip-length", ofToString(_clip->getDuration()));
    setAttr("active-timeline", activeTimeline);
    setAttr("new-clip-mode", _clip->getType());
    SubObMediator::Instance()->update("new-timeline-clip", this);
    //timelines[_timeline]->start();
    //updateFrameCount();
}

void Compositor::addClipToTimeline(string _timeline, Clip* _clip, string _id){
    cout << "adding clip " << _id << " to timeline " << _timeline << endl;
    //timelines[_timeline]->addClip(_clip, _id);
    //timelines[_timeline]->start();
    if(bHaveVideoTimeline){
        if(_timeline == videoTimelineName){
            videoTimeline->addClip(_clip, _id);
        //numFrames = videoTimeline->getNumFrames();
        } else {
            audioTimelines[_timeline]->addClip(_clip, _id);
        }
    } else {
        audioTimelines[_timeline]->addClip(_clip, _id);
    }
    //updateFrameCount();
}

void Compositor::removeClipFromTimeline(string _timeline, Clip* _clip){
}

void Compositor::removeClipFromTimeline(string _timeline, string _id){
}

void Compositor::setScrubberPosition(float _pct){
}

int Compositor::getEffectStatus(string _name){
}

void Compositor::setEffectStatus(string _name, int _effectStatus){
}

void Compositor::update(){
    //numFrames = videoTimeline->getNumFrames();
    //updateFrameCount();
    if(bHaveVideoTimeline){
        if(videoTimeline->hasNewFrame()){
            if(!videoTimeline->isCompositing()){
                masterTex->loadData(videoTimeline->getPixels(), 854, 480, GL_RGB);
            }
        }
        if(videoTimeline->getNumFrames() < currentFrame){
            blackOut();
        }
        videoTimeline->update();
    }
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->update();
    }
    if(bSetPause){
        pause();
        bSetPause = false;
    }
    updateFrameCount();
}

void Compositor::start(){
    timer->start();
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->start();
    }
}

void Compositor::pause(){
    timer->stop();
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->pause();
    }
}

void Compositor::stop(){
    timer->stop();
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->pause();
    }
}

void Compositor::rewind(){
    currentFrame = 0;
    if(bHaveVideoTimeline){
        videoTimeline->rewind();
    }
    setAttr("time-pos",ofToString(0));
    SubObMediator::Instance()->update("timeline-position-changed",this);
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->rewind();
    }
    bSetPause = true;
}

void Compositor::startRecording(){
    cout << "compositor starting recording" << endl;
    stream.start();
    recordPosition = 0;
    recordState = RECORD_STATE_RECORDING;
}

void Compositor::stopRecording(){
    stream.stop();
    cout << "compositor stopping recording" << endl;
    recordState = RECORD_STATE_STOPPED;
}

void Compositor::playRecording(){
    cout << "compositor playing recording" << endl;
    if(recordState == RECORD_STATE_STOPPED){
        stream.start();
        cout << "compositor playing recording" << endl;
        playPosition = 0;
        recordState = RECORD_STATE_PLAYING;
    }

}

void Compositor::saveRecording(){
    string date = ofGetTimestampString();
    string wavFile = "audio/user/" + date + ".wav";
    string xmlFile = "audio/user/" + date + ".xml";
    cout << "compositor saving recording as " << wavFile << endl;
    audioRenderer.writeFile(wavFile,recordBuffer,recordPosition);
    ofxXmlSettings xml;
    xml.loadFile(xmlFile);
    xml.addValue("type", "audio");
    xml.addValue("path", wavFile);
    xml.addValue("duration", (float)recordPosition / 44100.0);
    xml.saveFile();
    map<string,string>msg;
    msg["target"] = "user-pager";
    msg["action"] = "reload";
    SubObMediator::Instance()->update("audio-recording-complete", this);
    GuiConfigurator::Instance()->deliverMessage(msg);
    SceneManager::Instance()->popSheet();
}

void Compositor::draw(int _x, int _y, int _sx, int _sy){
    if(bHaveVideoTimeline){
        masterTex->draw(_x, _y, _sx, _sy);
    }
}

void Compositor::render(){
    GuiConfigurator::Instance()->openSheet("render");
    string timeBase = ofGetTimestampString();
    string wavFile = "render/audio/" + timeBase + ".wav";
    string movFile = "render/video/" + timeBase + ".mov";
    saveAudio(wavFile);
    runTimeline(movFile);

}

void Compositor::saveAudio(string _fileName){
    /*
    vector<Timeline*> audioTimelines;
    map<string,Timeline*>::iterator tIter;
    for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
        if(!tIter->second->getIsEmpty()){
            if(tIter->second->getType() == "audio"){
                audioTimelines.push_back(tIter->second);
            }
        }
    }
    if(!audioTimelines.empty()){
        audioRenderer.renderAudio(_fileName, audioTimelines);
    }
    */
}

void Compositor::saveVideo(string _fileName){
    /*
    if(isRecording){
        saver.finishMovie();
        isRecording = false;
        cout << "stopping recording" << endl;
    } else {
        //string movFile = _fileName;
        cout << "starting recording" << endl;
        saver.setup(854,480,_fileName);
        saver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_LOSSLESS);
        saver.setCodecType(18);
    }
    */
}

string Compositor::makeDateName(){
    string name = "";
    name += ofGetYear();
    name += "-";
    name += ofGetMonth();
    name += "-";
    name += ofGetDay();
    name += "-";
    name += ofGetHours();
    name += ":";
    name += ofGetMinutes();
    name += ":";
    name += ofGetSeconds();
    return name;
}
