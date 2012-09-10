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
        virtual void execute();
        virtual void activate();
        virtual void update();
        virtual void update(string _subName, Subject* _sub);
        void setMessage(map<string,string> _msg){}
        void draw();
        string getAttr(const char* _key){return attrs[_key];}
		void setChannel(string _channel){channel = _channel;}
		string getChannel(){return channel;}
	protected:
    private:
        ofImage inactive;
        ofImage active;
        ofImage arabic;
		string channel;
		bool drawActive;
        bool haveActive;
        bool haveImage;
        bool haveArabic;
};

#endif // GUIBUTTON_H
