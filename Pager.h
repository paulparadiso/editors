#ifndef PAGER_H_INCLUDED
#define PAGER_H_INCLUDED

#include "ofMain.h"
#include "GuiCreator.h"
#include "ofxXmlSettings.h"

ofTexture getVideoPreview(string _path, float _pos);

class MediaPreview;

class PagerItem : public GuiNode{

public:
    PagerItem() : GuiNode(){}
    void setPos(ofVec2f _pos){pos = _pos;}
    void setSize(ofVec2f _size){size = _size;}
    virtual void draw(){}
    bool processMouse(int _x, int _y, int _state);
    virtual void execute(){}
    void setPosition(int _x, int _y);
    void setDisplaySize(int _x, int _y);
    void setPagerPadding(int _x, int _y);
    void setItemPadding(int _x, int _y);

protected:
    ofVec2f basePos;
    ofVec2f displaySize;
    ofVec2f itemPadding;
    ofVec2f pagerPadding;
};

/*

class VideoItem : public PagerItem{

public:
    VideoItem(string _dir);
    void setupVideo();
    void setupImage();
    void setupAudio();
    void draw();
    void drawVideo();
    void drawImage();
    void drawAudio();
    void execute();
    void executeVideo();
    void executeImage();
    void executeAudio();

private:
    ofImage preview;
    ofImage frame;
    string duration;
    string drawDuration;
    //string path;
    //string type;
    //string target;
    MediaPreview *mediaPreview;
    GuiSheet *previewSheet;
    ofTrueTypeFont texter;
    map<string,string> msg;
    GuiButton *button;
};

*/

class Pager : public GuiNode {

public:
    Pager(map<string,string> &_attrs);
    //virtual ~Pager(){}
    bool processMouse(int _x, int _y, int _state);
    virtual void execute(){}
    virtual void reload(){}
    void draw();
    void nextPage();
    void previousPage();
    void setPositions();
    void setAllAttr(string _attr, string _val);

protected:
    vector<PagerItem*>items;
    void message(map<string,string> _msg);
    int currentPage;
    int numItemsPerPage;
    int firstItem;
    int lastItem;
    ofVec2f pageDims;
    ofVec2f pageSize;
    ofVec2f itemDims;
    bool isInside(int _x, int _y);
    GuiButton *exit;
    bool drawPrevious;
    bool drawNext;
    ofTrueTypeFont texter;
};

/*
class VideoPager : public Pager {
public:
    VideoPager(map<string, string> & _attrs);
    virtual void reload();
private:
    string dir;
    ofDirectory lister;
    ofxXmlSettings xml;
};
*/

#endif // PAGER_H_INCLUDED
