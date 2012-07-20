#include "Subject.h"
#include "SubObMediator.h"

void Subject::update(){
    SubObMediator::Instance()->update(handle, this);
}
