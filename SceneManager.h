#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include "GuiCreator.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"
#include "GuiNumberRenderer.h"

class SceneManager : public Subject, public Observer {

public:
    static SceneManager* Instance();
    void pushSheet(GuiSheet* _sheet);
    void popSheet();
    void addMask(GuiSheet* _mask);
    void addInfoMask(GuiSheet* _infoMask);
	void addInteractiveNode(GuiNode* _node);
    void touchSheet(string _target);
    void update();
    void update(string _subName, Subject *_sub);
    void draw();
    GuiSheet* getTopSheet();
    void reset();
    string getAttr(const char* _key){return attrs[_key];}
    void drawTimeRemaining(string _track);
    void drawNumber(int _num, int _x, int _y, int _sx, int _sy, int _type = 0){numberRenderer->drawNumber(_num,_x,_y,_sx,_sy,_type);}

private:
    SceneManager();
    static SceneManager* mInstance;
    vector<GuiSheet*> drawStack;
	vector<GuiNode*> interactiveNodes;
	GuiSheet *mask;
	GuiSheet *infoMask;
	bool haveMask;
	bool bFirstStart;
	map<string,string> attrs;
	GuiNumberRenderer *numberRenderer;
	ofImage timeRemaining;
	ofImage timeRemainingArabic;
	bool displayArabic;
	ofVec2f timeRemainingPos;
	ofVec2f timeRemainingPosArabic;
	map<string,int>timeRemainingOnTrack;
};

#endif // SCENEMANAGER_H_INCLUDED
