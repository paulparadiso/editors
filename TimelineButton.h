#ifndef TIMELINEBUTTON_H
#define TIMELINEBUTTON_H

#include "ofMain.h"
#include "GuiCreator.h"

class TimelineButton : public GuiButton
{
    public:
        TimelineButton(float _length);
        virtual ~TimelineButton();
        void draw();
        void setTotalLength(float _totalWidth);
        void makeDropShadow();
        void makeButton();
        void makeInnerCurve();
        void makeOuterCurve();
        void update();

    protected:
    private:
        float width, height, totalWidth;
        float length;
        float widthSegment, heightSegment;
        float curveRadius, curveBuffer;
        ofPath buttonPath;
        ofPath dropShadow;
        ofPath innerCurve;
        ofPath outerCurve;

        ofImage img;
};

#endif // TIMELINEBUTTON_H
