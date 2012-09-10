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
}
