/*
 *  GuiMediaPreview.cpp
 *  emptyExample
 *
 *  Created by Paul Paradiso on 7/25/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "GuiMediaPreview.h"
#include "SceneManager.h"

GuiMediaPreview::GuiMediaPreview(map<string,string> &_attrs): GuiNode(){
    attrs = _attrs;
    //initialize();
    type = attrs["type"];
    exit = NULL;
    select = NULL;
    play = NULL;
    havePreview = false;
    //cout << "================>>>>>>>>>>>MEDIAPREVIEW<<<<<<<<<<<============== " << type << endl;
    if(type == "video" || type == "image"){
        setupVideo();
    } else if (type == "audio"){
        setupAudio();
    }
}

void GuiMediaPreview::setupVideo(){
    //frame.loadImage("cuts/pb_preview_frame.png");
    exit = new GuiButton("cuts/preview_exit.png");
    select = new GuiButton("cuts/preview_add.png");
    if(type == "video"){
        play = new GuiButton("cuts/pb_preview_replay.png");
        play->setAttr("sfx","button11");
    }
    exit->setAttr("action", "close");
    exit->setChannel("button");
    exit->setAttr("sfx","button11");
    select->setAttr("action", "message");
    select->setAttr("levels", "2");
    select->setChannel("clip-selected");
    select->setAttr("sfx","button11");
	selectOffset.x = 40;
    selectOffset.y = 320;
    exitOffset.x = 770;
    exitOffset.y = -50;
    playOffset.x = 650;
    playOffset.y = 320;
}

void GuiMediaPreview::setupAudio(){
    preview = NULL;
    //frame.loadImage("cuts/audio_preview.png");
    ball.loadImage("cuts/ball_clipped.png");
    exit = new GuiButton("cuts/preview_exit.png");
    exit->setAttr("action", "close");
    exit->setChannel("button");
    exit->setAttr("sfx","button11");
    select = new GuiButton("cuts/preview_add.png");
    play = new GuiButton("cuts/preview_add.png");
    play->setAttr("sfx","button11");
    select->setAttr("action", "message");
    select->setAttr("levels", "2");
    select->setChannel("clip-selected");
    select->setAttr("sfx","button11");
    selectOffset.x = 230;
    selectOffset.y = 505;
    exitOffset.x = 1525;
    exitOffset.y = 395;
    playOffset.x = 400;
    playOffset.y = 515;
}

void GuiMediaPreview::setSelectTarget(string _target){
    select->setAttr("target", _target);
}

void GuiMediaPreview::execute(){
}

void GuiMediaPreview::activate(){
    /*
    if(preview){
        preview->startPreview();
    }
    */
}

void GuiMediaPreview::setPosition(ofVec2f _pos){
    pos = _pos;
    exit->setPosition(_pos + exitOffset);
    select->setPosition(_pos + selectOffset);
    if(play){
        play->setPosition(_pos + playOffset);
    }
}

void GuiMediaPreview::update(){
    if(!havePreview){
        preview = MediaCabinet::Instance()->getLastClip();
        cout << "getting clip." << endl;
        MediaCabinet::Instance()->increaseClipHold(preview);
        //preview->startPreview();
        //cout << "setting select path to " << preview->getName() << endl;
        select->setAttr("path", preview->getName());
        havePreview = true;
    } else {
        preview->update();
    }
}

void GuiMediaPreview::draw(){
    if(!havePreview){
        preview = MediaCabinet::Instance()->getLastClip();
        //preview->startPreview();
        havePreview = true;
    }
    if(type == "video" || type == "image"){
        preview->drawPreview(pos.x - 15 , pos.y + 5, 850, 475);
        //SceneManager::Instance()->drawPreviewFrame(pos.x - 15 , pos.y + 5, 850, 475);
        //frame.draw(pos.x,pos.y);
        //frame.draw(0,0);
        SceneManager::Instance()->drawPreviewFrame(0,0);
        select->draw();
        if(type == "video"){
            play->draw();
        }
        //exit->draw();
    } else if(type == "audio"){
        float pct = preview->getPosition();
        int maxX = 937;
        int xDiff = 937 * pct;
        //frame.draw(pos.x ,pos.y);
        SceneManager::Instance()->drawAudioPreviewFrame(pos.x, pos.y);
        ofRect(pos.x + 574 + xDiff, pos.y + 559,937 - xDiff ,31);
        ball.draw(pos.x + 574 + xDiff - (ball.getWidth() / 2), pos.y + 559 - 3);
        //ofRect(play->getPos().x,play->getPos().y,play->getSize().x, play->getSize().y);
    }
}

bool GuiMediaPreview::isInside(int _x, int _y){
    if(exit->isInside(_x,_y)){
        exit->execute();
        preview->stopPreview();
        //MediaCabinet::Instance()->removeClip(preview);
        MediaCabinet::Instance()->decreaseClipHold(preview);
        havePreview = false;
        return true;
    }
    if(select->isInside(_x,_y)){
        select->execute();
        preview->stopPreview();
        MediaCabinet::Instance()->decreaseClipHold(preview);
        havePreview = false;
        return true;
    }
    if(play){
        if(play->isInside(_x,_y)){
            preview->startPreview();
        }
    }
    return false;
}

bool GuiMediaPreview::processMouse(int _x, int _y, int _state){
	if(_state == MOUSE_STATE_DOWN){
		if(exit->isInside(_x, _y)){
			exit->execute();
			preview->stopPreview();
			MediaCabinet::Instance()->decreaseClipHold(preview);
			havePreview = false;
			preview = NULL;
			return true;
		}
        if(select->isInside(_x,_y)){
            select->execute();
            //select->setChannel("button");
            preview->stopPreview();
            MediaCabinet::Instance()->decreaseClipHold(preview);
            havePreview = false;
            preview = NULL;
            cout << "MediaPreview done." << endl;
            return true;
        }
        if(play){
            if(play->isInside(_x,_y)){
                preview->startPreview();
            }
        }
	}
    return false;
}

void GuiMediaPreview::message(map<string,string> _msg){
    cout << name << " got message" << endl;
    string action = _msg["action"];
    if(action == "replay"){
        preview->startPreview();
    }
}
