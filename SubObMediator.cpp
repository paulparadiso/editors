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

void SubObMediator::removeObserver(Observer* _obs){
    map<string,vector<Observer*> >::iterator mIter;
    vector<Observer*>::iterator oIter;
    for(mIter = observers.begin(); mIter != observers.end(); ++mIter){
        for(oIter = (*mIter).second.begin(); oIter != (*mIter).second.end();){
            if((*oIter) == _obs){
                oIter = (*mIter).second.erase(oIter);
                cout << "removing observer." << endl;
                return;
            } else {
                ++oIter;
            }
        }
    }
}

void SubObMediator::update(string _subName, Subject* _sub){
    //cout << "updating observers of - " << _subName << endl;
    vector<Observer*>::iterator oIter;
    for(oIter = observers[_subName].begin(); oIter != observers[_subName].end(); ++oIter){
        (*oIter)->update(_subName, _sub);
    }
    for(oIter = observers["all"].begin(); oIter != observers["all"].end(); ++oIter){
        (*oIter)->update(_subName, _sub);
    }
}
