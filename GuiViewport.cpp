#include "GuiViewport.h"
#include "SceneManager.h"

GuiViewport::GuiViewport(map<string,string> &_attrs) : GuiNode(){
    attrs = _attrs;
    initialize();
    size = stringToVec2f(attrs["size"]);
}

void GuiViewport::update(){
}

void GuiViewport::draw(){
    SceneManager::Instance()->drawCompositor(pos.x,pos.y,size.x,size.y);
}

void GuiViewport::message(map<string,string> _msg){
    cout << name << " told to " << _msg["action"] << endl;
    string action = _msg["action"];
    if(action == "play"){
        //Compositor::Instance()->start();
    } else if(action == "pause"){
        //Compositor::Instance()->pause();
    } else if(action == "rewind"){
        //Compositor::Instance()->rewind();
        //Compositor::Instance()->start();
    }

}
