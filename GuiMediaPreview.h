#ifndef GUIMEDIAPREVIEW_H_INCLUDED
#define GUIMEDIAPREVIEW_H_INCLUDED

#include "GuiNode.h"
#include "GuiButton.h"
#include "Subject.h"
#include "Clip.h"
#include "MediaCabinet.h"

class GuiMediaPreview : public GuiNode{

public:
    GuiMediaPreview(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void setupVideo();
    void setupAudio();
    bool processMouse(int _x, int _y, int _state);
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

#endif

