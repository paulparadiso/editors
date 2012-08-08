#include "GuiNumberRenderer.h"

GuiNumberRenderer::GuiNumberRenderer(){
    lister.allowExt("png");
    lister.listDir("cuts/time_remaining");
    for(int i = 0; i < lister.size(); i++){
        string path = lister.getPath(i);
        vector<string> lineTokens = ofSplitString(path, "-");
        if(lineTokens.size() > 0){
            int number = ofToInt(ofSplitString(lineTokens[1], ".")[0]);
            timeRemainingEnglish[number].loadImage(path);
        }
    }
    lister.listDir("cuts/time_remaining_arabic");
    for(int i = 0; i < lister.size(); i++){
        string path = lister.getPath(i);
        vector<string> lineTokens = ofSplitString(path, "-");
        if(lineTokens.size() > 0){
            int number = ofToInt(ofSplitString(lineTokens[1], ".")[0]);
            timeRemainingArabic[number].loadImage(path);
        }
    }
    lister.listDir("cuts/clip_time_arabic");
    for(int i = 0; i < lister.size(); i++){
        string path = lister.getPath(i);
        vector<string> lineTokens = ofSplitString(path, "-");
        if(lineTokens.size() > 0){
            int number = ofToInt(ofSplitString(lineTokens[1], ".")[0]);
            clipTimeArabic[number].loadImage(path);
        }
    }
    displayArabic = false;
    SubObMediator::Instance()->addObserver("button", this);
}

GuiNumberRenderer::GuiNumberRenderer(string _path){
    /*
    lister.allowExt("png");
    lister.listDir(_path);
    for(int i = 0; i < lister.size(); i++){
        string path = lister.getPath(i);
        vector<string> lineTokens = ofSplitString(path, "-");
        if(lineTokens.size() > 0){
            int number = ofToInt(ofSplitString(lineTokens[1], ".")[0]);
            numberImages[number] = new ofImage(path);
        }
        //items.push_back(new VideoItem(lister.getPath(i)));
    }
    */
}

GuiNumberRenderer::~GuiNumberRenderer()
{
    //dtor
}

void GuiNumberRenderer::drawNumber(int _num, int _x, int _y, int _sx, int _sy, int _type){
    if(_type == 0){
        if(!displayArabic){
            timeRemainingEnglish[_num].draw(_x, _y, _sx, _sy);
        } else {
            clipTimeArabic[_num].draw(_x, _y, _sx, _sy);
        }
    }
    if(_type == 1){
        if(!displayArabic){
            timeRemainingEnglish[_num].draw(_x, _y, _sx, _sy);
        } else {
            timeRemainingArabic[_num].draw(_x, _y, _sx, _sy);
        }
    }
}

void GuiNumberRenderer::update(string _subName, Subject* _sub){
    if(_subName == "button"){
        string target = _sub->getAttr("target");
        string action = _sub->getAttr("action");
        if(target == "language" && action == "switch"){
            displayArabic = !displayArabic;
        }
    }
}
