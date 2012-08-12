#pragma once

#include "ofMain.h"
#include "GuiConfigurator.h"
#include "SubObMediator.h"
#include "Subject.h"
#include "SoundEffectsPlayer.h"
#include "Compositor.h"

class testApp : public ofBaseApp, public Subject {

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

        GuiConfigurator *gc;
        ofImage img;
        float timeOfLastInteraction;
        float timeOutCounter;

        void updateMouseState(const char * _state, int _x, int _y, int _button);
        map<string,string> attrs;
        string getAttr(const char* _key){return attrs[_key];}
        SoundEffectsPlayer *sfxPlayer;
};
