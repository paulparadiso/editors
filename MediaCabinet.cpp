#include "MediaCabinet.h"
#include "XMLLoader.h"

MediaCabinet* MediaCabinet::mInstance = NULL;

MediaCabinet::MediaCabinet(){
}

MediaCabinet* MediaCabinet::Instance(){
    if(!mInstance){
        mInstance = new MediaCabinet();
    }
    return mInstance;
}

void MediaCabinet::addClip(string _name, string _path){
    cout << "Media Cabinet addClip called with name - " << _name << endl;
    XMLLoader loader(_path);
    string name = _path;
    string type = loader.getValue("type");
    string duration = loader.getValue("duration");
    string path = loader.getValue("path");
    if(type == "video"){
        cout << "Media Cabinet adding video - " << path << endl;
        //delete cabinet[name];
        cabinet[path] = new VideoClip(path);
    } else if(type == "audio"){
        cout << "Media cabinet adding audio - " << path << endl;
        //delete cabinet[name];
        cabinet[path] = new AudioClip(path, ofToFloat(duration));
    }
    clipHolds[path] = 0;
    cout << "MediaCabinet size = " << cabinet.size() << endl;
    lastItem = path;
}

Clip* MediaCabinet::getClip(string _name){
    Clip* clip = cabinet[_name];
    cout << "returning a clip named - " << clip->getName() << endl;
        //cout << "MediaCabinet return clip of length - " << clip->getDuration() << endl;
    return clip;
}

Clip* MediaCabinet::getLastClip(){
    return cabinet[lastItem];
}

void MediaCabinet::increaseClipHold(Clip* _clip){
    clipHolds[_clip->getName()] += 1;
    cout << "MediaCabinet increasing hold on " << _clip->getName() << " to " << clipHolds[_clip->getName()] << endl;
}

void MediaCabinet::decreaseClipHold(Clip* _clip){
    clipHolds[_clip->getName()] -= 1;
    cout << "MediaCabinet decreasing hold on " << _clip->getName() << " to " << clipHolds[_clip->getName()] << endl;
    if(clipHolds[_clip->getName()] < 1){
        //removeClip(_clip);
    }
}

void MediaCabinet::removeClip(Clip* _clip){
    map<string,Clip*>::iterator mIter;
    for(mIter = cabinet.begin(); mIter != cabinet.end(); ++mIter){
        if(mIter->second == _clip){
            //delete mIter->second;
            cabinet.erase(mIter);
            cout << "removing clip" << endl;
        }
    }
    cout << "MediaCabinet size = " << cabinet.size() << endl;
}

void MediaCabinet::reset(){
    map<string,Clip*>::iterator mIter;
    for(mIter = cabinet.begin(); mIter != cabinet.end(); ++mIter){
        delete mIter->second;
    }
    cabinet.clear();
}
