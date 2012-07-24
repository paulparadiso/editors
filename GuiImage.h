#ifndef GUIIMAGE_H
#define GUIIMAGE_H

#include "GuiNode.h"

class GuiImage : public GuiNode{

public:
    GuiImage(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();
    bool processMouse(int _x, int _y, int _state){return false;}

private:
    ofImage img;

};

#endif // GUIIMAGE_H
