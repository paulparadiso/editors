#include "SubObMediator.h"

SubObMediator* SubObMediator::mInstance = NULL;

SubObMediator* SubObMediator::Instance(){
    if(!mInstance){
        mInstance = new SubObMediator();
    }
    return mInstance;
}

void SubObMediator::addObserver(string _subName, Observer* _obs){
    observers[_subName].push_back(_obs);
    //std::cout << "adding an observer to subject - " << _subName << std::endl;
}

void SubObMediator::update(string _subName, Subject* _sub){
    //cout << "updating observers of - " << _subName << endl;
    vector<Observer*>::iterator oIter;
    for(oIter = observers[_subName].begin(); oIter != observers[_subName].end(); ++oIter){
        (*oIter)->update(_subName, _sub);
    }
}
