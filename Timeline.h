#ifndef TIMELINE_H_INCLUDED
#define TIMELINE_H_INCLUDED

#include "ofMain.h"
#include "Clip.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"
#include "Timer.h"

class ClipContainer{

public:
    ClipContainer(Clip* _clip){clip = _clip;}
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

    int getDataLength(){clip->getDataLength();}
    float* getData(){return clip->getData();}

    void setPosition(float _pos){clip->setPosition(_pos);}
    string clipID;
    int getEffectStatus(){return clip->getEffectStatus();}
    void setEffectStatus(int _effectStatus){clip->setEffectStatus(_effectStatus);}

    /*New Frame Controls*/
    int getCurrentFrame(){return clip->getCurrentFrame();}
    int getTotalNumFrames(){return clip->getTotalNumFrames();}
    unsigned char* getPixels(){return clip->getPixels();}
    void setFrame(int _frameNum){clip->setFrame(_frameNum);}

    int getStartFrame(){return startFrame;}
    int getStopFrame(){return stopFrame;}
    void setStartFrame(int _startFrame);

private:
    Clip* clip;
    int startFrame;
    int stopFrame;
};

class Timeline : public Observer, public Subject{

public:

    Timeline(float _totalTime, string _name);
    void addClip(Clip *_clip);
    void addClip(Clip *_clip, string _id);
    void removeClip(Clip *_clip);
    void removeClip(string _id);
    void start();
    void stop();
    void pause();
    void unpause();
    void restart();
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
    unsigned char* getPixels();
    int getNumFrames(){return numFrames;}
    void compositeFrames(unsigned char* _one, unsigned char* _two, int _w, int _h, int _bd);
    bool hasNewFrame(){return bHaveNewFrame;}

    string getAttr(const char* _key){return attrs[_key];}

private:
    vector<ClipContainer*> clips;
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
    unsigned char * frameBuffer;
    bool bHaveNewFrame;
};

#endif // TIMELINE_H_INCLUDED
