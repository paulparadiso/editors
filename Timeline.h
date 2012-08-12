#ifndef TIMELINE_H_INCLUDED
#define TIMELINE_H_INCLUDED

#include "ofMain.h"
#include "Clip.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"
#include "Timer.h"
#include "Effects.h"

class ClipContainer : public Observer, public Subject{

public:
    ClipContainer(Clip* _clip);
    Clip* getClip(){return clip;}
    void play(){clip->play();}
    void play(float _runningTime){clip->play(_runningTime);}
    void stop(){clip->stop();}
    void pause(){clip->pause();}
    void reset(){clip->reset();}
    void update(){clip->update();}
    void unpause(){clip->unpause();}
    void draw(int _x, int _y, int _sx, int _sy){clip->draw(_x,_y,_sx,_sy);}
    string getType(){return clip->getType();}
    float getStartTime(){return clip->getStartTime();}
    float getStopTime(){return clip->getStopTime();}
    float getTimeRemaining(){return clip->getTimeRemaining();}
    float getPosition(){return clip->getPosition();}
    const char* getID(){cout << "returning " << clipID << endl; return clipID.c_str();}
    void setID(string _id){clipID = _id;}
    bool isDone(){return clip->isDone();}
    unsigned char* getNextFrame(){return clip->getNextFrame();}
    void setTransitionFrame(unsigned char * _frame){clip->setTransitionFrame(_frame);}
    void releaseTransitionFrame(){clip->releaseTransitionFrame();}
    unsigned char * getLastFrame(){return clip->getLastFrame();}

    void update(string _subName, Subject* _sub);

    float getDuration(){return clip->getDuration();}

    int getDataLength(){clip->getDataLength();}
    float* getData(){return clip->getData();}

    void setPosition(float _pos){clip->setPosition(_pos);}
    int getEffectStatus(){return clip->getEffectStatus();}
    void setEffectStatus(int _effectStatus){effectStatus = _effectStatus;}

    /*New Frame Controls*/
    int getCurrentFrame(){return clip->getCurrentFrame();}
    int getTotalNumFrames(){return clip->getTotalNumFrames();}
    int getTotalNumSamples(){return clip->getTotalNumSamples();}
    unsigned char* getPixels();
    float *getSamples(){return clip->getSamples();}
    float getSample(int _index){return clip->getSample(_index);}
    void setFrame(int _frameNum){clip->setFrame(_frameNum);}

    int getStartFrame(){return startFrame;}
    int getStopFrame(){return stopFrame;}
    void setStartFrame(int _startFrame);
    void adjustStartFrame(int _amt);

    bool hasCrossfade(){return clip->hasCrossfade();}
    void setTransitionOut(bool _fade){clip->setFadeOut(_fade);}
    bool hasTransition(){return clip->hasTransition();}
    bool needsNudge(){return clip->needsNudge();}
    void setFadeOut(bool _fade){clip->setFadeOut(_fade);}

private:
    /*Effects*/
    float duration;
    int effectStatus;
    Clip* clip;
    int startFrame;
    int stopFrame;
    unsigned char * buffer;
    string clipID;
    bool bHaveTransition;
    bool bCrossfade;

};

class Timeline : public Observer, public Subject, public ofBaseApp{

public:

    Timeline(float _totalTime, string _name, string _mode);
    void addClip(Clip *_clip);
    void addClip(Clip *_clip, string _id);
    void removeClip(Clip *_clip);
    void removeClip(string _id);
    void start();
    void stop();
    void pause();
    void unpause();
    void restart();
    void rewind();
    void update();
    void update(float);
    void update(string _subName, Subject* _sub);
    void updateRun();
    void reset();
    void draw(int _x, int _y, int _sx, int _sy);
    void setScrubTime(float _pct);
    int getEffectStatus(string _name);
    void setEffectStatus(string _name, int _effectStatus);
    string getType();
    bool getIsEmpty();
    int getTotalLength();
    void assembleData();
    float* getData(){return allData;}
    float* allData;
    float getTimeRemaining(){return timeRemaining;};
    unsigned char* getNextFrame();
    void setupRun();
    void clear();

    /*New frame functions*/
    int getCurrentFrame();
    void setFrame(int _frame);
    void setVideoFrame(int _frame);
    void setAudioFrame(int _frame);
    unsigned char* getPixels();
    int getNumFrames(){return numFrames;}
    void compositeFrames(unsigned char* _one, unsigned char* _two, int _w, int _h, int _bd, bool _blend);
    bool hasNewFrame(){return bHaveNewFrame;}
    void adjustStartFrames(int _amt);
    void setStartFrames();

    string getAttr(const char* _key){return attrs[_key];}
    void adjustForTransition(bool _on);
    bool isCompositing(){return bCompositing;}

    void audioRequested(float * output, int bufferSize, int nChannels);
    void updateAudio();

private:
    vector<ClipContainer*> clips;
    vector<ClipContainer*> activeClips;
    map<float,Clip*> mappedClips;
    ClipContainer *currentClip;
    int clipIndex;
    float startTime, runningTime, totalTime;
    bool isPlaying;
    bool isTransitioning;
    int totalLength;
    int timeRemaining;
    map<string,string> attrs;

    int numFrames;
    int currentMaxFrames;
    unsigned char * frameBuffer;
    unsigned char * output;
    float* sndBuffer;
    int samplePosition;
    ofRtAudioSoundStream *stream;
    bool bHaveNewFrame;
    bool bCompositing;
    int previousFrame;

    string mode;
};

#endif // TIMELINE_H_INCLUDED
