#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "GuiNode.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"
#include "ofMain.h"

class GuiNode;

class GuiButton : public GuiNode, public Subject, public Observer
{
    public:
        GuiButton(map<string,string> &_attrs);
        GuiButton(string _img);
        GuiButton(){}
        virtual ~GuiButton(){}
        bool processMouse(int _x, int _y, int _state);
        void execute();
        virtual void update(Subject* _sub);
        void setMessage(map<string,string> _msg){}
        void draw();
        string getAttr(const char* _key){return attrs[_key];}
    protected:
    private:
        ofImage inactive;
        ofImage active;
        bool drawActive;
        bool haveActive;
        bool haveImage;
};

#endif // GUIBUTTON_H
