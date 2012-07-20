#ifndef GUIIMAGE_H
#define GUIIMAGE_H

#include "GuiNode.h"

class GuiImage : public GuiNode{

public:
    GuiImage(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();

private:
    ofImage img;

};

#endif // GUIIMAGE_H
