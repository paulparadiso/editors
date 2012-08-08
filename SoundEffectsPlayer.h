#ifndef SOUNDEFFECTSPLAYER_H
#define SOUNDEFFECTSPLAYER_H

#include "ofMain.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"

class SoundEffectsPlayer : public Observer
{
    public:
        SoundEffectsPlayer();
        virtual ~SoundEffectsPlayer();
        void update(string _subName, Subject* _sub);
    protected:
    private:
        map<string,ofSoundPlayer>players;
        ofDirectory lister;
};

#endif // SOUNDEFFECTSPLAYER_H
