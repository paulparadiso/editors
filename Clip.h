#ifndef CLIP_H_INCLUDED
#define CLIP_H_INCLUDED

#include "ofMain.h"
#include "sndfile.h"

enum{
    VIDEO_EFFECT_NONE,
    VIDEO_EFFECT_ROTATE,
    VIDEO_EFFECT_WIPE,
    VIDEO_EFFECT_FADE,
    VIDEO_EFFECT_CROSS
};

enum{
    AUDIO_EFFECT_NONE,
    AUDIO_EFFECT_FADE_IN_OUT,
    AUDIO_EFFECT_VOLUME_DOWN,
    AUDIO_EFFECT_VOLUME_UP,
    AUDIO_EFFECT_ECHO,
    AUDIO_EFFECT_MUTE
};

class Effect{

public:
    Effect(float _duration){clipDuration = _duration;}
    Effect(float _clipDuration, float _effectLength, float _effectStart, float _multiplier){clipDuration = _clipDuration;
                                                                                            effectLength = _effectLength;
                                                                                            effectStart = _effectStart;
                                                                                            multiplier = _multiplier;
                                                                                            }
    void setClipDuration(float _clipDuration){clipDuration = _clipDuration;}
    void setEffectLength(float _effectsLength){effectLength = _effectsLength;}
    void setEffectStart(float _effectStart){effectStart = _effectStart;}
    void setMultiplier(float _multiplier){multiplier = _multiplier;}
    virtual void process(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pct) = 0;
    virtual ~Effect(){}

protected:
    float clipDuration;
    float effectLength;
    float effectStart;
    float multiplier;
};

class FadeToBlack : public Effect{

public:
    FadeToBlack(float _clipDuration);
    FadeToBlack(float _clipDuration, float _effectLength, float _effectStart, float _multiplier);
    void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);

private:
    void fadeIn(unsigned char* _buf, unsigned char* _oBuf, int _w, int _h, float _pos);
    void fadeOut(unsigned char* _buf, unsigned char* _oBuf, int _w, int _h, float _pos);
};

class Greyscale : public Effect{

public:
    Greyscale(float _clipDuration);
    Greyscale(float _clipDuration, float _effectLength, float _effectStart, float _multiplier);
    void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);

};

class Wipe : public Effect{

public:
    Wipe(float _clipDuration);
    Wipe(float _clipDuration, float _effectLength, float _effectStart, float _multiplier);
    void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);

private:
    void wipeIn(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos);
    void wipeOut(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos);
};

class Rotate : public Effect{

public:
    Rotate(float _clipDuration);
    Rotate(float _clipDuration, float _effectLength, float _effectStart, float _multiplier);
    void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);

private:
    unsigned char* processedBuffer;

};

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
    virtual void setFrame(int _frameNum){}

    /*Transitions*/

    void setWipeOut(bool _wipeOut){bWipeOut = _wipeOut;}
    void setWipeIn(bool _wipeIn){bWipeIn = _wipeIn;}
    void setFadeOut(bool _fadeOut){bFadeOut = _fadeOut;}
    void setFadeIn(bool _fadeIn){bFadeIn = _fadeIn;}

    bool getWipeOut(){return wipeOut;}
    bool getWipeIn(){return wipeIn;}
    bool getFadeIn(){return fadeIn;}
    bool getFadeOut(){return fadeOut;}

    Wipe *wipeOut;
    Wipe *wipeIn;
    FadeToBlack *fadeOut;
    FadeToBlack *fadeIn;

    string getType(){return type;}
    string getName(){return name;}
    int getEffectStatus(){return effectStatus;}
    virtual void setEffectStatus(int _effectStatus){};

    virtual int getDataLength(){}
    virtual float* getData(){}

protected:
    string name;
    string type;
    int effectStatus;
    vector<Effect*> effects;
    float startTime;
    float stopTime;
    float duration;
    bool bWipeIn;
    bool bWipeOut;
    bool bFadeIn;
    bool bFadeOut;
    bool bPlayingPreview;

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
    void setEffectStatus(int _effectStatus){effectStatus = _effectStatus;}
    unsigned char* getNextFrame();
    int getDataLength(){return video.getTotalNumFrames();}
    void setTransitionFrame(unsigned char* _frame);
    void releaseTransitionFrame();
    unsigned char * getLastFrame(){return lastFrame;}

    /*New Frame Controls*/
    int getCurrentFrame();
    int getTotalNumFrames();
    unsigned char* getPixels();
    void setFrame(int _frameNum);

private:
    ofVideoPlayer video;
    ofTexture dispTex;
    unsigned char* lastFrame;
    unsigned char* transitionFrame;
    unsigned char* clipData;
    unsigned char* black;
    int vw, vh;
    FadeToBlack* fader;
    Wipe* wiper;
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

private:
    //ofSoundPlayer *audio;
    //ofPtr<ofFmodSoundPlayer> basePlayer;
    ofRtAudioSoundStream *stream;
    float *sndBuf;
    float *processedBuffer;
    SF_INFO sndInfo;

    /*Audio Effect*/

    void amplifyBuffer(float _amps);
    void fadeBuffer(float _inPct, float _outPct);
    void echoBuffer();

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
};

#endif // CLIP_H_INCLUDED
