#include "GuiImage.h"

GuiImage::GuiImage(map<string,string> &_attrs): GuiNode(){
    img.loadImage(_attrs["image"]);
    attrs = _attrs;
    initialize();
}

void GuiImage::execute(){
}

void GuiImage::draw(){
    //ofSetColor(0,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
	img.draw(pos.x,pos.y);
}

