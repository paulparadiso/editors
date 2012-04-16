#ifndef TRANSITION_H_INCLUDED
#define TRANSITION_H_INCLUDED

#include "ofMain.h"

enum{LINEAR, RAMP, LOG};

class Animator {

public:
    Animator(){}
    virtual void setup(float _start, float _stop, float _length) = 0;
    virtual float getCurrentValue(float _percent) = 0;

protected:
    float startVal, stopVal, length;

};

class LinearAnimator : public Animator {

public:
    LinearAnimator() : Animator() {}
    void setup(float _start, float _stop, float _length);
    float getCurrentValue(float _percent);

private:
    float distance;
};

class Transition{

public:
    Transition(){}
    Transition(string _mode);
    void start(float _startVal, float _stopVal, int _length);
    float getVal();

private:
    float startVal, stopVal, currentVal;
    int direction;
    int startTime, length;
    Animator* animator;

};

#endif // TRANSITION_H_INCLUDED
