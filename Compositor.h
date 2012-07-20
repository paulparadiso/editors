#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "ofMain.h"
#include "MediaCabinet.h"
#include "Timeline.h"
#include "ofxQtVideoSaver.h"
#include "AudioRenderer.h"
#include "ofxXmlSettings.h"

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

class Compositor : public ofBaseApp
{
    public:
        static Compositor* Instance();
        virtual ~Compositor();
        void addTimeline(string _name);
        void addClipToTimeline(string _timeline, Clip* _clip);
        void addClipToTimeline(string _timeline, Clip* _clip, string _id);
        void removeClipFromTimeline(string _timeline, Clip* _clip);
        void removeClipFromTimeline(string _timeline, string _id);
        void update();
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
        Timeline* getTimeline(string _name){return timelines[_name];}
        void blackOut();
        void render();
        void saveVideo(string _fileName);
        void saveAudio(string _fileName);
        unsigned char * getMasterBuffer(){return masterBuffer;}
        void runTimeline(string _fileName);

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
        unsigned char* black;
        unsigned char* masterBuffer;
        static Compositor* mInstance;
        map<string, Timeline*> timelines;
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
        Timeline* videoTimeline;
        int numFrames;
        int frameCounter;

        /*Record Audio*/
        void audioReceived(float* input, int bufferSize, int nChannels);
        void audioRequested(float * output, int bufferSize, int nChannels);
        float* recordBuffer;
        int maxRecordBuffer;
        int recordPosition;
        int playPosition;
        int recordState;
        ofRtAudioSoundStream stream;
};

#endif // COMPOSITOR_H
