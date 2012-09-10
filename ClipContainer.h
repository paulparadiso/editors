#ifndef CLIPCONTAINER_H_INCLUDED
#define CLIPCONTAINER_H_INCLUDED

#include "ofMain.h"
#include "Clip.h"
#include "GuiTimeline.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"

/*
The ClipContainer allows the timeline to hold a pointer to the same clip at multiple
points along itself.
*/

class ClipContainer : public Observer, public Subject{

public:

    /*
    Constructor
    */
    ClipContainer(Clip* _clip);

    /*
    Return the clips type.
    */

    string getType(){return clip->getType();}

    /*
    Return the contained clip.
    */

    Clip* getClip(){return clip;}

    /*
    Update the clip.
    */

    void update(){clip->update();}

    /*
    Observer::update
    */

    void update(string _subName, Subject* _sub);

    /*
    Return the clip's duration,
    */

    float getDuration(){return clip->getDuration();}

    /*
    Set the position of the timeline button.
    */

    void setPosition(ofVec2f _pos){timelineButton->setPosition(_pos);}

    /*
    Get the size of the timeline button.
    */

    ofVec2f getSize(){return timelineButton->getSize();}

    /*
    Get and set the clip's effect status.
    */
    int getEffectStatus(){return clip->getEffectStatus();}
    void setEffectStatus(int _effectStatus);

    /*
    Get and set the start and stop frames for the clip container.
    */

    int getStartFrame(){return startFrame;}
    void setStartFrame(int _startFrame);
    int getStopFrame(){return stopFrame;}

    /*
    Return the clips current frame number.
    */

    int getCurrentFrame(){return clip->getCurrentFrame();}

    /*
    Return the total number of frames in the clip.
    */

    int getTotalNumFrames(){return clip->getTotalNumFrames();}
    int getTotalNumSamples(){return clip->getTotalNumSamples();}

    /*
    Return a pointer to a video clip's pixels.
    */

    unsigned char* getPixels();

    /*
    Return a pointer to an audio clip's samples.
    */

    float *getSamples(){return clip->getSamples();}

    /*
    Get the sample at the given position.
    */

    float getSample(int _samplePosition);

    /*
    Set the clip containers current frame.
    */

    void setFrame(int _frameNum){clip->setFrame(_frameNum);}

    /*
    Push the start frame of the clip container forwards or backwards.
    */

    void adjustStartFrame(int _amt);

    /*
    Functions for determining whether a clip needs to be moved or have another
    effect applied based on other clip containers.
    */

    bool hasCrossfade(){return bHasCrossfade;}
    void setTransitionOut(bool _fade){clip->setFadeOut(_fade);}
    bool hasTransition(){return bHasTransition;}
    bool needsNudge(){return bNeedsNudge;}
    void setFadeOut(bool _fade){clip->setFadeOut(_fade);}

    /*
    Call draw() on the GuiTimelineButton.
    */

    void draw();

    /*
    Set and check if bDead to true.
    */

    void kill(){bDead = true;}
    bool getIsDead(){return bDead;}

    /*
    Check button
    */

    bool isInside(int _x, int _y){return timelineButton->isInside(_x,_y);}
    void execute(){timelineButton->execute();}

    /*
    Reset and adjust button.  Adjust subtract 0.5.
    */

    void resetButton();
    void adjustButton();

    /*
    Move Clip starttimes base on effect status.
    */

    bool bNeedsNudge;
    bool bHasTransition;
    bool bHasCrossfade;

    bool hasNewEffect();

private:

    /*
    The contained clip.
    */

    Clip* clip;

    /*
    The current effect status of the clip container.
    */

    int effectStatus;

    /*
    The start and stop frame of the clip container.
    */

    int startFrame;
    int stopFrame;

    /*
    Whether a clip container currently has a transition effect.
    */

    bool bHaveTransition;
    bool bCrossfade;

    /*
    Set by GuiButton delete.  Used by Timeline to identify and remove dead clips.
    */

    bool bDead;

    /*
    The GuiTimelineButton.
    */

    GuiTimelineButton *timelineButton;

    /*
    The clip duration/button length
    */

    float buttonLength;

    bool bHaveNewEffect;

};


#endif // CLIPCONTAINER_H_INCLUDED
