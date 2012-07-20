#include "Observer.h"

Observer::Observer()
{
    //ctor
}

Observer::~Observer()
{
    //dtor
}

void Observer::update(Subject* _sub){
    cout << "observer updated to - " << _sub->getIntVal() << ", " << _sub->getFloatVal() << ", " << _sub->getBoolVal() << ", " << _sub->getStringVal() << endl;
}
