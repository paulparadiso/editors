#ifndef GUICREATOR_H_INCLUDED
#define GUICREATOR_H_INCLUDED

#include "ofMain.h"
#include "Transition.h"
#include "Clip.h"
#include "MediaCabinet.h"
#include "Timeline.h"
#include "GuiNode.h"
#include "GuiButton.h"
#include "GuiImage.h"
#include "GuiSheet.h"
#include "GuiTimeline.h"

/*

Abstract GuiNode class.  Menus, buttons, timelines, previews,
images and decorations will all inherit from this class.

*/

/*
class GuiSheet{

public:
    GuiSheet();
    void addNode(GuiNode* _node);
    void pushToStack();
    void popFromStack();
    void update();
    void activate();
    void draw();
    void touch();
    void setName(string _name){name = _name;}
    void setType(string _type){type = _type;}
    void setAllAttr(string _attr, string _val);
    void reset();
    string getName(){return name;}
    bool checkNodes(int _x, int _y);
    bool checkNodesForDrag(int _x, int _y);
    GuiNode* getActivatedNode(){return activatedNode;}
    bool deliverMessage(map<string, string> _msg);
    void setLoosie(){haveLoosie = true;}

private:
    vector<GuiNode*> nodes;
    string name;
    string type;
    string effectTarget;
    GuiNode* activatedNode;
    bool haveLoosie;

};
*/

class GuiCreator{

public:

    GuiCreator();
    //GuiNode* makeNode(map<string,string> &_attrs);

private:
    vector<string> nodes;


};

/*

class GuiButton: public GuiNode{

public:
    GuiButton(map<string,string> &_attrs);
    GuiButton(string _img);
    GuiButton() : GuiNode(){}
    //void initialize(map<string, string> &_attrs);
    void execute();
    virtual void draw();
    void setMessage(map<string, string> _msg){msg = _msg;}
    void sendMessage();
    void click();
    void unclick();
    bool getClicked(){return drawClicked;}

protected:
    ofImage bg;
    ofImage clicked;
    bool haveImage;
    map<string, string> msg;
    bool drawClicked;
    bool haveClicked;
};
*/

/*
class TimelineButton : public GuiButton
{
    public:
        TimelineButton(float _length);
        virtual ~TimelineButton(){}
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
*/

/*

class GuiMovie : public GuiButton{

    public:
        GuiMovie(map<string,string> &_attrs);
        virtual ~GuiMovie(){}
        void draw();
        void update();

    private:
        ofVideoPlayer mov;
};

*/

/*

class Image : public GuiNode{

public:
    Image(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();

private:
    ofImage img;

};
*/

class MediaPreview : public GuiNode{

public:
    MediaPreview(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void setupVideo();
    void setupAudio();
    void execute();
    void update();
    void draw();
    void message(map<string,string> _msg);
    bool isInside(int _x, int _y);
    void setSelectTarget(string _target);
    void setPosition(ofVec2f _pos);
    void setSelectMessage(map<string,string> _msg){select->setMessage(_msg);}

private:
    string media;
    ofImage frame;
    ofImage ball;
    Clip* preview;
    ofVec2f exitOffset;
    ofVec2f selectOffset;
    ofVec2f playOffset;
    GuiButton *exit;
    GuiButton *select;
    GuiButton *play;

};

/*

class TimelineGui : public GuiNode{

public:
    TimelineGui(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();
    void message(map<string,string> _msg);
    bool isInside(int _x, int _y);
    void addItem(map<string,string> _msg);
    void deleteItem(map<string,string> _msg);
    float getTimeRemaining(){return maxTime - currentTime;};

private:
    list<TimelineButton*> images;
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

class Viewport : public GuiNode{

public:
    Viewport(map<string,string> &_attrs);
    void execute(){}
    void update();
    void draw();
    void message(map<string,string> _msg);
};

/*
class Scrubber : public GuiNode{

public:
    Scrubber(map<string,string> &_attrs);
    bool isDragging(int _x, int _y);
    void executeDrag(int _x, int _y);
    void execute();
    void update();
    void draw();
    float getPercent(){return pct;}

private:
    float homeX, lastX, maxX;
    float pct;
    float lastPct;
    ofImage ball;
    ofImage stick;
};
*/
#endif // GUICREATOR_H_INCLUDED
