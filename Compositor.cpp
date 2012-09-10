#include "Compositor.h"
#include "GuiConfigurator.h"

Compositor::Compositor(){
    masterTex = new ofTexture;
    masterTex->allocate(854,480,GL_RGB);
    black = new unsigned char[854 * 480 * 3];
    memset(black,0,854 * 480 * 3);
    blackOut();
    scrubberPosition = 0.0;
    maxRecordBuffer = 44100 * 30;
    playPosition = 0;
    recordPosition = 0;
    recordState = RECORD_STATE_STOPPED;
    recordBuffer = new float[maxRecordBuffer];
    stream.setup(this,1,1,44100,256,4);
    stream.stop();
    framerate = 24;
    maxNumFrames = 24 * 59;
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
    bDoRender = false;
    isPlaying = false;
}

bool Compositor::haveVideoClips(){
    if(bHaveVideoTimeline){
        if(videoTimeline->haveClips()){
            return true;
        } else {
            return false;
        }
    }
    return false;
}

Compositor::~Compositor(){
}

void Compositor::addTimeline(string _name, string _mode, string _pos){
    if(_mode == "video"){
        videoTimeline = new Timeline(60.0,_name,_mode,_pos);
        videoTimelineName = _name;
        bHaveVideoTimeline = true;
    } else if (_mode == "audio"){
        map<string, Timeline*>::iterator tIter;
        tIter = audioTimelines.find(_name);
        if(tIter != audioTimelines.end()){
            audioTimelines.erase(_name);
        }
        audioTimelines[_name] = new Timeline(60.0, _name,_mode,_pos);
    }
}

void Compositor::update(string _subName, Subject* _sub){
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
        /*
        if(_sub->getAttr("name") == "render-button"){
            cout << "rendering" << endl;
            render();
        }
        */
        if(_sub->getAttr("name") == "trash"){
            blackOut();
            rewind();
        }
    }
    if(_subName == "timer-tick"){
        //cout << "Timer Tick" << endl;
        if(numFrames > 0){
            //currentFrame = (currentFrame + 1) % numFrames;
            if(bHaveVideoTimeline){
                if(!videoTimeline->isCompositing()){
                    videoTimeline->setFrame(currentFrame);
                }
            }
            map<string,Timeline*>::iterator tIter;
            for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
                tIter->second->setFrame(currentFrame);
            }
            currentFrame++;
            if(currentFrame > numFrames){
                currentFrame = numFrames;
                //currentFrame = 0;
                stop();
            }
        }
        float timePos = (float)currentFrame / (float)maxNumFrames;
        setAttr("time-pos",ofToString(timePos));
        SubObMediator::Instance()->update("timeline-position-changed",this);
    }
    if(_subName == "sheet-changed"){
        stop();
    }
    if(_subName == "scrubber-position-changed"){
        float inPct = ofToFloat(_sub->getAttr("time-pos"));
        //currentFrame = numFrames * inPct;
        int jumpFrame = maxNumFrames * inPct;
        if(jumpFrame < numFrames){
            currentFrame = jumpFrame;
        }
        if((jumpFrame < numFrames) && bHaveVideoTimeline){
            currentFrame = jumpFrame;
            //if(currentFrame < numFrames && numFrames > 0){
                videoTimeline->setFrame(currentFrame);
                float timePos = (float)currentFrame / (float)maxNumFrames;
                setAttr("time-pos",ofToString(timePos));
                SubObMediator::Instance()->update("timeline-position-changed",this);
            //} else {
                /*
                //blackOut();
                float timePos = (float)currentFrame / (float)maxNumFrames;
                setAttr("time-pos",ofToString(timePos));
                SubObMediator::Instance()->update("timeline-position-changed",this);
                */
            //}
        } else {
            float timePos = (float)currentFrame / (float)maxNumFrames;
            setAttr("time-pos",ofToString(timePos));
            SubObMediator::Instance()->update("timeline-position-changed",this);
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

void Compositor::blackOut(){
    masterTex->loadData(black, 854, 480, GL_RGB);
    //memset(masterBuffer,0,854 * 480 * 3);
}

void Compositor::addClipToTimeline(string _timeline, Clip* _clip){
    cout << "Compositor adding clip " << _clip->getName() << " to timeline " << _timeline << endl;
    if(bHaveVideoTimeline){
        if(_timeline == videoTimelineName){
            videoTimeline->addClip(_clip);
        } else {
            audioTimelines[_timeline]->addClip(_clip);
        }
    } else {
        map<string,Timeline*>::iterator mIter;
        mIter = audioTimelines.find(_timeline);
        if(mIter != audioTimelines.end()){
            audioTimelines[_timeline]->addClip(_clip);
        }
    }
    cout << "Compositor added clip to timeline." << endl;
    updateFrameCount();
    cout << "Compositor updatedFrameCount." << endl;
}

void Compositor::removeClipFromTimeline(string _timeline, Clip* _clip){
}

void Compositor::setScrubberPosition(float _pct){
}

void Compositor::update(){
    //numFrames = videoTimeline->getNumFrames();
    updateFrameCount();
    if(bHaveVideoTimeline){
        if(videoTimeline->hasNewFrame() && !bSetPause){
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
    if(bDoRender){
        pause();
        render();
    }
}

void Compositor::reset(){
    if(bHaveVideoTimeline){
        videoTimeline->reset();
    }
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->reset();
    }
    rewind();
    blackOut();
}

void Compositor::start(){
    if(currentFrame >= numFrames){
        currentFrame = 0;
    }
    timer->start();
    isPlaying = true;
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->start();
    }
}

void Compositor::pause(){
    timer->stop();
    isPlaying = false;
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->pause();
    }
}

void Compositor::stop(){
    timer->stop();
    isPlaying = false;
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
    //blackOut();
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
    if(bHaveVideoTimeline){
        videoTimeline->draw();
    }
    map<string, Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->draw();
    }
}

void Compositor::draw(){
}

void Compositor::setRender(){
    bDoRender = true;
    renderFrames = 200;
}

void Compositor::render(){
    /*
    GuiConfigurator::Instance()->openSheet("render");
    string timeBase = ofGetTimestampString();
    string wavFile = "render/audio/" + timeBase + ".wav";
    string movFile = "render/video/" + timeBase + ".mov";
    saveAudio(wavFile);
    runTimeline(movFile);
    */
    //cout << "compositor starting rendering" << endl;

    if(renderFrames > 0){
        //ofSleepMillis(2);
        renderFrames--;
        return;
    }
    GuiConfigurator::Instance()->replaceSheet("save-complete");
    if(bHaveVideoTimeline){
        videoTimeline->reset();
    }
    map<string,Timeline*>::iterator tIter;
    for(tIter = audioTimelines.begin(); tIter != audioTimelines.end(); ++tIter){
        tIter->second->reset();
    }
    //SubObMediator::Instance()->update("reset",this);
    //SceneManager::Instance()->reset();
    currentFrame = 0;
    bDoRender = false;
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
