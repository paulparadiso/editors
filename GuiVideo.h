#ifndef GUIVIDEO_H
#define GUIVIDEO_H

#include "GuiButton.h"

class GuiVideo : public GuiButton {

    public:
        GuiVideo(map<string,string> &_attrs);
        virtual ~GuiVideo(){}
        void draw();
        void update();
        void activate();
        void update(string _subName, Subject* _sub);
    private:
        ofVideoPlayer *vid;
        ofVideoPlayer *vidArabic;
        bool haveArabic;
        bool displayArabic;
};

#endif // GUIVIDEO_H
