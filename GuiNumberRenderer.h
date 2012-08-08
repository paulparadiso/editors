#ifndef GUINUMBERRENDERER_H
#define GUINUMBERRENDERER_H

#include "ofMain.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"

class GuiNumberRenderer : public Observer
{
    public:
        GuiNumberRenderer();
        GuiNumberRenderer(string _path);
        virtual ~GuiNumberRenderer();
        void drawNumber(int _num, int _x, int _y, int _sx, int _sy, int _type);
        void update(string _subName, Subject *_sub);
    protected:
    private:
        map<int,ofImage> timeRemainingEnglish;
        map<int,ofImage> timeRemainingArabic;
        map<int,ofImage> clipTimeArabic;
        ofDirectory lister;
        bool displayArabic;
};

#endif // GUINUMBERRENDERER_H
