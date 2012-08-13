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
    void setPos(ofVec2f _pos){pos = _pos;}
    void setSize(ofVec2f _size){size = _size;}
    virtual void draw(){}
    virtual void drawFramed(){}
    bool processMouse(int _x, int _y, int _state);
    virtual void execute(){}
    void setPosition(int _x, int _y);
    void setDisplaySize(int _x, int _y);
    void setPagerPadding(int _x, int _y);
    void setItemPadding(int _x, int _y);
    void update(string _subName, Subject *_sub);
    string getItemIndex(){return itemIndex;}
    string getItemType(){return itemType;}

protected:
    ofVec2f basePos;
    ofVec2f displaySize;
    ofVec2f itemPadding;
    ofVec2f pagerPadding;
    int timeRemainingOnTrack;
    string itemType;
    string itemIndex;
    bool haveArabic;
    bool displayArabic;
    //GuiNumberRenderer *numberRenderer;
};

class VideoItem : public PagerItem{

public:
    VideoItem(string _dir);
    void setupVideo();
    void setupImage();
    void setupAudio();
    void draw();
    void drawFramed();
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
    ofImage arabicFrame;
    string duration;
    string drawDuration;
    int durationInt;
    //string path;
    //string type;
    //string target;
    GuiMediaPreview *mediaPreview;
    GuiSheet *previewSheet;
    ofTrueTypeFont texter;
    map<string,string> msg;
    GuiButton *button;
};

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
    map<int,PagerItem*>items;
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
    string activeTimeline;
    bool bReloader;
};

class VideoPager : public Pager, public Observer {
public:
    VideoPager(map<string, string> & _attrs);
    virtual void reload();
    void update(string _subName, Subject *_sub);
private:
    string dir;
    ofDirectory lister;
    ofxXmlSettings xml;
};

#endif // PAGER_H_INCLUDED
