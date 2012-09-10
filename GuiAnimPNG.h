#ifndef GUIANIMPNG_H
#define GUIANIMPNG_H

#include "GuiNode.h"

class GuiAnimPNG : public GuiNode
{
    public:
        GuiAnimPNG(map<string,string> &_attrs);
        virtual ~GuiAnimPNG();
        void update();
        void draw();
        void execute(){}
        bool processMouse(int _x, int _y, int _state){}
    protected:
    private:
        ofImage animations[8];
        int currentAnimation;
        float switchDelay;
        float timeOfLastSwitch;
};

#endif // GUIANIMPNG_H
