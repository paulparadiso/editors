#include "SceneManager.h"

SceneManager* SceneManager::mInstance = NULL;

SceneManager* SceneManager::Instance(){
    if(!mInstance){
        mInstance = new SceneManager();
    }
    return mInstance;
}

void SceneManager::pushSheet(GuiSheet* _sheet){
	cout << "drawStack adding sheet." << endl;
    drawStack.push_back(_sheet);
}

void SceneManager::popSheet(){
    drawStack.pop_back();
}

GuiSheet* SceneManager::getTopSheet(){
    return drawStack.back();
}

void SceneManager::addInteractiveNode(GuiNode* _node){
	interactiveNodes.push_back(_node);
}

void SceneManager::draw(){
	//cout << "SceneManager drawing: drawStack size = " << drawStack.size() << endl;
    vector<GuiSheet*>::iterator gIter;
    for(gIter = drawStack.begin(); gIter != drawStack.end(); gIter++){
        (*gIter)->draw();
    }
}
