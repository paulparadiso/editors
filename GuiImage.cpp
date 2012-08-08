#include "GuiImage.h"

GuiImage::GuiImage(map<string,string> &_attrs): GuiNode(){
    img.loadImage(_attrs["image"]);
    attrs = _attrs;
    initialize();
    map<string,string>::iterator mIter;
    mIter = attrs.find("arabic");
    haveArabic = false;
    displayArabic = false;
    if(mIter != attrs.end()){
        haveArabic = true;
        imgArabic.loadImage(attrs["arabic"]);
        SubObMediator::Instance()->addObserver("button", this);
    }
}

void GuiImage::execute(){
}

void GuiImage::draw(){
    //ofSetColor(0,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
	if(!displayArabic){
        img.draw(pos.x,pos.y);
	} else {
       imgArabic.draw(pos.x,pos.y);
    }
}

void GuiImage::update(string _subName, Subject *_sub){
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

