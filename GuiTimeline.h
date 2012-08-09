#ifndef GUITIMELINE_H_INCLUDED
#define GUITIMELINE_H_INCLUDED

#include "GuiNode.h"
#include "Compositor.h"
#include "GuiButton.h"
#include "GuiConfigurator.h"
#include "SubObMediator.h"
#include "Observer.h"
#include "Subject.h"

class GuiTimelineButton : public GuiButton
{
    public:
        GuiTimelineButton(float _length);
        virtual ~GuiTimelineButton(){}
        void draw();
        void drawLoosie();
        void setTotalLength(float _totalWidth);
        void makeDropShadow();
        void makeButton();
        void makeInnerCurve();
        void makeOuterCurve();
        void update();
        void update(string _subName, Subject* _sub);
        void setAwaitingResponse(bool _b){bAwaitingResponse = _b;}
        bool getIsAwaitingResponse(){return bAwaitingResponse;}
        void setId(string _id){id = _id;}

    protected:
    private:
        float width, height, totalWidth;
        float length;
        float widthSegment, heightSegment;
        float curveRadius, curveBuffer;
        ofPath buttonPath;
        ofPath dropShadow;
        ofPath innerCurve;
        ofPath outerCurve;

        string id;

        map<string,ofColor*> colors;
        string activeEffect;

        ofImage img;
        bool bLoosie;
        bool bAwaitingResponse;
};

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
    /*This is bad.
    Inherits two versions of getAttr.
    */

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

#endif // GUITIMELINE_H_INCLUDED
