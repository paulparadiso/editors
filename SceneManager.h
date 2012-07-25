#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include "GuiCreator.h"
#include "Subject.h"
#include "Observer.h"
#include "SubObMediator.h"

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
};

#endif // SCENEMANAGER_H_INCLUDED
