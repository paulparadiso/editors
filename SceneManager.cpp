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
    SubObMediator::Instance()->addObserver("clip-selected", this);
    frame.loadImage("cuts/wide_frame.png");
    frameArabic.loadImage("cuts/wide_frame_arabic.png");
    previewFrame.loadImage("cuts/pb_preview_frame.png");
    audioPreviewFrame.loadImage("cuts/audio_preview.png");
    noCustom.loadImage("cuts/voco_no_custom.png");
    noCustomArabic.loadImage("cuts/voco_no_custom_arabic.png");
    activeTimeline = "video-timeline";
    compositor = new Compositor();
    bReset = false;
}

void SceneManager::addTimeline(string _name, string _mode, string _pos){
    compositor->addTimeline(_name,_mode,_pos);
}

void SceneManager::setReset(){
    bReset = true;
}

void SceneManager::reset(){
    //drawStack.clear();
    //cout << "draw stack now has " << drawStack.size() << endl;
    compositor->reset();
    while(drawStack.size() >0){
        popSheet();
    }
    GuiConfigurator::Instance()->openSheet("attract");
    timeRemainingOnTrack.clear();
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
    if(drawStack.size() > 0){
        attrs["top-sheet"] = drawStack.back()->getName();
        SubObMediator::Instance()->update("sheet-changed", this);
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
        string buttonName = _sub->getAttr("name");
        //string target = _sub->getAttr("target");
        cout << "SceneManager received a button named " << buttonName << endl;
        if(buttonName == "switch"){
            displayArabic = !displayArabic;
        }
        if(buttonName == "english"){
            displayArabic = false;
        }
        if(buttonName == "arabic"){
            displayArabic = true;
        }
        string targetType = _sub->getAttr("target-type");
        string callerName = _sub->getAttr("name");
        if(targetType == "timeline"){
            activeTimeline = callerName;
            cout << "SceneManager set activeTimelne to " << activeTimeline << endl;
        }
    }
    if(_subName == "time-remaining"){
        //string track = _sub->getAttr("name");
        int time = ofToInt(_sub->getAttr("time"));
        cout << "time remaining for track " << activeTimeline << " updated to " << time << endl;
        timeRemainingOnTrack[activeTimeline] = time;
    }
    if(_subName == "clip-selected"){
        string clipName = _sub->getAttr("path");
        compositor->addClipToTimeline(activeTimeline, MediaCabinet::Instance()->getClip(clipName));
    }
 }

GuiSheet* SceneManager::getTopSheet(){
    return drawStack.back();
}

int SceneManager::getTimeRemainingOnTrack(){
   return timeRemainingOnTrack[activeTimeline];
}

void SceneManager::update(){
	//cout << "SceneManager drawing: drawStack size = " << drawStack.size() << endl;
    vector<GuiSheet*>::iterator gIter;
    for(gIter = drawStack.begin(); gIter != drawStack.end(); gIter++){
        (*gIter)->update();
    }
    compositor->update();
    if(bReset){
        reset();
        bReset = false;
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

void SceneManager::drawCompositor(int _x, int _y, int _sx, int _sy){
    compositor->draw(_x,_y,_sx,_sy);
}

void SceneManager::drawTimeRemaining(){
    map<string,int>::iterator mIter;
    mIter = timeRemainingOnTrack.find(activeTimeline);
    if(mIter == timeRemainingOnTrack.end()){
        timeRemainingOnTrack[activeTimeline] = 60;
    }
    if(!displayArabic){
        timeRemaining.draw(timeRemainingPos.x, timeRemainingPos.y);
        drawNumber(timeRemainingOnTrack[activeTimeline],450,251,41,25,1);
    } else {
        timeRemainingArabic.draw(timeRemainingPosArabic.x, timeRemainingPosArabic.y);
        drawNumber(timeRemainingOnTrack[activeTimeline],130,250,91,30,1);
    }
}

void SceneManager::drawFrame(int _x, int _y){
    if(displayArabic){
        frameArabic.draw(_x, _y);
    } else {
        frame.draw(_x,_y);
    }
}

void SceneManager::drawPreviewFrame(int _x, int _y){
    previewFrame.draw(_x,_y);
}

void SceneManager::drawAudioPreviewFrame(int _x, int _y){
    audioPreviewFrame.draw(_x,_y);
}

void SceneManager::drawNoCustom(){
    if(displayArabic){
        noCustomArabic.draw(200,320);
    } else {
        noCustom.draw(200,320);
    }
}
