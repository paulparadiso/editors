#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "ofMain.h"
#include "MediaCabinet.h"
#include "Timeline.h"
#include "ofxQtVideoSaver.h"
#include "AudioRenderer.h"
#include "ofxXmlSettings.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"
#include "Timer.h"

enum{
    PLAY_STATE_STOPPED,
    PLAY_STATE_PLAYING,
    PLAY_STATE_PAUSED
};

enum{
    RECORD_STATE_STOPPED,
    RECORD_STATE_RECORDING,
    RECORD_STATE_PLAYING
};

class Compositor : public ofBaseApp, public Subject, public Observer
{
    public:
        static Compositor* Instance();
        virtual ~Compositor();
        void addTimeline(string _name, string _mode);
        void addClipToTimeline(string _timeline, Clip* _clip);
        void addClipToTimeline(string _timeline, Clip* _clip, string _id);
        void removeClipFromTimeline(string _timeline, Clip* _clip);
        void removeClipFromTimeline(string _timeline, string _id);
        void update();
        void update(string _subName, Subject* _sub);
        void start();
        void stop();
        void pause();
        void rewind();
        void setScrubberPosition(float _pct);
        float getScrubberPosition(){return scrubberPosition;}
        int getEffectStatus(string _name);
        void setEffectStatus(string _name, int _effectStatus);
        ofTexture* getMasterTex(){return masterTex;}
        void draw(int _x, int _y, int _sx, int _sy);
        void drawTex(int _x, int _y, int _sx, int _sy);
        //Timeline* getTimeline(string _name){return timelines[_name];}
        void blackOut();
        void render();
        void saveVideo(string _fileName);
        void saveAudio(string _fileName);
        unsigned char * getMasterBuffer(){return masterBuffer;}
        void runTimeline(string _fileName);
        void setAttr(string _attr, string _val){attrs[_attr] = _val;}
        string getAttr(const char* _key){return attrs[_key];}

        void updateFrameCount();

        /*Record Audio*/
        void startRecording();
        void stopRecording();
        void saveRecording();
        void playRecording();
        float getRecordingPosition();
        float getPlayingPosition();
        int getRecordStatus();

    protected:

    private:
        Compositor();
        string makeDateName();
        map<string, string>attrs;
        unsigned char* black;
        unsigned char* masterBuffer;
        static Compositor* mInstance;
        map<string, Timeline*> audioTimelines;
        Timeline* videoTimeline;
        string videoTimelineName;

        float scrubberPosition;
        float startTime;
        bool isRunning;
        int playState;
        float timeOfLastUpdate;
        float runningTime;
        ofTexture* masterTex;
        ofxQtVideoSaver saver;
        AudioRenderer audioRenderer;
        float saverFrameRate;
        bool isRecording;
        bool bRunningTimeline;
        float recordingStartTime;
        float timeOfLastFrame;
        int numFrames;
        int maxNumFrames;
        int frameCounter;
        int clipCounter;
        string activeTimeline;

        /*Record Audio*/
        void audioReceived(float* input, int bufferSize, int nChannels);
        void audioRequested(float * output, int bufferSize, int nChannels);
        float* recordBuffer;
        int maxRecordBuffer;
        int recordPosition;
        int playPosition;
        int recordState;
        ofRtAudioSoundStream stream;

        int framerate;
        int frameNum;
        int currentFrame;
        Timer *timer;
        bool bSetPause;

        bool bHaveVideoTimeline;
};

#endif // COMPOSITOR_H
