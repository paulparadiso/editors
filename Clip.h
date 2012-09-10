#ifndef CLIP_H_INCLUDED
#define CLIP_H_INCLUDED

#include "ofMain.h"
#include "sndfile.h"
//#include "WavHandler.h"
#include "Effects.h"
#include "ofxFFmpegVideoPlayer.h"

class Clip{

public:
    Clip();
    virtual ~Clip(){cout << "deleting clip." << endl;}
    virtual void draw(int _x, int _y, int _sx, int _sy) = 0;
    virtual void drawPreview(int _x, int _y, int _sx, int _sy) = 0;
    virtual void update(float _time) = 0;
    virtual void update(){}
    virtual void play() = 0;
    virtual void play(float _startTime) = 0;
    virtual void pause() = 0;
    virtual void unpause() = 0;
    virtual void stop() = 0;
    virtual void reset() = 0;
    virtual void setStartTime(float _startTIme) = 0;
    virtual void startPreview() = 0;
    virtual void stopPreview() = 0;
    virtual bool isDone() = 0;
    virtual void setPosition(float _pct) = 0;
    virtual float getPosition() = 0;
    virtual float getTimeRemaining(){}
    float getStartTime();
    float getStopTime();
    float getDuration();
    void setupTransitions();
    virtual unsigned char* getNextFrame(){}
    virtual void setTransitionFrame(unsigned char *_frame){}
    virtual void releaseTransitionFrame(){}
    virtual unsigned char* getLastFrame(){}

    /*New Frame Controls*/
    virtual int getCurrentFrame(){return -1;}
    virtual int getTotalNumFrames(){return -1;}
    virtual unsigned char* getPixels(){return NULL;}
    virtual float* getSamples(){return NULL;}
    virtual void setFrame(int _frameNum){}
    virtual float getSample(int _index){return -1;}
    virtual int getTotalNumSamples(){return -1;}

    /*Transitions*/

    void setTransitionFade(bool _b){bFadeOut = _b;}
    bool getTransitionFade(){return bFadeOut;}

    string getName(){return name;}
    string getType(){return type;}
    int getEffectStatus(){return effectStatus;}
    virtual void setEffectStatus(int _effectStatus){cout << "Base setEffectStatus" << endl;}

    virtual int getDataLength(){}
    virtual float* getData(){}

    virtual void setFadeOut(bool _fade){bFadeOut = _fade;}
    bool needsNudge(){return bNeedsNudge;}
    bool hasTransition(){return bHasTransition;}
    bool hasCrossfade(){return bHasCrossfade;}

protected:
    string name;
    string type;
    int effectStatus;
    vector<Effect*> effects;
    Effect *fadeOut;
    bool bFadeOut;
    float startTime;
    float stopTime;
    float duration;
    bool bPlayingPreview;
    float framerateAdjust;
    bool bNeedsNudge;
    bool bHasTransition;
    bool bHasCrossfade;
};

class VideoClip : public Clip{

public:
    VideoClip(string _path);
    ~VideoClip();
    void draw(int _x, int _y, int _sx, int _sy);
    void drawPreview(int _x, int _y, int _sx, int _sy);
    void update();
    void update(float _time);
    void play();
    void play(float _startTime);
    void pause();
    void unpause();
    void stop();
    void reset();
    bool isDone();
    void startPreview();
    void stopPreview();
    void setStartTime(float _startTime);
    void setPosition(float _pct);
    void setDuration(float _dur){duration = _dur;}
    float getPosition(){}
    float getTimeRemaining();
    void setEffectStatus(int _effectStatus);
    unsigned char* getNextFrame();
    int getDataLength(){return video.getTotalNumFrames();}
    void setTransitionFrame(unsigned char* _frame);
    void releaseTransitionFrame();
    unsigned char * getLastFrame(){return lastFrame;}

    /*New Frame Controls*/
    int getCurrentFrame();
    int getTotalNumFrames();
    unsigned char* getPixels();
    unsigned char* getImagePixels();
    unsigned char* getVideoPixels();
    void setFrame(int _frameNum);

    /*Image */
    void setupVideo(string _file);
    void setupImage(string _file);

private:
    ofxFFmpegVideoPlayer video;
    ofImage img;
    int currentFrame;
    ofTexture dispTex;
    unsigned char* lastFrame;
    unsigned char* transitionFrame;
    unsigned char* clipData;
    unsigned char* black;
    int vw, vh;
    FadeToBlack* fader;
    Wipe* wiper;
    string mode;
};

class ImageClip : public Clip{

public:
    ImageClip(string _path);
    ~ImageClip(){}
    void draw(int _x, int _y, int _sx, int _sy);
    void drawPreview(int _x, int _y, int _sx, int _sy);
    void update();
    void update(float _time){}
    void play();
    void play(float _startTime){}
    void pause(){}
    void unpause(){}
    void stop();
    void reset(){}
    bool isDone(){}
    void startPreview(){}
    void stopPreview(){}
    void setStartTime(float _startTime){}
    void setPosition(float _pct){}
    void setDuration(float _dur){duration = _dur;}
    Effect *fader;
    Effect *wiper;

private:
    ofImage image;
    int vw, vh;
};

class AudioClip : public Clip, public ofBaseApp{

public:
    AudioClip(string _path, float _dur);
    ~AudioClip();
    void draw(int _x, int _y, int _sx, int _sy);
    void drawPreview(int _x, int _y, int _sx, int _sy);
    void update(float _time);
    void update(){}
    void play();
    void play(float _startTime);
    void pause();
    void unpause();
    void stop();
    void reset();
    bool isDone();
    void startPreview();
    void stopPreview();
    void setStartTime(float _startTime);
    void setPosition(float _pct){}
    float getPosition();
    void audioRequested(float * output, int bufferSize, int nChannels);
    int getDataLength();
    float* getData();
    float *allData;
    void setEffectStatus(int _effectStatus);
    void setEffect();
    float* getSamples();
    float getSample(int _index);
    void setFadeOut(bool _fade);

    /*New Frame Controls*/
    int getCurrentFrame();
    int getTotalNumFrames();
    int getTotalNumSamples();
    unsigned char* getPixels();
    void setFrame(int _frameNum);

private:
    //ofSoundPlayer *audio;
    //ofPtr<ofFmodSoundPlayer> basePlayer;
    ofRtAudioSoundStream stream;
    float *sndBuf;
    float *processedBuffer;
    float *fadedBuffer;
    SF_INFO sndInfo;
    //WavHandler wavHandler;
    /*Audio Effect*/

    void amplifyBuffer(float* _inBuf, float* _outBuf, float _amps);
    void fadeBuffer(float* _inBuf, float* _outBuf, float _inPct, float _outPct);
    void echoBuffer(float* _inBuf, float* _outBuf);

    int echoPhase;
    float echoDamp;

    int samplePosition;
    int pausePosition;
    int numSamples;
    int numProcessedSamples;
    float playVolume;
    float lowVolume;
    float midVolume;
    float highVolume;
    float muteVolume;
    float pauseVolume;
    bool bPlaying;
    bool bIsDone;

    float *frameSamples;
};

#endif // CLIP_H_INCLUDED
