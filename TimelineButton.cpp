#include "TimelineButton.h"

TimelineButton::TimelineButton(float _length) : GuiButton(){
    totalWidth = 1400;
    curveRadius = 14;
    length = _length;
    if(length < 1.0){
        length = 1.0;
    }
    height = 69;
    width = totalWidth * length / 60;
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
    outerCurve.setStrokeColor(ofColor(255,255,255));
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
    dropShadow.draw();
    buttonPath.draw();
    innerCurve.draw();
    outerCurve.draw();
    img.draw(width / 2 - img.getWidth() / 2, height / 2 - img.getHeight() / 2);
}
