#include "GuiCreator.h"
#include "GuiButton.h"
#include "SceneManager.h"
#include "GuiConfigurator.h"
#include "MediaCabinet.h"
#include "Compositor.h"

GuiCreator::GuiCreator(){
}

/*

GuiSheet::GuiSheet(){
    haveLoosie = false;
}

void GuiSheet::addNode(GuiNode* _node){
    nodes.push_back(_node);
    _node->setParent(name);
}

void GuiSheet::pushToStack(){
    SceneManager::Instance()->pushSheet(this);
}

void GuiSheet::activate(){
    if(type == "effects"){
        Compositor::Instance()->rewind();
        effectTarget = GuiConfigurator::Instance()->getGlobal("effect-target");
        cout << name << " of type " << type << " activating.  Adding effects to " << effectTarget << endl;
        int effectStatus = Compositor::Instance()->getEffectStatus(effectTarget);
        cout << "Effect status = " << effectStatus << endl;
        string effectButton = "effect_";
        effectButton += ofToString(effectStatus);
        vector<GuiNode*>::iterator gIter;
        for(gIter = nodes.begin(); gIter != nodes.end(); ++gIter){
            if((*gIter)->getName() == effectButton)
                (*gIter)->click();
        }
        //int clipEffectStatus = MediaCabinet::Instance()->getClip(effectTarget)->getEffectStatus();
        //cout << "current effect status of " << effectTarget << " = " << clipEffectStatus << endl;
    }
    if(type == "pager"){
        Compositor::Instance()->rewind();
    }
}

void GuiSheet::popFromStack(){
    SceneManager::Instance()->popSheet();
}

bool GuiSheet::checkNodes(int _x, int _y){
    vector<GuiNode*>::iterator gIter;
    for(gIter = nodes.begin(); gIter != nodes.end(); gIter++){
        if((*gIter)->isInside(_x,_y)){
            activatedNode = (*gIter);
            vector<GuiNode*>::iterator hIter;
            for(hIter = nodes.begin(); hIter != nodes.end(); hIter++){
                if((*hIter) != activatedNode){
                    (*hIter)->unclick();
                }
            }
            if(type == "effects" && activatedNode->getAttr("target") == "effects"){
                if(!activatedNode->getClicked()){
                    vector<string> tokens = ofSplitString(activatedNode->getName(),"_");
                    if(tokens.size() > 1){
                        cout << "setting effect status of " << effectTarget << endl;
                        Compositor::Instance()->setEffectStatus(effectTarget,ofToInt(tokens[1]));
                        activatedNode->click();
                    }
                } else {
                    Compositor::Instance()->setEffectStatus(effectTarget,0);
                    activatedNode->unclick();
                }
            }
            return true;
        }
    }
    return false;
}

bool GuiSheet::checkNodesForDrag(int _x, int _y){
    vector<GuiNode*>::iterator gIter;
    for(gIter = nodes.begin(); gIter != nodes.end(); gIter++){
        if((*gIter)->isDragging(_x,_y)){
            activatedNode = (*gIter);
            return true;
        }
    }
    return false;
}

bool GuiSheet::deliverMessage(map<string,string> _msg){
    vector<GuiNode*>::iterator nIter;
    for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
        if((*nIter)->getName() == _msg["target"]){
            (*nIter)->message(_msg);
            return true;
        }
    }
    return false;
}

void GuiSheet::reset(){
    vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
		(*nIter)->reset();
	}
}

void GuiSheet::setAllAttr(string _attr,string _val){
    vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
		(*nIter)->setAllAttr(_attr,_val);
	}
}

void GuiSheet::update(){
    vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
		(*nIter)->update();
	}
}

void GuiSheet::draw(){
	vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
		(*nIter)->draw();
	}
    if(haveLoosie){
        GuiConfigurator::Instance()->getLoosie()->drawLoosie();
    }
    if(name == "audio-recorder"){
        ofImage ball;
        ball.loadImage("cuts/ball_clipped.png");
        float pct = Compositor::Instance()->getRecordingPosition();
        int maxX = 837;
        int xDiff = maxX * pct;
        ofRect(711 + xDiff, 566,maxX - xDiff ,31);
        ball.draw(720 + xDiff - (ball.getWidth() / 2),561);
    }
}

*/

/*
GuiButton::GuiButton(map<string, string> &_attrs) : GuiNode(){
    attrs = _attrs;
    if(attrs["image"] != "none"){
        bg.loadImage(attrs["image"]);
        haveImage = true;
        size.x = bg.getWidth();
        size.y = bg.getHeight();
    } else {
        haveImage = false;
        size = stringToVec2f(attrs["size"]);
    }
    haveClicked = false;
    if(attrs["clicked"] != "none"){
        clicked.loadImage(attrs["clicked"]);
        haveClicked = true;
    }
    drawClicked = false;
    initialize();
}

GuiButton::GuiButton(string _img) : GuiNode(){
    bg.loadImage(_img);
    size.x = bg.getWidth();
    size.y = bg.getHeight();
    haveImage = true;
    drawClicked = false;
}

void GuiButton::execute(){
    //cout << "BANG!!! on " << attrs["name"] << endl;
    if(attrs["target-type"] == "timeline"){
        cout << "setting timeline as target - " << attrs["name"] << endl;
        GuiConfigurator::Instance()->setGlobal("timeline-target",attrs["name"]);
        GuiConfigurator::Instance()->setGlobal("clipInQuestion",attrs["path"]);
    }
    if(attrs["target"] == "video-effects" || attrs["target"] == "audio-effects"){
        cout << "setting effect target to " << name << endl;
        GuiConfigurator::Instance()->setGlobal("effect-target",name);
    }
    if(attrs["target-type"] == "timeline")
        GuiConfigurator::Instance()->setGlobal("caller",name);
    if(attrs["action"] == "open"){
        //cout << "activating - " << attrs["target"] << endl;
       GuiConfigurator::Instance()->openSheet(attrs["target"]);
    } else if(attrs["action"] == "close"){
        GuiConfigurator::Instance()->closeSheet(attrs["target"]);
    } else if(attrs["action"] == "replace"){
        GuiConfigurator::Instance()->replaceSheet(attrs["target"]);
    } else if(attrs["action"] == "message"){
        sendMessage();
        int levels = ofToInt(attrs["levels"]);
        for(int i = 0;i < levels; i++){
            GuiConfigurator::Instance()->closeSheet(attrs["target"]);
        }
    } else if(attrs["action"] == "delete"){
        msg["action"] = "delete";
        msg["target"] = GuiConfigurator::Instance()->getGlobal("active-timeline");
        msg["clip"] = GuiConfigurator::Instance()->getGlobal("caller");
        //cout << "deleting " << msg["clip"] << " from timeline " << ""
        sendMessage();
        GuiConfigurator::Instance()->closeSheet(attrs["target"]);
    } else if(attrs["action"] == "control"){
        if(attrs["target"] == "recorder"){
            if(attrs["message"] == "record"){
                Compositor::Instance()->startRecording();
            } else if(attrs["message"] == "stop"){
                Compositor::Instance()->stopRecording();
            } else if(attrs["message"] == "play"){
                Compositor::Instance()->playRecording();
            } else if(attrs["message"] == "save"){
                Compositor::Instance()->saveRecording();
            }
        } else {
            msg["action"] = attrs["message"];
            msg["target"] = attrs["target"];
            sendMessage();
        }
    } else if(attrs["action"] == "render-audio"){
        Compositor::Instance()->rewind();
        Compositor::Instance()->render();
    }
}

void GuiButton::click(){
    cout << name << " clicking" << endl;
    if(haveClicked)
        drawClicked = true;
}

void GuiButton::unclick(){
    cout << name << " uncliclicking" << endl;
    if(haveClicked)
        drawClicked = false;
}

void GuiButton::sendMessage(){
    cout << "sending message..." << "target = " << msg["target"] << endl;
    GuiConfigurator::Instance()->deliverMessage(msg);
}

void GuiButton::draw(){
    if(haveImage){
        if(!drawClicked){
            bg.draw(pos.x,pos.y);
        } else {
            clicked.draw(pos.x,pos.y);
        }
    }
}

*/

/*
TimelineButton::TimelineButton(float _length) : GuiButton(){
    totalWidth = 1400;
    curveRadius = 14;
    length = _length;
    if(length < 1.0){
        length = 1.0;
    }
    height = 69;
    width = totalWidth * length / 60;
    setSize(width + 9,height);
    curveBuffer = curveRadius * 2;
    widthSegment = width - curveBuffer;
    heightSegment = height - curveBuffer;
    makeDropShadow();
    makeButton();
    makeInnerCurve();
    makeOuterCurve();
    if(_length > 2.0){
        img.loadImage("cuts/timeline_button_audio_big_blue.png");
    } else {
        img.loadImage("cuts/timeline_button_saudio_small_blue.png");
    }
    bLoosie = false;
}

void TimelineButton::setTotalLength(float _length){
    length = _length;
}

void TimelineButton::makeDropShadow(){
    dropShadow.setFillColor(ofColor(120,120,120,127));
    dropShadow.setFilled(true);
    dropShadow.moveTo(curveRadius, 0);
    dropShadow.lineTo(width - curveRadius,0);
    dropShadow.bezierTo(width,0,width,0,width, curveRadius);
    dropShadow.lineTo(width, height - curveRadius);
    dropShadow.bezierTo(width,height,width,height, width - curveRadius, height);
    dropShadow.lineTo(curveRadius, height);
    dropShadow.bezierTo(0, height, 0, height, 0, height - curveRadius);
    dropShadow.lineTo(0, curveRadius);
    dropShadow.bezierTo(0,0,0,0, curveRadius, 0);
    dropShadow.translate(ofVec2f(10,10));
}

void TimelineButton::makeButton(){
    buttonPath.setFillColor(ofColor(43,172,226));
    buttonPath.setFilled(true);
    buttonPath.moveTo(curveRadius, 0);
    buttonPath.lineTo(width - curveRadius,0);
    buttonPath.bezierTo(width,0,width,0,width, curveRadius);
    buttonPath.lineTo(width, height - curveRadius);
    buttonPath.bezierTo(width,height,width,height, width - curveRadius, height);
    buttonPath.lineTo(curveRadius, height);
    buttonPath.bezierTo(0, height, 0, height, 0, height - curveRadius);
    buttonPath.lineTo(0, curveRadius);
    buttonPath.bezierTo(0,0,0,0, curveRadius, 0);
    buttonPath.translate(ofVec2f(2,2));
}

void TimelineButton::makeInnerCurve(){
    innerCurve.setCurveResolution(20);
    innerCurve.setFillColor(ofColor(128,215,247));
    innerCurve.moveTo(curveRadius, 0);
    innerCurve.lineTo(width - curveRadius,0);
    innerCurve.bezierTo(width,0,width,0,width, curveRadius);
    innerCurve.lineTo(width, curveRadius * 2);
    innerCurve.bezierTo(width / 2, height / 4 * 3 ,width / 2, height / 4 *3, 0, curveRadius * 2);
    innerCurve.lineTo(0, curveRadius);
    innerCurve.bezierTo(0,0,0,0, curveRadius, 0);
    innerCurve.tessellate();
    innerCurve.translate(ofVec2f(2,2));
}

void TimelineButton::makeOuterCurve(){
    outerCurve.setStrokeWidth(5);
    outerCurve.setFilled(false);
    outerCurve.moveTo(curveRadius, 0);
    outerCurve.lineTo(width - curveRadius,0);
    outerCurve.bezierTo(width,0,width,0,width, curveRadius);
    outerCurve.lineTo(width, height - curveRadius);
    outerCurve.bezierTo(width,height,width,height, width - curveRadius, height);
    outerCurve.lineTo(curveRadius, height);
    outerCurve.bezierTo(0, height, 0, height, 0, height - curveRadius);
    outerCurve.lineTo(0, curveRadius);
    outerCurve.bezierTo(0,0,0,0, curveRadius, 0);
    outerCurve.translate(ofVec2f(2,2));
}

void TimelineButton::update(){
}

void TimelineButton::draw(){
    if(!bLoosie){
        outerCurve.setStrokeColor(ofColor(255,255,255));
    } else {
        outerCurve.setStrokeColor(ofColor(141,198,63));
    }
    ofPushMatrix();
    ofTranslate(pos.x,pos.y,0);
    dropShadow.draw();
    buttonPath.draw();
    innerCurve.draw();
    outerCurve.draw();
    img.draw(width / 2 - img.getWidth() / 2, height / 2 - img.getHeight() / 2);
    ofPopMatrix();
}

void TimelineButton::drawLoosie(){
    bLoosie = true;
    draw();
    bLoosie = false;
}
*/

/*

GuiMovie::GuiMovie(map<string,string> &_attrs) : GuiButton(_attrs){
    mov.loadMovie(attrs["movie"]);
    mov.play();
    mov.setLoopState(OF_LOOP_NORMAL);
}

void GuiMovie::update(){
    mov.update();
}

void GuiMovie::draw(){
    mov.draw(pos.x,pos.y,size.x,size.y);
}

*/

/*
Image::Image(map<string,string> &_attrs): GuiNode(){
    img.loadImage(_attrs["image"]);
    attrs = _attrs;
    initialize();
}

void Image::execute(){
}

void Image::draw(){
    //ofSetColor(0,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
	img.draw(pos.x,pos.y);
}
*/

/*
MediaPreview::MediaPreview(map<string,string> &_attrs): GuiNode(){
    attrs = _attrs;
    //initialize();
    type = attrs["type"];
    exit = NULL;
    select = NULL;
    play = NULL;
    if(type == "video"){
        setupVideo();
    } else if (type == "audio"){
        setupAudio();
    }
}

void MediaPreview::setupVideo(){
    preview = NULL;
    frame.loadImage("cuts/pb_preview_frame.png");
    exit = new GuiButton("cuts/preview_exit.png");
    select = new GuiButton("cuts/preview_add.png");
    exit->setAttr("action", "close");
    select->setAttr("action", "message");
    select->setAttr("levels", "2");
    selectOffset.x = 40;
    selectOffset.y = 320;
    exitOffset.x = 770;
    exitOffset.y = -50;
}

void MediaPreview::setupAudio(){
    preview = NULL;
    frame.loadImage("cuts/audio_preview.png");
    ball.loadImage("cuts/ball_clipped.png");
    exit = new GuiButton("cuts/preview_exit.png");
    select = new GuiButton("cuts/preview_add.png");
    play = new GuiButton("cuts/preview_add.png");
    exit->setAttr("action", "close");
    select->setAttr("action", "message");
    select->setAttr("levels", "2");
    selectOffset.x = 230;
    selectOffset.y = 505;
    exitOffset.x = 1525;
    exitOffset.y = 395;
    playOffset.x = 400;
    playOffset.y = 515;
}

void MediaPreview::setSelectTarget(string _target){
    select->setAttr("target", _target);
}

void MediaPreview::execute(){
}

void MediaPreview::setPosition(ofVec2f _pos){
    pos = _pos;
    exit->setPosition(_pos + exitOffset);
    select->setPosition(_pos + selectOffset);
    if(play){
        play->setPosition(_pos + playOffset);
    }
}

void MediaPreview::update(){
    if(!preview){
        preview = MediaCabinet::Instance()->getLastClip();
        cout << "getting clip." << endl;
        MediaCabinet::Instance()->increaseClipHold(preview);
        preview->startPreview();
    } else {
        preview->update();

    }
}

void MediaPreview::draw(){
    if(!preview){
        preview = MediaCabinet::Instance()->getLastClip();
        preview->startPreview();
    }
    if(type == "video"){
        preview->drawPreview(pos.x - 15 , pos.y + 5, 850, 475);
        //frame.draw(pos.x,pos.y);
        frame.draw(0,0);
        select->draw();
        //exit->draw();
    } else if(type == "audio"){
        float pct = preview->getPosition();
        int maxX = 937;
        int xDiff = 937 * pct;
        frame.draw(pos.x ,pos.y);
        ofRect(pos.x + 574 + xDiff, pos.y + 559,937 - xDiff ,31);
        ball.draw(pos.x + 574 + xDiff - (ball.getWidth() / 2), pos.y + 559 - 3);
        //ofRect(play->getPos().x,play->getPos().y,play->getSize().x, play->getSize().y);
    }
}

bool MediaPreview::isInside(int _x, int _y){
    if(exit->isInside(_x,_y)){
        exit->execute();
        preview->stopPreview();
        //MediaCabinet::Instance()->removeClip(preview);
        MediaCabinet::Instance()->decreaseClipHold(preview);
        preview = NULL;
        return true;
    }
    if(select->isInside(_x,_y)){
        select->execute();
        preview->stopPreview();
        MediaCabinet::Instance()->decreaseClipHold(preview);
        preview = NULL;
        return true;
    }
    if(play){
        if(play->isInside(_x,_y)){
            preview->startPreview();
        }
    }
    return false;
}

void MediaPreview::message(map<string,string> _msg){
    cout << name << " got message" << endl;
    string action = _msg["action"];
    if(action == "replay"){
        preview->startPreview();
    }
}
*/

/*
TimelineGui::TimelineGui(map<string,string> &_attrs): GuiNode(){
    attrs = _attrs;
    mode = attrs["mode"];
    initialize();
    clipCounter = 0;
    currentTime = 0;
    maxTime = 60;
    Compositor::Instance()->addTimeline(attrs["name"]);
    timeline = Compositor::Instance()->getTimeline(attrs["name"]);
}

void TimelineGui::message(map<string,string> _msg){
    cout << name << " got new message" << endl;
    string action = _msg["action"];
    if(action == "add")
        addItem(_msg);
    if(action == "delete")
        deleteItem(_msg);
}

void TimelineGui::addItem(map<string,string> _msg){
    cout << "adding " << _msg["path"] << " to timeline." << endl;
    float length = ofToFloat(_msg["duration"]);
    if(currentTime + length > maxTime + 1)
        return;
    currentTime += length;
    if(mode == "video" || mode == "image"){
        addVideo(length, _msg["path"]);
    }
    if(mode == "audio"){
        addAudio(length, _msg["path"]);
    }
    //Compositor::Instance()->addClipToTimeline(attrs["name"], MediaCabinet::Instance()->getClip(_msg["path"]));
}

void TimelineGui::deleteItem(map<string,string> _msg){
    cout << _msg["target"] << " deleting " << _msg["clip"] << endl;
    string clipToDelete = GuiConfigurator::Instance()->getGlobal("clipInQuestion");
    list<TimelineButton*>::iterator iIter;
    for(iIter = images.begin(); iIter != images.end(); ++iIter){
        if((*iIter)->getName() == _msg["clip"]){
            cout << "deleting - " << (*iIter)->getName() << endl;
            currentTime -= (*iIter)->getDur();
            delete (*iIter);
            images.erase(iIter);
            break;
        }
    }
    //Clip *clip = MediaCabinet::Instance()->getClip(clipToDelete);
    Compositor::Instance()->removeClipFromTimeline(name, _msg["clip"]);
}

void TimelineGui::addVideo(float _length, string _path){
    cout << "timeline adding video - " << _path << " length = " << _length << endl;
    images.push_back(new GuiTimelineButton(_length));
    string tmpName = _path + ofToString(clipCounter++);
    images.back()->setName(tmpName);
    images.back()->setAttr("action","open");
    images.back()->setAttr("target","video-effects");
    images.back()->setAttr("target-type","timeline");
    images.back()->setAttr("path",_path);
    images.back()->setAttr("active-timeline",name);
    images.back()->setDur(_length);
    Compositor::Instance()->addClipToTimeline(name,MediaCabinet::Instance()->getClip(_path), images.back()->getName());
    //images.back()->setAttr("dur","15");
}

void TimelineGui::addAudio(float _length, string _path){
    images.push_back(new TimelineButton(_length));
    //images.back()->setName(name + ofToString(clipCounter++));
    string tmpName = _path + ofToString(clipCounter++);
    images.back()->setName(tmpName);
    images.back()->setAttr("action","open");
    images.back()->setAttr("target","audio-effects");
    images.back()->setAttr("target-type","timeline");
    images.back()->setAttr("path", _path);
    images.back()->setAttr("active-timeline",name);
    //images.back()->setAttr("length",ofToString(_length));
    images.back()->setDur(_length);
    Compositor::Instance()->addClipToTimeline(name,MediaCabinet::Instance()->getClip(_path), images.back()->getName());
}

bool TimelineGui::isInside(int _x, int _y){
    list<TimelineButton*>::iterator iIter;
    for(iIter = images.begin(); iIter != images.end(); ++iIter){
        if((*iIter)->isInside(_x, _y)){
            GuiConfigurator::Instance()->setGlobal("active-timeline", name);
            GuiConfigurator::Instance()->addLoosie((*iIter));
            (*iIter)->execute();
            return true;
        }
    }
    return false;
}

void TimelineGui::execute(){
}

void TimelineGui:: draw(){
    //ofSetColor(255,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
    ofVec2f drawPos = pos;
    drawPos.y += 5;
    list<TimelineButton*>::iterator iIter;
    for(iIter = images.begin(); iIter != images.end(); ++iIter){
        (*iIter)->setPosition(drawPos);
        drawPos.x += (*iIter)->getSize().x - 3;
        (*iIter)->draw();
    }
}
*/

/*
Viewport::Viewport(map<string,string> &_attrs) : GuiNode(){
    attrs = _attrs;
    initialize();
    size = stringToVec2f(attrs["size"]);
}

void Viewport::update(){
    Compositor::Instance()->update();
}

void Viewport::draw(){
    Compositor::Instance()->draw(pos.x,pos.y,size.x,size.y);
}

void Viewport::message(map<string,string> _msg){
    cout << name << " told to " << _msg["action"] << endl;
    string action = _msg["action"];
    if(action == "play"){
        Compositor::Instance()->start();
    } else if(action == "pause"){
        Compositor::Instance()->pause();
    } else if(action == "rewind"){
        Compositor::Instance()->rewind();
        Compositor::Instance()->start();
    }

}
*/

/*

Scrubber::Scrubber(map<string,string> &_attrs){
    attrs = _attrs;
    initialize();
    ball.loadImage("cuts/ball_clipped.png");
    stick.loadImage("cuts/scrubber_stick.png");
    size.x = ball.getWidth();
    size.y = ball.getHeight();
    homeX = pos.x;
    lastX = homeX;
    maxX = 1446;
    lastPct = 0.0;
}

void Scrubber::execute(){
    Compositor::Instance()->pause();
}

bool Scrubber::isDragging(int _x, int _y){
    ofVec2f actualPos(pos.x - ball.getWidth() / 2, pos.y);
    if((_x > actualPos.x && _x < actualPos.x + size.x) &&
       (_y > actualPos.y && _y < actualPos.y + size.y)){
        return true;
    }
    return false;
}

void Scrubber::executeDrag(int _x, int _y){
    //cout << "scrubber moving..." << endl;
    pos.x += _x - pos.x;
    if(pos.x < homeX){
        pos.x = homeX;
    }
    if(pos.x > maxX){
        pos.x = maxX;
    }
    pct = (pos.x - homeX) / (maxX - homeX);
    if(abs(pct - lastPct) > 0.01){
        Compositor::Instance()->setScrubberPosition(pct);
        lastPct = pct;
    }
}

void Scrubber::update(){
    //float pct = Compositor::Instance()->getScrubberPosition();
    pct = lastPct = Compositor::Instance()->getScrubberPosition();
    pos.x = homeX + (maxX - homeX) * pct;
}

void Scrubber::draw(){
    stick.draw(pos.x - stick.getWidth() / 2, pos.y);
    ball.draw(pos.x - ball.getWidth() / 2, pos.y);
}

*/
