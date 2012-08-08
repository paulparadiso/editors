#ifndef TIMER_H
#define TIMER_H

#include "ofMain.h"
#include "Subject.h"
#include "SubObMediator.h"

class Timer : public ofThread, public Subject
{
    public:
        Timer(){}
        Timer(float _interval);
        virtual ~Timer();
        void start();
        void stop();
        void setInterval(float _interval);
        void threadedFunction();
    protected:
    private:
        float interval;
        float timeOfLastClick;
        string channel;
};

#endif // TIMER_H
