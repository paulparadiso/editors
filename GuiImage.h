#ifndef GUIIMAGE_H
#define GUIIMAGE_H

#include "GuiNode.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"

class GuiImage : public GuiNode, public Observer{

public:
    GuiImage(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();
    bool processMouse(int _x, int _y, int _state){return false;}
    void update(string _subName, Subject *_sub);

private:
    ofImage img;
    ofImage imgArabic;
    bool haveArabic;
    bool displayArabic;
};

#endif // GUIIMAGE_H
