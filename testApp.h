#pragma once

#include "ofMain.h"
#include "GuiConfigurator.h"
#include "SubObMediator.h"
#include "Subject.h"
#include "Observer.h"
#include "SoundEffectsPlayer.h"
#include "Compositor.h"

class testApp : public ofBaseApp, public Subject, public Observer {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void update(string _subName, Subject* _sub);

        GuiConfigurator *gc;
        ofImage img;

        /*
        The last time any interaction occured.
        */

        float timeOfLastInteraction;

        /*
        The last time an input was sent (mouse down).
        */

        float timeOfLastInput;
        float timeOutCounter;
        bool checkTimeOut;

        float inputDelayTime;

        void updateMouseState(const char * _state, int _x, int _y, int _button);
        map<string,string> attrs;
        string getAttr(const char* _key){return attrs[_key];}
        SoundEffectsPlayer *sfxPlayer;
};
