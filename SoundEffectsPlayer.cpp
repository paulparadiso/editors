#include "SoundEffectsPlayer.h"

SoundEffectsPlayer::SoundEffectsPlayer()
{
    lister.allowExt("mp3");
    lister.listDir("sfx");
    for(int i = 0; i < lister.size(); i++){
        vector<string> tokens = ofSplitString(lister.getName(i), ".");
        players[tokens[0]].loadSound(lister.getPath(i));
    }
    SubObMediator::Instance()->addObserver("all", this);
}

void SoundEffectsPlayer::update(string _subName, Subject* _sub){
    string sfx = _sub->getAttr("sfx");
    map<string,ofSoundPlayer>::iterator mIter;
    mIter = players.find(sfx);
    if(mIter != players.end()){
        players[sfx].play();
    }
}

SoundEffectsPlayer::~SoundEffectsPlayer()
{
    //dtor
}
