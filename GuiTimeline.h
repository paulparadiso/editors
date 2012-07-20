#ifndef GUITIMELINE_H_INCLUDED
#define GUITIMELINE_H_INCLUDED

#include "GuiNode.h"
#include "Compositor.h"
#include "GuiButton.h"
#include "GuiConfigurator.h"

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

        ofImage img;
        bool bLoosie;
};

class GuiTimeline : public GuiNode{

public:
    GuiTimeline(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();
    void message(map<string,string> _msg);
    bool isInside(int _x, int _y);
    void addItem(map<string,string> _msg);
    void deleteItem(map<string,string> _msg);
    float getTimeRemaining(){return maxTime - currentTime;};

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
