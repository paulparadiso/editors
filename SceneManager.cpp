#include "SceneManager.h"

SceneManager* SceneManager::mInstance = NULL;

SceneManager* SceneManager::Instance(){
    if(!mInstance){
        mInstance = new SceneManager();
    }
    return mInstance;
}

SceneManager::SceneManager(){
    haveMask = false;
    numberRenderer = new GuiNumberRenderer();
    timeRemaining.loadImage("cuts/time_remaining.png");
    timeRemainingArabic.loadImage("cuts/time_remaining_arabic.png");
    displayArabic = true;
    timeRemainingPos.set(120,245);
    timeRemainingPosArabic.set(230,245);
    SubObMediator::Instance()->addObserver("mouse-changed", this);
    SubObMediator::Instance()->addObserver("button", this);
    SubObMediator::Instance()->addObserver("time-remaining", this);
}

void SceneManager::reset(){
    //drawStack.clear();
    //cout << "draw stack now has " << drawStack.size() << endl;
    while(drawStack.size() > 0){
        popSheet();
    }
    GuiConfigurator::Instance()->openSheet("attract");
}

void SceneManager::addMask(GuiSheet* _mask){
    cout << "Scene manager adding mask." << endl;
    mask = _mask;
    haveMask = true;
}

void SceneManager::addInfoMask(GuiSheet* _infoMask){
    infoMask = _infoMask;
    haveMask = true;
}

void SceneManager::pushSheet(GuiSheet* _sheet){
	cout << "drawStack adding sheet - " << _sheet->getName() << endl;
    if(haveMask){
        cout << "adding mask" << endl;
        if(_sheet->getName() == "info"){
            drawStack.push_back(infoMask);
        } else {
            drawStack.push_back(mask);
        }
    }
    drawStack.push_back(_sheet);
    _sheet->activate();
    attrs["top-sheet"] = drawStack.back()->getName();
    SubObMediator::Instance()->update("sheet-changed", this);
    cout << "top sheet = " << attrs["top-sheet"] << endl;
}

void SceneManager::popSheet(){
    drawStack.pop_back();
    if(haveMask){
        drawStack.pop_back();
    }
    //attrs["top-sheet"] = drawStack.back()->getName();
    //SubObMediator::Instance()->update("sheet-changed", this);
    //cout << "top sheet = " << attrs["top-sheet"] << endl;
}

void SceneManager::update(string _subName, Subject *_sub){
    if(_subName == "mouse-changed"){
        string mouseState = _sub->getAttr("mouseState");
        int mouseX = ofToInt(_sub->getAttr("mouseX"));
        int mouseY = ofToInt(_sub->getAttr("mouseY"));
        //cout << "Received mouse change. State = " << mouseState << ", x = " << mouseX << ", y = " << mouseY << endl;
        drawStack.back()->checkNodes(mouseX,mouseY,mouseState);
    }
    if(_subName == "button"){
        string action = _sub->getAttr("action");
        string target = _sub->getAttr("target");
        if(target == "language" && action == "switch"){
            displayArabic = !displayArabic;
        }
    }
    if(_subName == "time-remaining"){
        string track = _sub->getAttr("name");
        int time = ofToInt(_sub->getAttr("time"));
        cout << "time remaining for track " << track << " updated to " << time << endl;
        timeRemainingOnTrack[track] = time;
    }
 }

GuiSheet* SceneManager::getTopSheet(){
    return drawStack.back();
}

void SceneManager::touchSheet(string _target){

}

void SceneManager::addInteractiveNode(GuiNode* _node){
	interactiveNodes.push_back(_node);
}

void SceneManager::update(){
	//cout << "SceneManager drawing: drawStack size = " << drawStack.size() << endl;
    vector<GuiSheet*>::iterator gIter;
    for(gIter = drawStack.begin(); gIter != drawStack.end(); gIter++){
        (*gIter)->update();
    }
}

void SceneManager::draw(){
	//cout << "SceneManager drawing: drawStack size = " << drawStack.size() << endl;
    if(drawStack.size() > 0){
        vector<GuiSheet*>::iterator gIter;
        for(gIter = drawStack.begin(); gIter != drawStack.end(); ++gIter){
            (*gIter)->draw();
        }
    }
}

void SceneManager::drawTimeRemaining(string _track){
    map<string,int>::iterator mIter;
    mIter = timeRemainingOnTrack.find(_track);
    if(mIter == timeRemainingOnTrack.end()){
        timeRemainingOnTrack[_track] = 60;
    }
    if(!displayArabic){
        timeRemaining.draw(timeRemainingPos.x, timeRemainingPos.y);
        drawNumber(timeRemainingOnTrack[_track],450,251,41,25,1);
    } else {
        timeRemainingArabic.draw(timeRemainingPosArabic.x, timeRemainingPosArabic.y);
        drawNumber(timeRemainingOnTrack[_track],130,250,91,30,1);
    }
}
