#include "GuiVideo.h"

GuiVideo::GuiVideo(map<string,string> &_attrs) : GuiButton(_attrs){
    //vid.loadMovie(attrs["movie"]);
    haveArabic = false;
    displayArabic = false;
    map<string,string>::iterator mIter;
    mIter = attrs.find("arabic");
    /*
    if(mIter != attrs.end()){
        haveArabic = true;
        displayArabic = false;
        vidArabic.loadMovie(attrs["arabic"]);
        vidArabic.stop();
        vidArabic.setLoopState(OF_LOOP_NORMAL);
    }
    */
    //vid.play();
    //vid.stop();
    //vid.setLoopState(OF_LOOP_NORMAL);
    SubObMediator::Instance()->addObserver("sheet-changed", this);
    SubObMediator::Instance()->addObserver("button", this);
    vid = NULL;
}

void GuiVideo::activate(){
    if(haveDelay){
        startTime = ofGetElapsedTimef();
        checkDelay = true;
    }
    //vid.loadMovie(attrs["movie"]);
}

void GuiVideo::update(){
    if(vid){
        vid->update();
        //vidArabic.update();
    }
    if(haveDelay && checkDelay){
        if(ofGetElapsedTimef() > startTime + delayTime){
            execute();
            checkDelay = false;
        }
    }
}

void GuiVideo::update(string _subName, Subject* _sub){
    if(_subName == "sheet-changed"){
        string topSheet = _sub->getAttr("top-sheet");
        //cout << "video got sheet change.  New sheet = " << topSheet << endl;
        if(topSheet == parent){
            if(!vid){
                cout << "video starting." << endl;
                vid = new ofVideoPlayer();
                vid->loadMovie(attrs["movie"]);
                vid->play();
                vid->setLoopState(OF_LOOP_NORMAL);
                /*
                if(haveArabic){
                    vidArabic.play();
                }
                */
            }
        } else {
            if(vid){
                cout << "video stopping." << endl;
                vid->stop();
                vid->closeMovie();
                delete vid;
                vid = NULL;
                /*
                if(haveArabic){
                    vidArabic.stop();
                }
                */
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
    /*
    if(!displayArabic){
        vid.draw(pos.x,pos.y);
    } else {
        vidArabic.draw(pos.x,pos.y);
    }
    */
    if(vid){
        vid->draw(pos.x,pos.y);
    }
}
