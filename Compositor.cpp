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
    masterTex->allocate(853,480,GL_RGB);
    black = new unsigned char[853 * 480 * 3];
    masterBuffer = new unsigned char[853 * 480 * 3];
    memset(black,0,853 * 480 * 3);
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
    videoTimeline = NULL;
}

Compositor::~Compositor(){
}

void Compositor::update(string _subName, Subject* _sub){
    if(_subName == "clip-selected"){
        string clipName = _sub->getAttr("path");
        cout << "Compositor asked to add " << clipName << " to " << activeTimeline << endl;
        addClipToTimeline(activeTimeline, MediaCabinet::Instance()->getClip(clipName));
    }
    if(_subName == "button"){
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
        }
    }
    if(_subName == "timer-tick"){
        //cout << "Timer Tick" << endl;
        if(numFrames > 0){
            currentFrame = (currentFrame + 1) % numFrames;
            videoTimeline->setFrame(currentFrame);
        }
    }
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
        videoTimeline = new Timeline(60.0, _name);
        videoTimelineName = _name;
    }
    if(_mode == "audio"){
        audioTimelines[_name] = new Timeline(60.0, _name);
    }
}

void Compositor::blackOut(){
    //masterTex->loadData(black, 853, 480, GL_RGB);
    memset(masterBuffer,0,853 * 480 * 3);
}

void Compositor::addClipToTimeline(string _timeline, Clip* _clip){
    string clipID = _clip->getName() + ofToString(clipCounter++);
    if(_timeline == videoTimelineName){
        videoTimeline->addClip(_clip, clipID);
        numFrames = videoTimeline->getNumFrames();
        cout << "numFrames = " << numFrames << endl;
    } else {
        audioTimelines[_timeline]->addClip(_clip, clipID);
    }
    setAttr("new-clip-id", clipID);
    setAttr("new-clip-length", ofToString(_clip->getDuration()));
    setAttr("active-timeline", activeTimeline);
    setAttr("new-clip-mode", _clip->getType());
    SubObMediator::Instance()->update("new-timeline-clip", this);
    //timelines[_timeline]->start();

}

void Compositor::addClipToTimeline(string _timeline, Clip* _clip, string _id){
    cout << "adding clip " << _id << " to timeline " << _timeline << endl;
    //timelines[_timeline]->addClip(_clip, _id);
    //timelines[_timeline]->start();
    if(_timeline == videoTimelineName){
        videoTimeline->addClip(_clip, _id);
        numFrames = videoTimeline->getNumFrames();
    } else {
        audioTimelines[_timeline]->addClip(_clip, _id);
    }
}

void Compositor::removeClipFromTimeline(string _timeline, Clip* _clip){
    /*
    timelines[_timeline]->removeClip(_clip);
    rewind();
    stop();
    */
}

void Compositor::removeClipFromTimeline(string _timeline, string _id){
    /*
    timelines[_timeline]->removeClip(_id);
    rewind();
    stop();
    */
}

void Compositor::setScrubberPosition(float _pct){
    /*
    map<string,Timeline*>::iterator tIter;
    for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
        tIter->second->setScrubTime(_pct);
    }
    scrubberPosition = _pct;
    */
}

int Compositor::getEffectStatus(string _name){
    /*
    map<string,Timeline*>::iterator tIter;
    for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
        int r = tIter->second->getEffectStatus(_name);
        if(r > -1){
            return r;
        }
    }
    return 0;
    */
}

void Compositor::setEffectStatus(string _name, int _effectStatus){
    /*
    cout << "setting effect status of " << _name << endl;
    map<string,Timeline*>::iterator tIter;
    for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
        tIter->second->setEffectStatus(_name, _effectStatus);
    }
    */
}

/*
void Compositor::update(){
    float tmpCurrentTime = ofGetElapsedTimef();
    map<string,Timeline*>::iterator tIter;
    if(!bRunningTimeline){
        for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
            tIter->second->update(runningTime);
        }
        if(playState == PLAY_STATE_PLAYING){
            runningTime += tmpCurrentTime - timeOfLastUpdate;
            scrubberPosition = runningTime / 60.0;
            if(runningTime > 60.0){
                stop();
            }
        }
        timeOfLastUpdate = tmpCurrentTime;
    } else {
        timelines["video-timeline"]->updateRun();
    }
    if(isRecording){
        //if(ofGetElapsedTimef() - timeOfLastFrame > 1.0 / 30.0){
            //ofPixels pixels;
            //masterTex->readToPixels(pixels);
            //cout << "writing data to video" << endl;
            //ofPixels pixels;
            //masterTex->readToPixels(pixels);
            saver.addFrame(masterBuffer,1/ofGetFrameRate());
        //    timeOfLastFrame = ofGetElapsedTimef();
        //}
        if(ofGetElapsedTimef() - recordingStartTime > 60.0){
            saver.finishMovie();
            isRecording = false;
        }
    }
}
*/

void Compositor::update(){
    if(videoTimeline){
        if(videoTimeline->hasNewFrame()){
            cout << "updating Compositor" << endl;
            masterTex->loadData(videoTimeline->getPixels(), 853, 480, GL_RGB);
        }
    }
}

void Compositor::start(){
    /*
    switch(playState){
        case PLAY_STATE_STOPPED:{
            runningTime = 0.0;
            playState = PLAY_STATE_PLAYING;
            map<string,Timeline*>::iterator tIter;
            for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
                tIter->second->start();
            }
            break;
        }
        case PLAY_STATE_PAUSED:{
            playState = PLAY_STATE_PLAYING;
            map<string,Timeline*>::iterator tIter;
            for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
                tIter->second->unpause();
            }
            break;
        }
        default:
            break;
    }
    */
    timer->start();
}

void Compositor::pause(){
    /*
    cout << "pause called on Compositor. playState = " << playState << endl;
    if(playState == PLAY_STATE_PLAYING){
        cout << "Compositor pausing." << endl;
        playState = PLAY_STATE_PAUSED;
        map<string,Timeline*>::iterator tIter;
            for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
            tIter->second->pause();
        }
    } else {
        cout << "Compositor NOT pausing." << endl;
    }
    */
    timer->stop();
}

void Compositor::stop(){
    /*
    map<string,Timeline*>::iterator tIter;
    for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
        tIter->second->stop();
    }
    playState = PLAY_STATE_STOPPED;
    scrubberPosition = 0.0;
    */
    timer->stop();
    currentFrame = 0;

}

void Compositor::rewind(){
    /*
    map<string,Timeline*>::iterator tIter;
    for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
        tIter->second->restart();
    }
    startTime = ofGetElapsedTimef();
    */
    /*
    map<string,Timeline*>::iterator tIter;
    for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
        tIter->second->reset();
    }
    stop();
    */
    //playState = PLAY_STATE_STOPPED;
    //start();
    currentFrame = 0;
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
    GuiConfigurator::Instance()->deliverMessage(msg);
    SceneManager::Instance()->popSheet();
}

/*
void Compositor::draw(int _x, int _y, int _sx, int _sy){
    if(timelines.size() < 1){
        ofSetColor(0,0,0);
        ofRect(_x,_y,_sx,_sy);
        ofSetColor(255,255,255);
    } else if(bRunningTimeline){
        //masterTex->loadData(timelines["video-timeline"]->getNextFrame(), 853, 480, GL_RGB);
        //masterTex->draw(_x, _y, _sx, _sy);
        if(frameCounter++ < numFrames){
            //cout << frameCounter << " out of " << numFrames << endl;
            saver.addFrame(timelines["video-timeline"]->getNextFrame());
        } else {
            saver.finishMovie();
            map<string,Timeline*>::iterator tIter;
            /*
            for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
                tIter->second->clear();
            }
            SceneManager::Instance()->reset();
            MediaCabinet::Instance()->reset();
            */
            /*
            bRunningTimeline = false;
            GuiConfigurator::Instance()->closeSheet("render");
        }
    } else if (!isRecording) {
        /*
        map<string,Timeline*>::iterator tIter;
        for(tIter = timelines.begin(); tIter != timelines.end(); ++tIter){
            tIter->second->draw(_x,_y,_sx,_sy);
        }
        */
        /*
        masterTex->loadData(masterBuffer,853, 480, GL_RGB);
        masterTex->draw(_x, _y, _sx, _sy);
    }
}
*/

void Compositor::draw(int _x, int _y, int _sx, int _sy){
    masterTex->draw(_x, _y, _sx, _sy);
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
        saver.setup(853,480,_fileName);
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
