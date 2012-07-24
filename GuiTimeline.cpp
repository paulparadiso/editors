#include "GuiTimeline.h"

GuiTimelineButton::GuiTimelineButton(float _length) : GuiButton(){
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

void GuiTimelineButton::update(){
}

void GuiTimelineButton::draw(){
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
    Compositor::Instance()->addTimeline(attrs["name"]);
    timeline = Compositor::Instance()->getTimeline(attrs["name"]);
}

void GuiTimeline::message(map<string,string> _msg){
    cout << name << " got new message" << endl;
    string action = _msg["action"];
    if(action == "add")
        addItem(_msg);
    if(action == "delete")
        deleteItem(_msg);
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
