#include "Observer.h"
#include "SubObMediator.h"

Observer::Observer()
{
    //ctor
}

Observer::~Observer()
{
    SubObMediator::Instance()->removeObserver(this);
}

void Observer::update(string _subName, Subject* _sub){
    cout << "observer updated to - " << _sub->getIntVal() << ", " << _sub->getFloatVal() << ", " << _sub->getBoolVal() << ", " << _sub->getStringVal() << endl;
}
