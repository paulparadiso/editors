#include "SubObMediator.h"

SubObMediator* SubObMediator::mInstance = NULL;

SubObMediator* SubObMediator::Instance(){
    if(!mInstance){
        mInstance = new SubObMediator();
    }
    return mInstance;
}

SubObMediator::SubObMediator(){
    bHaveNewObservers = false;
}

/*
Only add observer to new observer vector for now.
*/

void SubObMediator::addObserver(string _subName, Observer* _obs){
    //observers[_subName].push_back(_obs);
    //std::cout << "adding an observer to subject - " << _subName << std::endl;
    newObservers.push_back(_obs);
    newObserverChannels.push_back(_subName);
    bHaveNewObservers = true;
}

void SubObMediator::removeObserver(Observer* _obs){
    map<string,vector<Observer*> >::iterator mIter;
    vector<Observer*>::iterator oIter;
    for(mIter = observers.begin(); mIter != observers.end(); ++mIter){
        for(oIter = (*mIter).second.begin(); oIter != (*mIter).second.end();){
            if((*oIter) == _obs){
                oIter = (*mIter).second.erase(oIter);
                return;
            } else {
                ++oIter;
            }
        }
    }
}

void SubObMediator::update(string _subName, Subject* _sub){
    //cout << "updating observers of - " << _subName << endl;
    if(bHaveNewObservers){
        if(newObservers.size() == newObserverChannels.size()){
            for(int i = 0; i < newObservers.size(); i++){
                observers[newObserverChannels[i]].push_back(newObservers[i]);
            }
            newObserverChannels.clear();
            newObservers.clear();
        }
        bHaveNewObservers = false;
    }
    vector<Observer*>::iterator oIter;
    for(oIter = observers[_subName].begin(); oIter != observers[_subName].end(); ++oIter){
        (*oIter)->update(_subName, _sub);
    }
    for(oIter = observers["all"].begin(); oIter != observers["all"].end(); ++oIter){
        (*oIter)->update(_subName, _sub);
    }
}
