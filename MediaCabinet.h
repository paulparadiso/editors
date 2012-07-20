#ifndef MEDIACABINET_H_INCLUDED
#define MEDIACABINET_H_INCLUDED

#include "ofMain.h"
#include "Clip.h"
#include "XMLLoader.h"

class MediaCabinet{

public:
    static MediaCabinet* Instance();
    void addClip(string _name, string _file);
    Clip* getLastClip();
    Clip* getClip(string _name);
    void removeClip(Clip* _clip);
    void increaseClipHold(Clip* _clip);
    void decreaseClipHold(Clip* _clip);
    void reset();

private:
    MediaCabinet(){}
    static MediaCabinet* mInstance;
    map<string, Clip*> cabinet;
    map<string, int> clipHolds;
    string lastItem;
};

#endif // MEDIACABINET_H_INCLUDED
