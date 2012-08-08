#include "Timer.h"

Timer::Timer(float _interval){
    interval = 1.0 / _interval;
    timeOfLastClick = ofGetElapsedTimef();
    channel = "timer-tick";
}

Timer::~Timer()
{
    //dtor
}

void Timer::start(){
    startThread(true, false);
}

void Timer::stop(){
    stopThread();
}

void Timer::setInterval(float _interval){
    interval = 1.0 / _interval;
}

void Timer::threadedFunction(){
    while(isThreadRunning() != 0){
        float now = ofGetElapsedTimef();
        if(now - timeOfLastClick > interval){
            timeOfLastClick = now;
            SubObMediator::Instance()->update(channel, this);
        }
    }
}
