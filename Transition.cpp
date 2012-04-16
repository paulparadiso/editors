#include "Transition.h"

/*
Transition constructor takes an argument which set the
mode and animator for the transition.
*/

Transition::Transition(string _mode){
    if(_mode == "linear")
        animator = new LinearAnimator();
    else
        animator = new LinearAnimator();
}

void Transition::start(float _startVal, float _stopVal, int _length){
    startVal = _startVal;
    stopVal = _stopVal;
    length = _length;
    animator->setup(startVal, stopVal, length);
    startTime = ofGetElapsedTimeMillis();
    if(stopVal > startVal)
        direction = 1;
    else
        direction = -1;
}

float Transition::getVal(){
    if((currentVal * direction) < (stopVal * direction))
        currentVal = animator->getCurrentValue(1.0f);
    return currentVal;
}

void LinearAnimator::setup(float _startVal, float _stopVal, float _length){
    startVal = _startVal;
    stopVal = _stopVal;
    length = _length;
    distance = stopVal - startVal;
}

float LinearAnimator::getCurrentValue(float _percent){
    return startVal + (distance * _percent);
}

