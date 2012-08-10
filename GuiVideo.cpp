#include "GuiVideo.h"

GuiVideo::GuiVideo(map<string,string> &_attrs) : GuiButton(_attrs){
    vid.loadMovie(attrs["movie"]);
    map<string,string>::iterator mIter;
    mIter = attrs.find("arabic");
    if(mIter != attrs.end()){
        haveArabic = true;
        displayArabic = false;
        vidArabic.loadMovie(attrs["arabic"]);
        vidArabic.stop();
        vidArabic.setLoopState(OF_LOOP_NORMAL);
    }
    //vid.play();
    vid.stop();
    vid.setLoopState(OF_LOOP_NORMAL);
    SubObMediator::Instance()->addObserver("sheet-changed", this);
    SubObMediator::Instance()->addObserver("button", this);
}

void GuiVideo::update(){
    if(vid.isPlaying()){
        vid.update();
        vidArabic.update();
    }
}

void GuiVideo::update(string _subName, Subject* _sub){
    if(_subName == "sheet-changed"){
        string topSheet = _sub->getAttr("top-sheet");
        //cout << "video got sheet change.  New sheet = " << topSheet << endl;
        if(topSheet == parent){
            if(!vid.isPlaying()){
                cout << "video starting." << endl;
                vid.play();
                if(haveArabic){
                    vidArabic.play();
                }
            }
        } else {
            if(vid.isPlaying()){
                cout << "video stopping." << endl;
                vid.stop();
                if(haveArabic){
                    vidArabic.stop();
                }
            }
        }
    }
    if(_subName == "button"){
        string target = _sub->getAttr("target");
        string action = _sub->getAttr("action");
        if(target == "language" && action == "switch"){
            if(haveArabic){
                displayArabic = !displayArabic;
            }
        }
    }
}

void GuiVideo::draw(){
    if(!displayArabic){
        vid.draw(pos.x,pos.y,1680,1050);
    } else {
        vidArabic.draw(pos.x,pos.y,1680,1050);
    }
}
