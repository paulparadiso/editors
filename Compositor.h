#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "ofMain.h"
#include "MediaCabinet.h"
#include "Timeline.h"
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

class Timeline;
class AudioRenderer;

class Compositor : public ofBaseApp, public Subject, public Observer
{
    public:
        Compositor();
        virtual ~Compositor();

        /*
        Add a timeline.
        */

        void addTimeline(string _name, string _mode, string _pos);

        /*
        Add a clip to a timeline;
        */

        void addClipToTimeline(string _timeline, Clip* _clip);

        /*
        Remove a clip from a timeline.
        */

        void removeClipFromTimeline(string _timeline, Clip* _clip);

        /*
        Update.
        */

        void update();

        /*
        Reset.
        */

        void reset();

        /*
        Observer::update();
        */

        void update(string _subName, Subject* _sub);

        /*
        Positioning functions.
        */

        void start();
        void stop();
        void pause();
        void rewind();

        /*
        Set the scrubber position.
        */

        void setScrubberPosition(float _pct);

        /*
        Draw the video timeline if there is one.
        */

        void draw(int _x, int _y, int _sx, int _sy);
        void draw();

        /*
        Set the masterTex to black.
        */

        void blackOut();

        /*
        Render and save the output of the compositor.
        */

        void render();
        void setRender();

        /*
        Save the video file.
        */

        void saveVideo(string _fileName);

        /*
        Save the sudio file.
        */

        void saveAudio(string _fileName);

        /*
        Set attributes used by Subject::getAttr.
        */

        void setAttr(string _attr, string _val){attrs[_attr] = _val;}

        /*
        Subject::getAttr function.
        */

        string getAttr(const char* _key){return attrs[_key];}

        /*
        Update the current framecount to the timeline with the largest
        framecount.
        */

        void updateFrameCount();

        /*
        Record Audio
        */

        void startRecording();
        void stopRecording();
        void saveRecording();
        void playRecording();

        /*
        Used for position record and playback progress bars.
        */

        float getRecordingPosition();
        float getPlayingPosition();
        int getRecordStatus();

        /*
        Return if playing.
        */

        bool getIsPlaying(){return isPlaying;}

        bool haveVideoClips();

    protected:

    private:

        /*
        Make the timestamp string used to save files.
        */

        string makeDateName();

        /*
        The attrs map.
        */

        map<string, string>attrs;

        /*
        Black buffer used in blackOut()
        */

        unsigned char* black;

        /*
        Map containing audio timelines.
        */

        map<string, Timeline*> audioTimelines;

        /*
        The video timeline and whether it exists.
        */

        Timeline* videoTimeline;
        bool bHaveVideoTimeline;
        string videoTimelineName;

        /*
        The scrubber position.
        */

        float scrubberPosition;

        /*
        The master texture set to the head of the video timeline.
        */

        ofTexture* masterTex;

        /*
        AudioRenderer used to render multiple timline down to one and write
        to file.
        */

        AudioRenderer audioRenderer;

        /*
        The current number of frames in the compositor and the maximum number of frames accepted.
        */

        int numFrames;
        int maxNumFrames;

        /*
        The audio stream used to record audio.
        */

        ofRtAudioSoundStream stream;

        /*
        The audioReceived and audioRequested functions are used to record and playback
        */

        void audioReceived(float* input, int bufferSize, int nChannels);
        void audioRequested(float * output, int bufferSize, int nChannels);

        /*
        The record buffer and associated record and playback positioning variable.
        */

        float* recordBuffer;
        int maxRecordBuffer;
        int recordPosition;
        int playPosition;

        /*
        The current record state of the compositor.
        */

        int recordState;

        /*
        The timer used to set the frame count and it's framerate.
        */

        Timer *timer;
        int framerate;

        /*
        The current frame used to update the timelines.
        */

        int currentFrame;

        /*
        Used by update() to determine whether pause has been set by another function.
        */

        bool bSetPause;

        /*
        Whether the timeline is currently compositiing.
        */

        bool bCompositing;

        /*
        Delay render start until sheet is open;
        */

        bool bDoRender;
        int renderFrames;

        /*
        If playing.
        */

        bool isPlaying;
};

#endif // COMPOSITOR_H
