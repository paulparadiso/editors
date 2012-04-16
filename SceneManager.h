#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include "GuiCreator.h"

class SceneManager{

public:
    static SceneManager* Instance();
    void pushSheet(GuiSheet* _sheet);
    void popSheet();
	void addInteractiveNode(GuiNode* _node);
    void draw();
    GuiSheet* getTopSheet();

private:
    SceneManager(){}
    static SceneManager* mInstance;
    vector<GuiSheet*> drawStack;
	vector<GuiNode*> interactiveNodes;
};

#endif // SCENEMANAGER_H_INCLUDED
