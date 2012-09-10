#ifndef PAGER_H_INCLUDED
#define PAGER_H_INCLUDED

#include "ofMain.h"
#include "GuiCreator.h"
#include "ofxXmlSettings.h"
#include "GuiMediaPreview.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"

ofTexture getVideoPreview(string _path, float _pos);

class PagerItem : public GuiNode, public Subject, public Observer{

public:
    PagerItem() : GuiNode(){}
    PagerItem(string _dir);
    void setPos(ofVec2f _pos){pos = _pos;}
    void setSize(ofVec2f _size){size = _size;}
    void makePreview();
    void draw();
    bool processMouse(int _x, int _y, int _state);
    void execute();
    void setPosition(int _x, int _y);
    void setDisplaySize(int _x, int _y);
    void setPagerPadding(int _x, int _y);
    void setItemPadding(int _x, int _y);
    string getItemIndex(){return itemIndex;}
    string getItemType(){return itemType;}
    void setReloader(){bReloader = true;}
    string getAttr(const char* _key){string ret = _key; return attrs[ret];}

protected:
    ofVec2f basePos;
    ofVec2f displaySize;
    ofVec2f itemPadding;
    ofVec2f pagerPadding;
    string itemType;
    string itemIndex;
    ofImage preview;
    string duration;
    string drawDuration;
    int durationInt;
    GuiMediaPreview *mediaPreview;
    GuiSheet *previewSheet;
    ofTrueTypeFont texter;
    bool bReloader;
};

class Pager : public GuiNode, public Observer {

public:
    Pager(map<string,string> &_attrs);
    //virtual ~Pager(){}
    bool processMouse(int _x, int _y, int _state);
    void reload();
    void draw();
    void execute(){}
    void nextPage();
    void previousPage();
    void setPositions();
    void setAllAttr(string _attr, string _val);
    void update(string _subName, Subject *_sub);

protected:
    map<int,PagerItem*>items;
    void populate(string _dir);
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
    string activeTimeline;
    bool bReloader;
    string dir;
    ofDirectory lister;
    ofxXmlSettings xml;
};

#endif // PAGER_H_INCLUDED
