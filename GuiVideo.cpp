#include "GuiVideo.h"

GuiVideo::GuiVideo(map<string,string> &_attrs) : GuiButton(_attrs){
    vid.loadMovie(attrs["movie"]);
    //vid.play();
    vid.stop();
    vid.setLoopState(OF_LOOP_NORMAL);
    SubObMediator::Instance()->addObserver("sheet-changed", this);
}

void GuiVideo::update(){
    if(vid.isPlaying()){
        vid.update();
    }
}

void GuiVideo::update(Subject* _sub){
    string topSheet = _sub->getAttr("top-sheet");
    //cout << "video got sheet change.  New sheet = " << topSheet << endl;
    if(topSheet == parent){
        if(!vid.isPlaying()){
            cout << "video starting." << endl;
            vid.play();
        }
    } else {
        if(vid.isPlaying()){
            cout << "video stopping." << endl;
            vid.stop();
        }
    }
}

void GuiVideo::draw(){
    vid.draw(pos.x,pos.y,size.x,size.y);
}
