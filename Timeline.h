#ifndef TIMELINE_H_INCLUDED
#define TIMELINE_H_INCLUDED

#include "ofMain.h"
#include "Clip.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"
//#include "GuiConfigurator.h"
#include "Utils.h"

/*
The timeline holds a series of ClipContainers in order and receives ticks from
the compositor to advance their frame.
*/

class ClipContainer;

class Timeline : public Observer, public Subject, public ofBaseApp{

public:

    /*
    Constructor.
    */

    Timeline(float _totalTime, string _name, string _mode);

    /*
    The pos is used as a base position for draw the GuiButtons.
    */

    Timeline(float _totalTime, string _name, string _mode, string _pos);

    /*
    Add and remove a clip.
    */

    void addClip(Clip *_clip);
    void removeClip(ClipContainer *_clip);

    /*
    Update the timeline and it's ClipContainers.
    */

    void update();

    /*
    Observer::update
    */

    void update(string _subName, Subject* _sub);

    /*
    Reset the timeline.
    */
    void reset();

    /*
    Whether the timeline is empty.
    */

    bool getIsEmpty();

    /*
    Set the timelines frame.  This will set the position of whichever clip is within that range.
    */

    void setFrame(int _frame);
    void setAudioFrame(int _frame);
    void setVideoFrame(int _frame);

    /*
    Return the current frame of the timeline.
    */

    int getCurrentFrame();

    /*
    Get the total number of frames in the timeline.
    */

    int getNumFrames(){return numFrames;}

    /*
    Return either the total number of frames or samples.
    */

    int getTotalLength;

    /*
    Return the pixels at the head of the timeline.
    */

    unsigned char* getPixels();

    /*
    Blend all frames that are currently overlapping on the timeline.  Whenever there is a wipe or
    crossfade applied to an effect.
    */

    void compositeFrames(unsigned char* _one, unsigned char* _two, int _w, int _h, int _bd, bool _blend);

    /*
    Return whether the timeline is currently compositing.
    */

    bool isCompositing(){return bCompositing;};

    /*
    Return whether a new frame is ready to be sent to the compositor.
    */

    bool hasNewFrame(){return bHaveNewFrame;}

    /*
    Adjust the start frames of all ClipContainers.
    */

    void adjustStartFrames(int _amt);

    /*
    Set the start frames of all ClipContainers.
    */
    void setStartFrames();

    /*
    Subject::getAttr.
    */

    string getAttr(const char* _key){return attrs[_key];}

    /*
    Stream audioRequested.  For audio timelines.
    */

    void audioRequested(float * output, int bufferSize, int nChannels);

    /*
    Start, stop, pause, rewind.
    */

    void start();
    void stop();
    void pause();
    void rewind();

    /*
    Return the timelines mode.  audio or video.
    */

    string getType();

    /*
    Call draw on each of the ClipContainers.
    */

    void draw();

    /*
    Find and remove dead clips.
    */

    void clean();
    bool bHaveDeadClips;
    bool haveClips();

private:

    /*
    The ClipContainer vector.  Maybe should be a list?
    */

    vector<ClipContainer*> clips;

    /*
    Vector of ClipContainers that are active at any given frame.
    */

    vector<ClipContainer*> activeClips;

    /*
    Attrs used by Subject.
    */
    map<string,string> attrs;

    /*
    The current number of frames.
    */

    int numFrames;

    /*
    The maximum number of frames allowable on the timeline.
    */

    int currentMaxFrames;

    /*
    Frame buffer to hold the front most frame.
    */

    unsigned char * frameBuffer;

    /*
    Output used for blending and compositing frames.
    */
    unsigned char * output;

    /*
    The current sample to return from the frontmost clip.
    */

    int samplePosition;

    /*
    The audio streamer.
    */

    ofRtAudioSoundStream *stream;

    /*
    Whether or not there is a new frame.
    */
    bool bHaveNewFrame;

    /*
    Whether the timeline is currently compositing frames.
    */

    bool bCompositing;

    /*
    Used to detect larger just in frame count.
    */

    int previousFrame;

    /*
    The timeline mode, audio or video.
    */

    string mode;

    /*
    The time remaining on the track.  Used to update SceneManager of the tracks
    remaining time.
    */

    float timeRemaining;

    /*
    Whether the timeline is currently playing.
    */

    bool isPlaying;

    /*
    The base position for drawing buttons.
    */

    ofVec2f basePos;

    /*
    The total length of the timeline.
    */

    float totalTime;

    /*
    Recently changed.  Effects opening fix.
    */

    bool bRecentChange;
};

#endif // TIMELINE_H_INCLUDED
