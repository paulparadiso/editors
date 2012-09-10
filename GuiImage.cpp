#include "GuiImage.h"
#include "SceneManager.h"

GuiImage::GuiImage(map<string,string> &_attrs): GuiNode(){
    img.loadImage(_attrs["image"]);
    attrs = _attrs;
    initialize();
    haveArabic = false;
    map<string,string>::iterator mIter;
    mIter = attrs.find("arabic");
    if(mIter != attrs.end()){
        haveArabic = true;
        imgArabic.loadImage(attrs["arabic"]);
    }
}

void GuiImage::execute(){
}

void GuiImage::draw(){
    //ofSetColor(0,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
	if(haveArabic && SceneManager::Instance()->getDisplayArabic()){
        imgArabic.draw(pos.x,pos.y);
	} else {
       img.draw(pos.x,pos.y);
    }
}

void GuiImage::update(string _subName, Subject *_sub){
}

