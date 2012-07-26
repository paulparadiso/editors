#ifndef GUIVIEWPORT_H
#define GUIVIEWPORT_H

#include "GuiNode.h"
#include "Compositor.h"

class GuiViewport : public GuiNode{

public:
    GuiViewport(map<string,string> &_attrs);
    bool processMouse(int _x, int _y, int _state){return false;}
    void execute(){}
    void update();
    void draw();
    void message(map<string,string> _msg);
};

#endif // GUIVIEWPORT_H
