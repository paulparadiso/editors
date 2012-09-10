#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"
#include "GuiNumberRenderer.h"
#include "GuiSheet.h"
#include "GuiConfigurator.h"

class SceneManager : public Subject, public Observer {

public:

    /*
    Return the singleton pointer.
    */

    static SceneManager* Instance();

    /*
    Push a sheet onto the top of the draw stack.
    */

    void pushSheet(GuiSheet* _sheet);

    /*
    Pop the top sheet off of the draw stack.
    */

    void popSheet();

    /*
    Return a pointer to the top of the draw stack.
    */

    GuiSheet* getTopSheet();

    /*
    Add masks for displaying behind popups.  This shoud be a single
    function.
    */

    void addMask(GuiSheet* _mask);
    void addInfoMask(GuiSheet* _infoMask);

    /*
    Add a timeline to the compositor.
    */

    void addTimeline(string _name, string _mode, string _pos);

    /*
    Return the active timeline.
    */

    string getActiveTimeline(){return activeTimeline;}

    /*
    Update the SceneManager and it's member objects.
    */

    void update();

    /*
    Draw the SceneManager.
    */

    void draw();

    /*
    Reset the SceneManager to first sheet.
    */

    void setReset();
    void reset();

    /*
    Observer update function.
    */

    void update(string _subName, Subject *_sub);

    /*
    Subject getAttr.
    */

    string getAttr(const char* _key){return attrs[_key];}

    /*
    Return the time remaining on the active timeline.  activeTimeline
    is set by the Observer::update().
    */

    int getTimeRemainingOnTrack();

    /*
    Individual draw functions used by other objects to draw
    various elements
    */

    void drawTimeRemaining();
    void drawNumber(int _num, int _x, int _y, int _sx, int _sy, int _type = 0){numberRenderer->drawNumber(_num,_x,_y,_sx,_sy,_type);}
    void drawFrame(int _x, int _y);
    void drawPreviewFrame(int _x, int _y);
    void drawAudioPreviewFrame(int _x, int _y);
    void drawCompositor(int _x, int _y, int _sx, int _sy);
    void drawNoCustom();

    /*
    Call the compositor's render.
    */

    void render(){compositor->setRender();}

    /*
    Whether or not the system is set to arabic.
    */

    bool getDisplayArabic(){return displayArabic;}

    /*
    Get the compositor's recording position.
    */

    float getRecordingPosition(){return compositor->getRecordingPosition();}

    /*
    Check if the compositor is currently playing.
    */

    bool getIsPlaying(){return compositor->getIsPlaying();}
    bool haveActiveVideos(){return compositor->haveVideoClips();}

private:

    /*
    Private constructor.
    */

    SceneManager();

    /*
    The singleton pointer.
    */

    static SceneManager* mInstance;

    /*
    The draw stack.  GuiSheets are pushed and draw in order
    of bottom to top with a mask being inserted befor the
    top sheet.  Only the top sheet receives mouse data.
    */

    vector<GuiSheet*> drawStack;

    /*
    The main mask and the lighter info mask.
    */

	GuiSheet *mask;
	GuiSheet *infoMask;

	bool haveMask;
	map<string,string> attrs;

	/*
	Images used to draw the "time remain on track" in english
	and arabic.
	*/

	ofImage timeRemaining;
	ofImage timeRemainingArabic;

	/*
	Whether or not to display arabic versions of text and number images.
	*/

	bool displayArabic;

	/*
	The positions at which to draw the time remaining.
	*/

	ofVec2f timeRemainingPos;
	ofVec2f timeRemainingPosArabic;

	/*
	activeTimeline is set by the Observer::update function and is used to
	retrieve values from the timeRemainingOnTrack map also set by Observer::update
	*/

	string activeTimeline;
	map<string,int>timeRemainingOnTrack;

    /*
    Rendering objects.
    */

    ofImage frame;
    ofImage frameArabic;
    ofImage previewFrame;
    ofImage audioPreviewFrame;
    ofImage noCustom;
    ofImage noCustomArabic;
    GuiNumberRenderer *numberRenderer;
    Compositor *compositor;

    bool bReset;
};

#endif // SCENEMANAGER_H_INCLUDED
