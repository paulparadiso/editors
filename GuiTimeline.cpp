#include "GuiTimeline.h"

GuiTimelineButton::GuiTimelineButton(float _length) : GuiButton(){
    totalWidth = 1400;
    colors["none"] = new ofColor(255,255,255);
    colors["effect_1"] = new ofColor(172, 10, 117);
    colors["effect_2"] = new ofColor(190, 30, 45);
    colors["effect_3"] = new ofColor(28, 100, 10);
    colors["effect_4"] = new ofColor(141, 198, 63);
    activeEffect = "none";
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
    SubObMediator::Instance()->addObserver("button", this);
    bAwaitingResponse = false;
}

void GuiTimelineButton::update(string _subName, Subject* _sub){
    if(_sub->getAttr("target-type") == "effect"){
        if(bAwaitingResponse){
            string selectedEffect = _sub->getAttr("name");
            if(selectedEffect == activeEffect){
                activeEffect = "none";
            } else {
                activeEffect = selectedEffect;
            }
            cout << "active effect = " << activeEffect << endl;
            outerCurve.setStrokeColor(*colors[activeEffect]);
            bAwaitingResponse = false;
        }
    }
}

void GuiTimelineButton::setTotalLength(float _length){
    length = _length;
}

void GuiTimelineButton::makeDropShadow(){
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

void GuiTimelineButton::makeButton(){
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

void GuiTimelineButton::makeInnerCurve(){
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

void GuiTimelineButton::makeOuterCurve(){
    outerCurve.setStrokeWidth(5);
    //outerCurve.setFillColor(ofColor(0,0,0));
    //outerCurve.setUseShapeColor(true);
    outerCurve.setFilled(false);
    //green.set(0,255,0);
    outerCurve.setStrokeColor(*colors[activeEffect]);
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

void GuiTimelineButton::update(){
}

void GuiTimelineButton::draw(){
    /*
    if(!bLoosie){
        outerCurve.setStrokeColor(ofColor(255,255,255));
    } else {
        outerCurve.setStrokeColor(ofColor(141,198,63));
    }
    */
    ofPushMatrix();
    ofTranslate(pos.x,pos.y,0);
    dropShadow.draw();
    buttonPath.draw();
    innerCurve.draw();
    outerCurve.draw();
    img.draw(width / 2 - img.getWidth() / 2, height / 2 - img.getHeight() / 2);
    ofPopMatrix();
}

void GuiTimelineButton::drawLoosie(){
    bLoosie = true;
    draw();
    bLoosie = false;
}

GuiTimeline::GuiTimeline(map<string,string> &_attrs): GuiNode(){
    attrs = _attrs;
    mode = attrs["mode"];
    initialize();
    clipCounter = 0;
    currentTime = 0;
    maxTime = 60;
    Compositor::Instance()->addTimeline(attrs["name"], attrs["mode"]);
    //timeline = Compositor::Instance()->getTimeline(attrs["name"]);
    SubObMediator::Instance()->addObserver("new-timeline-clip", this);
    SubObMediator::Instance()->addObserver("button", this);
}

void GuiTimeline::message(map<string,string> _msg){
    cout << name << " got new message" << endl;
    string action = _msg["action"];
    if(action == "add")
        addItem(_msg);
    if(action == "delete")
        deleteItem(_msg);
}

void GuiTimeline::update(string _subName, Subject* _sub){
    if(_subName == "new-timeline-clip"){
        if(_sub->getAttr("active-timeline") == attrs["name"]){
            //cout << "GuiTimeline adding clip with id " << _sub->getAttr("new-clip-id") << endl;
            addItem(_sub->getAttr("new-clip-mode"), _sub->getAttr("new-clip-id"), ofToFloat(_sub->getAttr("new-clip-length")));
        }
    }
    if(_subName == "button"){
        if(_sub->getAttr("target-type") == "timeline"){
            if(_sub->getAttr("name") == "trash"){
                //cout << "timeline asked to find and delete clip." << endl;
                findAndDeleteItem();
            }
        }
    }
}

void GuiTimeline::addItem(string _mode, string _id, float _duration){
    if(_mode == "video" || _mode == "image"){
        addVideo(_duration,_id);
    }
    if(mode == "audio"){
        addAudio(_duration, _id);
    }
}

void GuiTimeline::addItem(map<string,string> _msg){
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

void GuiTimeline::findAndDeleteItem(){
    //cout << "checking for items to delete." << endl;
    list<GuiTimelineButton*>::iterator iIter;
    for(iIter = images.begin(); iIter != images.end(); ++iIter){
        if((*iIter)->getIsAwaitingResponse()){
            cout << "deleting - " << (*iIter)->getAttr("name") << endl;
            //currentTime -= (*iIter)->getDur();
            setAttr("clip-to-remove", (*iIter)->getAttr("name"));
            cout << "setting clip-to-remove to " << (*iIter)->getAttr("name") << endl;
            SubObMediator::Instance()->update("timeline-clip-removed", this);
            attrs.erase("clip-to-remove");
            delete (*iIter);
            images.erase(iIter);
            cout << "GuiTimeline now has " << images.size() << " item(s)." << endl;
            break;
        }
    }
}

void GuiTimeline::deleteItem(map<string,string> _msg){
    cout << _msg["target"] << " deleting " << _msg["clip"] << endl;
    string clipToDelete = GuiConfigurator::Instance()->getGlobal("clipInQuestion");
    list<GuiTimelineButton*>::iterator iIter;
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

void GuiTimeline::addVideo(float _length, string _path){
    cout << "timeline adding video - " << _path << " length = " << _length << endl;
    /*
    if(_length == 5){
        images.push_back(new GuiButton("cuts/timeline_icon_5.png"));
    }
    if(_length == 10){
        images.push_back(new GuiButton("cuts/timeline_icon_10.png"));
    }
    if(_length == 15){
        images.push_back(new GuiButton("cuts/timeline_icon_15.png"));
    }
    if(_length == 30){
        images.push_back(new GuiButton("cuts/timeline_icon_30.png"));
    }
    //cout << "images size = " << images.size() << endl;
    */
    images.push_back(new GuiTimelineButton(_length));
    cout << "now have " << images.size() << " button(s) on timeline." << endl;
    //string tmpName = _path + ofToString(clipCounter++);
    //images.back()->setName(_path);
    images.back()->setChannel("button");
    images.back()->setAttr("action","open");
    images.back()->setAttr("target","video-effects");
    images.back()->setAttr("target-type","timeline");
    images.back()->setAttr("name",_path);
    images.back()->setAttr("active-timeline",name);
    images.back()->setDur(_length);
    //Compositor::Instance()->addClipToTimeline(name,MediaCabinet::Instance()->getClip(_path), images.back()->getName());
    //images.back()->setAttr("dur","15");
}

void GuiTimeline::addAudio(float _length, string _path){
    /*
    if(_length == 1){
        images.push_back(new GuiButton("cuts/timeline_audio_icon_1.png"));
    }
    if(_length == 5){
        images.push_back(new GuiButton("cuts/timeline_audio_icon_5.png"));
    }
    if(_length == 15){
        images.push_back(new GuiButton("cuts/timeline_audio_icon_15.png"));
    }
    if(_length == 30){
        images.push_back(new GuiButton("cuts/timeline_audio_icon_30.png"));
    }
    */
    images.push_back(new GuiTimelineButton(_length));
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

bool GuiTimeline::processMouse(int _x, int _y, int _state){
    if(_state == MOUSE_STATE_DOWN){
        list<GuiTimelineButton*>::iterator iIter;
        for(iIter = images.begin(); iIter != images.end(); ++iIter){
            if((*iIter)->isInside(_x, _y)){
                cout << "OPENING EFFECTS MENU" << endl;
                GuiConfigurator::Instance()->setGlobal("active-timeline", name);
                GuiConfigurator::Instance()->addLoosie((*iIter));
                (*iIter)->execute();
                (*iIter)->setAwaitingResponse(true);
                return true;
            }
        }
    }
    return false;
}

bool GuiTimeline::isInside(int _x, int _y){
    list<GuiTimelineButton*>::iterator iIter;
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

void GuiTimeline::execute(){
}

void GuiTimeline:: draw(){
    //ofSetColor(255,0,255);
    //ofRect(pos.x,pos.y,size.x, size.y);
    ofVec2f drawPos = pos;
    drawPos.y += 5;
    list<GuiTimelineButton*>::iterator iIter;
    for(iIter = images.begin(); iIter != images.end(); ++iIter){
        (*iIter)->setPosition(drawPos);
        drawPos.x += (*iIter)->getSize().x - 3;
        (*iIter)->draw();
    }
}
