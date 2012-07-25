#ifndef GUISCRUBBER_H
#define GUISCRUBBER_H

#include "GuiNode.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"
#include "Compositor.h"

class GuiScrubber : public GuiNode, public Subject, public Observer {

public:
    GuiScrubber(map<string,string> &_attrs);
    bool processMouse(int _x, int _y, int _state);
    bool isDragging(int _x, int _y);
    void executeDrag(int _x, int _y);
    void execute();
    void update();
    void update(string _subName, Subject *_sub);
    void draw();
    string getAttrs(const char* _key){return attrs[_key];}
    float getPercent(){return pct;}

private:
    float homeX, lastX, maxX;
    float pct;
    float lastPct;
    ofImage ball;
    ofImage stick;
    bool bActivated;
};

#endif // GUISCRUBBER_H
