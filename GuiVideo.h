#ifndef GUIVIDEO_H
#define GUIVIDEO_H

#include "GuiButton.h"

class GuiVideo : public GuiButton {

    public:
        GuiVideo(map<string,string> &_attrs);
        virtual ~GuiVideo(){}
        void draw();
        void update();
        void update(Subject* _sub);
    private:
        ofVideoPlayer vid;
};

#endif // GUIVIDEO_H