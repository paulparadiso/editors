#ifndef GUITIMELINE_H_INCLUDED
#define GUITIMELINE_H_INCLUDED

#include "GuiButton.h"
#include "SubObMediator.h"
#include "Observer.h"
#include "Subject.h"

/*
The timeline button draws itself rather than containing
an image.  Appears the same in arabic.
*/

class ClipContainer;

class GuiTimelineButton : public GuiButton
{
    public:

        /*
        Constructor takes length of clip and it's type (audio/video)
        */

        GuiTimelineButton(float _length, string _type, ClipContainer *_partnerClip);
        virtual ~GuiTimelineButton(){}

        /*
        Reset the length and rebuild the button.
        */

        void setLength(float _length);

        /*
        Redefine execute
        */

        void execute();

        /*
        Draw the button.
        */

        void draw();

        /*
        Draw the loosie version when the effects menu is open.
        */

        void drawLoosie();

        /*
        Set the totalLength of the button.
        */

        void setTotalLength(float _totalWidth);

        /*
        Make the drop shadow.
        */

        void makeDropShadow();

        /*
        Make the Button.
        */

        void makeButton();

        /*
        Make the inner curve.
        */

        void makeInnerCurve();

        /*
        Make the outer curve.
        */

        void makeOuterCurve();

        /*
        Update.
        */

        void update();

        /*
        Observer::update function.
        */

        void update(string _subName, Subject* _sub);

    protected:
    private:

        /*
        Size parameter.
        */

        float width;
        float height;
        float totalWidth;
        float length;
        float widthSegment, heightSegment;
        float curveRadius, curveBuffer;

        /*
        Paths for drawing components.
        */

        ofPath buttonPath;
        ofPath dropShadow;
        ofPath innerCurve;
        ofPath outerCurve;

        /*
        Collection of ofColors used for drawing border according to effect.
        */

        map<string,ofColor*> colors;

        /*
        The currently active effect.
        */

        string activeEffect;

        /*
        The image drawn on top of the button for audio or video.
        */

        ofImage img;

        /*
        Whether awaiting response from effects menu.
        */

        bool bAwaitingResponse;

        /*
        Whether is loosie.
        */

        bool bLoosie;

        /*
        The type of clip.
        */
        string type;

        /*
        The clip to which the button is connected.
        */

        ClipContainer *partnerClip;
};

/*

class GuiTimeline : public GuiNode, public Observer, public Subject {

public:
    GuiTimeline(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();
    void message(map<string,string> _msg);
    bool isInside(int _x, int _y);
    bool processMouse(int _x, int _y, int _state);
    void update(string _subName, Subject* _sub);
    void addItem(map<string,string> _msg);
    void addItem(string _mode, string _id, float _duration);
    void deleteItem(map<string,string> _msg);
    void findAndDeleteItem();
    float getTimeRemaining(){return maxTime - currentTime;}
    string getAttr(const char* _key){string ret = attrs[_key]; cout << "gui returning " << ret << endl; return ret;}

private:
    list<GuiTimelineButton*> images;
    //map<GuiButton*,Clip*>
    Timeline* timeline;
    string mode;
    void addVideo(float _length, string _path);
    void addAudio(float _length, string _path);
    int clipCounter;
    int currentTime;
    int maxTime;
    int clipIDCounter;
};
*/

#endif // GUITIMELINE_H_INCLUDED
