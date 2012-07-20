#ifndef SUBOBMEDIATOR_H
#define SUBOBMEDIATOR_H

#include <string>
#include <vector>
#include <map>
#include "Observer.h"
#include <iostream>

using namespace std;

class SubObMediator
{
    public:
        virtual ~SubObMediator(){}
        static SubObMediator* Instance();
        void update(string _subName, Subject* _sub);
        void addObserver(string _subName, Observer* _obs);
    protected:
    private:
        SubObMediator(){}
        map<string,vector<Observer*> > observers;
        static SubObMediator* mInstance;
};

#endif // SUBOBMEDIATOR_H
