#include "clip.h"
#include "Compositor.h"

Clip::Clip(){
    bPlayingPreview = false;
}

float Clip::getStartTime(){
    return startTime;
}

float Clip::getStopTime(){
    return stopTime;
}

float Clip::getDuration(){
    return duration;
}

void Clip::setupTransitions(){
    float transitionTime = 0.5 / duration;
    bWipeIn = false;
    bWipeOut = false;
    bFadeIn = false;
    bFadeOut = false;
    wipeIn = new Wipe(duration,transitionTime,0.0,0.0);
    wipeOut = new Wipe(duration,transitionTime,1.0 - transitionTime, 1.0);
    fadeIn = new FadeToBlack(duration, transitionTime, 1.0,1.0);
}

VideoClip::VideoClip(string _file) : Clip(){
    video.loadMovie(_file);
    video.setVolume(0.0);
    video.stop();
    video.setPosition(0.0);
    name = _file;
    vw = video.getWidth();
    vh = video.getHeight();
    dispTex.allocate(vw,vh,GL_RGBA);
    cout << "new video " << vw << ", " << vh << " with tex = " << dispTex.getWidth() << ", " << dispTex.getHeight() << endl;
    duration = video.getDuration();
    type = "video";
    effectStatus = 0;
    effects.push_back(new Rotate(duration));
    effects.push_back(new Wipe(duration, 0.5 / duration, 0.0,0.0));
    effects.push_back(new FadeToBlack(duration,0.5 / duration, 0.0,1.0));
    effects.push_back(new FadeToBlack(duration,0.5 / duration, 0.0,0.0));
    //lastFrame = new unsigned char[vw * vh * 3];
    //video.play();
    //video.setPosition(0.99);
    //video.update();
    //memcpy(lastFrame, video.getPixels(), vw * vh * 3);
    video.stop();
    setupTransitions();
    black = new unsigned char[853 * 480 * 3];
    transitionFrame = black;
}

VideoClip::~VideoClip(){
    cout << "video clip dying." << endl;
}

/*New Frame Controls*/

int VideoClip::getCurrentFrame(){
    return video.getCurrentFrame();
}

int VideoClip::getTotalNumFrames(){
    return video.getTotalNumFrames();
}

unsigned char* VideoClip::getPixels(){
    return video.getPixels();
}

void VideoClip::setFrame(int _frameNum){
    cout << "video setting frame to - " << _frameNum << endl;
    video.setFrame(_frameNum);
    cout << "video currently at frame - " << video.getCurrentFrame() << endl;
}

void VideoClip::setTransitionFrame(unsigned char * _frame){
    transitionFrame = _frame;
}

void VideoClip::releaseTransitionFrame(){
    transitionFrame = black;
}

void VideoClip::play(){
    startTime = ofGetElapsedTimef();
    stopTime = startTime + duration;
    video.play();
    cout << "playing video from " << startTime << " to " << stopTime << endl;
}

void VideoClip::play(float _startTime){
    startTime = _startTime;
    stopTime = startTime + duration;
    video.play();
    cout << "playing video from " << startTime << " to " << stopTime << endl;
}

void VideoClip::pause(){
    video.setPaused(true);
}

void VideoClip::unpause(){
    video.setPaused(false);
}

void VideoClip::stop(){
    video.stop();
}

void VideoClip::reset(){
    video.setPosition(0.0);
    video.stop();
}

bool VideoClip::isDone(){
    if(video.getTotalNumFrames() - video.getCurrentFrame() < 1){
        return true;
    }
    return false;
}

float VideoClip::getTimeRemaining(){
    return video.getDuration() - video.getPosition() * video.getDuration();
}

void VideoClip::startPreview(){
    bPlayingPreview = true;
    play();
    video.setLoopState(OF_LOOP_NORMAL);
}

void VideoClip::stopPreview(){
    bPlayingPreview = false;
    video.setLoopState(OF_LOOP_NONE);
    stop();
    video.setPosition(0.0);
}

unsigned char* VideoClip::getNextFrame(){
    //cout << "return next frame - " << video.getCurrentFrame() << endl;
    video.play();
    video.update();
    clipData = video.getPixels();
    //memset(clipData,ofRandom(0,255),853 * 480 * 3);
    video.nextFrame();
    video.stop();
    return clipData;
}

void VideoClip::update(){
    //cout << "video updating." << endl;
    video.update();
    /*
    if(video.isFrameNew()){
        clipData = video.getPixels();
        if(effectStatus > 0){
            effects[effectStatus - 1]->process(clipData, transitionFrame, vw, vh, video.getPosition());
        }
        if(bWipeIn){
            wipeIn->process(clipData, transitionFrame, vw, vh, video.getPosition());
        }
        if(bWipeOut){
            wipeOut->process(clipData, transitionFrame, vw, vh, video.getPosition());
        }
        if(bFadeIn){
            fadeIn->process(clipData, transitionFrame, vw, vh, video.getPosition());
        }
        if(bPlayingPreview){
            dispTex.loadData(clipData,vw,vh,GL_RGB);
        }
        //Compositor::Instance()->getMasterTex()->loadData(clipData,vw,vh,GL_RGB);
        memcpy(Compositor::Instance()->getMasterBuffer(),clipData, vw * vh * 3);
    }
    */
}

void VideoClip::update(float _time){
    if(_time > startTime && _time < stopTime){
        if(!video.isPlaying()){
            cout << name << " is starting.\n" << "--->input time = " << _time << ".\n" << "--->startTime = " << startTime << "." << endl;
            play();
        }
    } else {
        if(video.isPlaying()){
            cout << name << " is stopping.\n" << "--->input time = " << _time << ".\n" << "--->stopTime = " << stopTime << "." << endl;
            stop();
            video.setPosition(0.0);
        }
    }
    video.update();
    if(video.isFrameNew()){
        clipData = video.getPixels();
        if(effectStatus > 0){
            effects[effectStatus - 1]->process(clipData, transitionFrame, vw, vh, video.getPosition());
        }
        //dispTex.loadData(clipData,vw,vh,GL_RGB);
        dispTex.loadData(lastFrame,vw,vh,GL_RGB);
    }
}

void VideoClip::draw(int _x, int _y, int _sx, int _sy){
    //if(video.isPlaying()){
    //dispTex.draw(_x,_y,_sx,_sy);
    //}
    dispTex.draw(_x,_y,_sx,_sy);
}

void VideoClip::drawPreview(int _x, int _y, int _sx, int _sy){
    //if(video.isPlaying()){
    //dispTex.draw(_x,_y,_sx,_sy);
    //}
    dispTex.draw(_x,_y,_sx,_sy);
}

void VideoClip::setStartTime(float _startTime){
    startTime = _startTime;
    stopTime = startTime + video.getDuration();
}

void VideoClip::setPosition(float _pct){
    video.play();
    video.setPosition(_pct);
    update();
    video.stop();
}

ImageClip::ImageClip(string _file) : Clip(){
    image.loadImage(_file);
    type = "image";
    vw = image.getWidth();
    vh = image.getHeight();
    duration = 5.0;
}

void ImageClip::play(){
    startTime = ofGetElapsedTimef();
    stopTime = startTime + duration;
}

void ImageClip::stop(){
}

void ImageClip::draw(int _x, int _y, int _sx, int _sy){
    image.draw(_x, _y, _sx, _sy);
}

void ImageClip::drawPreview(int _x, int _y, int _sx, int _sy){
    image.draw(_x, _y, _sx, _sy);
}

void ImageClip::update(){
}

AudioClip::AudioClip(string _file, float _dur) : Clip(){
    cout << "loading - " << _file << endl;
    sndInfo.format = 0;
    SNDFILE *sndFile = sf_open(ofToDataPath(_file).c_str(),SFM_READ,&sndInfo);
    if(sndFile == NULL){
        cout << "file load failed." << endl;
    }
    numSamples = sndInfo.channels * sndInfo.frames;
    sndBuf = new float[numSamples];
    int numRead = sf_read_float(sndFile, sndBuf, numSamples);
    samplePosition = 0;
    echoPhase = 44100/4;
    echoDamp = 0.85;
    numProcessedSamples = numSamples;
    //processedBuffer = new float[numProcessedSamples];
    processedBuffer = new float[numProcessedSamples];
    //amplifyBuffer(1.25);
    memcpy(processedBuffer, sndBuf, numSamples * sizeof(float));
    //echoBuffer();
    name = _file;
    duration = _dur;
    type = "audio";
    stream = new ofRtAudioSoundStream;
    stream->setup(this,sndInfo.channels,0,sndInfo.samplerate,256, 32);
    cout << "opened file " << _file << " has " << numSamples << " samples." << endl;
    effectStatus = 0;
}

void AudioClip::setEffectStatus(int _effectStatus){
    //cout << "audio clip setting effect." << endl;
    effectStatus = _effectStatus;
    switch(effectStatus){
        case AUDIO_EFFECT_NONE:
            delete [] processedBuffer;
            processedBuffer = new float[numSamples];
            numProcessedSamples = numSamples;
            memcpy(processedBuffer,sndBuf,numSamples * sizeof(float));
            break;
        case AUDIO_EFFECT_FADE_IN_OUT:
            delete [] processedBuffer;
            processedBuffer = new float[numSamples];
            fadeBuffer(0.25,0.25);
            numProcessedSamples = numSamples;
            break;
        case AUDIO_EFFECT_VOLUME_DOWN:
            delete [] processedBuffer;
            processedBuffer = new float[numSamples];
            numProcessedSamples = numSamples;
            amplifyBuffer(0.5);
            break;
        case AUDIO_EFFECT_VOLUME_UP:
            delete [] processedBuffer;
            processedBuffer = new float[numSamples];
            numProcessedSamples = numSamples;
            amplifyBuffer(1.25);
            break;
        case AUDIO_EFFECT_ECHO:
            delete [] processedBuffer;
            numProcessedSamples = numSamples + echoPhase;
            processedBuffer = new float[numProcessedSamples];
            echoBuffer();
            break;
        case AUDIO_EFFECT_MUTE:
            delete [] processedBuffer;
            numProcessedSamples = numSamples;
            processedBuffer = new float[numProcessedSamples];
            amplifyBuffer(0.0);
            break;
        default:
            break;
    }
}

int AudioClip::getDataLength(){
    //cout << "numSamples = " << numSamples << endl;
    return numProcessedSamples;
}

float* AudioClip::getData(){
    //cout << "audio clip returning buffer" << endl;
    /*
    SF_INFO wInfo;
    wInfo.samplerate = 44100;
    wInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_32;
    wInfo.channels = 1;
    SNDFILE *wFile = sf_open(ofToDataPath("samp.wav").c_str(), SFM_RDWR, &wInfo);
    sf_write_sync(wFile);
    int wrote = sf_write_float(wFile, sndBuf, numSamples);
    //cout << "built data of length " << totalLength << endl;
    cout << "wrote" << endl;
    */
    return processedBuffer;
}

void AudioClip::audioRequested(float * output, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; i++){
        //cout << "setting sample " << sndBuf[samplePosition] << endl;;
        if(bPlaying){
            output[i] = processedBuffer[samplePosition];
            if(samplePosition < numProcessedSamples){
                samplePosition += sndInfo.channels;
            } else {
                samplePosition = 0;
                bPlaying = false;
                bIsDone = true;
            }
        } else {
            output[i] = 0.0;
        }
    }
}

void AudioClip::fadeBuffer(float _inPct, float _outPct){
    int fadeTo = numProcessedSamples * _inPct;
    int fadeFrom = numProcessedSamples - (numSamples * _outPct);
    for(int i = 0; i < numProcessedSamples; i++){
        if(i < fadeTo){
            processedBuffer[i] = sndBuf[i] * i / fadeTo;
        } else if(i < fadeFrom){
            processedBuffer[i] = sndBuf[i];
        } else {
            processedBuffer[i] = sndBuf[i] * 1.0 - ((i - fadeFrom) / fadeFrom);
        }
    }
}

void AudioClip::amplifyBuffer(float _amps){
    for(int i = 0; i < numProcessedSamples; i++){
        processedBuffer[i] = sndBuf[i] * _amps;
    }
}

void AudioClip::echoBuffer(){
    cout << "ECHO" << endl;
    int i, j;
    for(i = 0; i < echoPhase; i++){
        processedBuffer[i] = sndBuf[i];
    }
    for(j = 0; i < numSamples; i++, j++){
        processedBuffer[i] = sndBuf[i] + sndBuf[j] * echoDamp;
    }
    for(;j < numSamples; i++, j++){
        processedBuffer[i] = sndBuf[j] *echoDamp;
    }
}

AudioClip::~AudioClip(){
    cout << "audio clip dying" << endl;
    //delete audio;
}

void AudioClip::play(){
    /*
    cout << "starting audio clip." << endl;
    startTime = ofGetElapsedTimef();
    stopTime = startTime + duration;
    samplePosition = 0;
    //audio->play();
    */
}

void AudioClip::play(float _startTime){
    /*
    startTime = _startTime;
    stopTime = startTime + duration;
    //audio->play();
    cout << "playing video from " << startTime << " to " << stopTime << endl;
    */
    cout << "starting clip. " << endl;
    samplePosition = 0;
    bPlaying = true;
    bIsDone = false;
}

void AudioClip::pause(){
    //audio->setPaused(true);
    /*
    pausePosition = samplePosition;
    pauseVolume = playVolume;
    playVolume = muteVolume;
    */
    bPlaying = false;
}

void AudioClip::unpause(){
    /*
    samplePosition = pausePosition;
    playVolume = pauseVolume;
    */
    //audio->setPaused(false);
    bPlaying = true;
}

void AudioClip::stop(){
    //audio->stop();
    //playVolume = muteVolume;
    cout << "stopping clip." << endl;
    bPlaying = false;
    bIsDone = true;
    samplePosition = 0;
}

void AudioClip::reset(){
    /*
    //audio->setPosition(0.0);
    //audio->stop();
    stop();
    */
    stop();
}

void AudioClip::startPreview(){
    //audio->setPosition(0.0);
    //play();
    samplePosition = 0;
    bPlaying = true;
}

void AudioClip::stopPreview(){
    //audio->setPosition(0.0);
    //stop();
    //playVolume = muteVolume;
    bPlaying = false;
}

bool AudioClip::isDone(){
    /*
    if(audio->getPosition() > 0.99){
        return true;
    } else {
        return false;
    }
    if(samplePosition >= numProcessedSamples - 5000){
        cout << "clip done" << endl;
        return true;
    } else {
        return false;
    }
    */
    return bIsDone;
}

void AudioClip::update(float _time){
    /*
    if(_time > startTime && _time < stopTime){
        if(!audio->getIsPlaying()){
            cout << name << " is starting.\n" << "--->input time = " << _time << ".\n" << "--->startTime = " << startTime << "." << endl;
            play();
        }
    } else {
        if(audio->getIsPlaying()){
            cout << name << " is stopping.\n" << "--->input time = " << _time << ".\n" << "--->stopTime = " << stopTime << "." << endl;
            stop();
            audio->setPosition(0.0);
        }
    }
    */
}

float AudioClip::getPosition(){
    return (float)samplePosition / (float)numProcessedSamples;
}

void AudioClip::draw(int _x, int _y, int _sx, int _sy){
}

void AudioClip::drawPreview(int _x, int _y, int _sx, int _sy){
}

void AudioClip::setStartTime(float _startTime){
    startTime = _startTime;
    stopTime = startTime + duration;
}

FadeToBlack::FadeToBlack(float _duration) : Effect(_duration){
}

FadeToBlack::FadeToBlack(float _clipDuration, float _effectLength, float _effectStart, float _multiplier) : Effect(_clipDuration,_effectLength,_effectStart,_multiplier){
}

void FadeToBlack::process(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos){
    if(multiplier > 0.0){
        fadeOut(_buf,_oBuf,_w,_h,_pos);
    } else {
        fadeIn(_buf,_oBuf,_w,_h,_pos);
    }
}

void FadeToBlack::fadeOut(unsigned char* _buf, unsigned char* _oBuf, int _w, int _h, float _pos){
    if(_pos > 0.75){
        float pct = (_pos - 0.75) / 0.25;
        for(int y = 0; y < _h; y += 1){
            for(int x = 0; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _buf[index] = (_buf[index] * (1.0 - pct));
                _buf[index + 1] = (_buf[index + 1] * (1.0 - pct));
                _buf[index + 2] = (_buf[index + 2] * (1.0 - pct));
            }
        }
    }
}

void FadeToBlack::fadeIn(unsigned char* _buf, unsigned char* _oBuf, int _w, int _h, float _pos) {
    if(_pos < 0.1){
        float pct = _pos / 0.25;
        for(int y = 0; y < _h; y += 1){
            for(int x = 0; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _buf[index] = _buf[index] * pct + _oBuf[index] * (1.0 - pct);
                _buf[index + 1] = _buf[index + 1] * pct + _oBuf[index + 1] * (1.0 - pct);
                _buf[index + 2] = _buf[index + 2] * pct + _oBuf[index + 2] * (1.0 - pct);
            }
        }
    }
}

Greyscale::Greyscale(float _duration) : Effect(_duration){
}

void Greyscale::process(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos){
    float pct = (_pos - 0.75) / 0.25;
    for(int y = 0; y < _h; y += 1){
        for(int x = 0; x < _w * 3; x += 3){
            int index = y * (_w * 3) + x;
            int average = (_buf[index] + _buf[index + 1] + _buf[index + 2]) / 3;
            _buf[index] = average;
            _buf[index + 1] = average;
            _buf[index + 2] = average;
        }
    }
}

Wipe::Wipe(float _duration) : Effect(_duration){
}

Wipe::Wipe(float _clipDuration, float _effectLength, float _effectStart, float _multiplier) : Effect(_clipDuration,_effectLength,_effectStart,_multiplier){}

void Wipe::process(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos){
    /*
    set maxX to width * pct
    set all pixels up to that value to black
    */
    if(multiplier > 0.0){
        wipeOut(_buf,_oBuf,_w,_h,_pos);
    } else {
        wipeIn(_buf,_oBuf,_w,_h,_pos);
    }
}

void Wipe::wipeOut(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos){
    if(_pos > effectStart){
        float pct = (_pos - effectStart) / effectLength;
        int maxX = _w * pct;
        for(int y = 0; y < _h; y += 1){
            for(int x = 0; x < maxX * 3; x += 3){
                int index = y * (_w * 3) + x;
                _buf[index] = _oBuf[index];
                _buf[index + 1] = _oBuf[index + 1];
                _buf[index + 2] = _oBuf[index + 2];
            }
        }
    }
}

void Wipe::wipeIn(unsigned char* _buf, unsigned char *_oBuf, int _w, int _h, float _pos){
    if(_pos < effectLength){
        float pct = _pos / effectLength;
        int minX = _w * 3 * pct;
        for(int y = 0; y < _h; y += 1){
            for(int x = minX; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _buf[index] = _oBuf[index];
                _buf[index + 1] = _oBuf[index + 1];
                _buf[index + 2] = _oBuf[index + 2];
            }
        }
    }
}

Rotate::Rotate(float _duration) : Effect(_duration){
    processedBuffer = new unsigned char[960 * 540 * 3];
}

void Rotate::process(unsigned char* _buf, unsigned char *_oBuf, int _w, int _h, float _pos){
    memset(processedBuffer,0,_w * _h * 3);
    for(int y = 0; y < _h ; y++){
        for(int x = 0; x < _w * 3 ; x+=3){
            int oldIndex = y * _w * 3 + x;
            //int oldIndex2 = y * _w * 3 + (x + 1);
            //int oldIndex3 = y * _w * 3 + (x + 2);
            //ex get(10,12) --> 10 * (_w * 3) + 12  =====  get(12,10) --> 12 * (_w * 3) + 10
            //int newIndex = (y * (_w * 3)) * cos(90.0) - x * sin(90.0) + x * cos(90.0) + (y * (_w * 3)) * sin(90.0);
            //int newX = x * cos(-PI/2) + y * sin(-PI/2);
            //int newY = y * cos(-PI/2) + x *sin(-PI/2);
            int newIndex = x * _w + (y + 190) * 3;
            //int newIndex2 = (x + 2) * _w + y * 3;
            //int newIndex3 = (x + 3) * _w + y * 3;
            //cout << "newIndex = " << newIndex << ". Max = "  << _w * _h * 3 << endl;
            if(newIndex > 0 && newIndex < _w * _h * 3){
                processedBuffer[newIndex] = _buf[oldIndex];
                processedBuffer[newIndex + 1] = _buf[oldIndex + 1];
                processedBuffer[newIndex + 2] = _buf[oldIndex + 2];
            }
            //if(newIndex2 > 0 && newIndex2 < _w * _h * 3){
            //    processedBuffer[newIndex2] = _buf[oldIndex2];
                //processedBuffer[newIndex + 1] = _buf[oldIndex + 1];
                //processedBuffer[newIndex + 1] = _buf[oldIndex + 2];
            //}
            //if(newIndex3 > 0 && newIndex3 < _w * _h * 3){
            //    processedBuffer[newIndex3] = _buf[oldIndex3];
                //processedBuffer[newIndex + 1] = _buf[oldIndex + 1];
                //processedBuffer[newIndex + 1] = _buf[oldIndex + 2];
            //}

        }
    }
    memcpy(_buf,processedBuffer,853 * 480 * 3);
}
