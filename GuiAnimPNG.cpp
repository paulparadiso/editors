#include "GuiAnimPNG.h"
#include "Utils.h"

GuiAnimPNG::GuiAnimPNG(map<string,string> &_attrs) : GuiNode()
{
    attrs = _attrs;
    size = stringToVec2f(attrs["size"]);
    initialize();
    animations[0].loadImage("loadingPNGS/loading_animation_1.png");
    animations[1].loadImage("loadingPNGS/loading_animation_2.png");
    animations[2].loadImage("loadingPNGS/loading_animation_3.png");
    animations[3].loadImage("loadingPNGS/loading_animation_4.png");
    animations[4].loadImage("loadingPNGS/loading_animation_5.png");
    animations[5].loadImage("loadingPNGS/loading_animation_6.png");
    animations[6].loadImage("loadingPNGS/loading_animation_7.png");
    animations[7].loadImage("loadingPNGS/loading_animation_8.png");
    currentAnimation = 0;
    switchDelay = 1.0 / 8.0;
    timeOfLastSwitch = ofGetElapsedTimef();
}

void GuiAnimPNG::update(){
    if(ofGetElapsedTimef() > timeOfLastSwitch + switchDelay){
        currentAnimation = (currentAnimation + 1) % 8;
        timeOfLastSwitch = ofGetElapsedTimef();
    }
}

void GuiAnimPNG::draw(){
    //ofEnableAlphaBlending();
    animations[currentAnimation].draw(pos.x, pos.y, size.x, size.y);
    //ofDisableAlphaBlending();
}

GuiAnimPNG::~GuiAnimPNG()
{
    //dtor
}
