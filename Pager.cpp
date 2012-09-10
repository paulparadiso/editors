#include "Pager.h"
#include "GuiConfigurator.h"
#include "MediaCabinet.h"
#include "XMLLoader.h"

Pager::Pager(map<string, string> &_attrs) : GuiNode(){
    attrs = _attrs;
    pageDims.x = ofToInt(attrs["columns"]);
    pageDims.y = ofToInt(attrs["rows"]);
    numItemsPerPage = pageDims.x * pageDims.y;
    itemDims.x = 245;
    itemDims.y = 138;
    currentPage = 0;
    initialize();
    exit = new GuiButton("cuts/exit_big.png");
    exit->setPosition(ofVec2f(pos.x + 1320,pos.y - 200));
    exit->setAttr("action", "close");
    exit->setAttr("sfx","button11");
    dir = _attrs["directory"];
    if(dir == "audio/user"){
        bReloader = true;
    } else {
        bReloader = false;
    }
    populate(dir);
    setPositions();
    SubObMediator::Instance()->addObserver("audio-recording-complete", this);
    SubObMediator::Instance()->addObserver("button", this);
}

void Pager::populate(string _dir){
    lister.allowExt("xml");
    lister.listDir(dir);
    string folder = dir + "/";
    for(int i = 0; i < lister.size(); i++){
        PagerItem *tmpItemPtr = new PagerItem(lister.getPath(i));
        tmpItemPtr->setAttr("target", activeTimeline);
        if(bReloader){
            tmpItemPtr->setReloader();
        }
        if(!bReloader && tmpItemPtr->getItemType() == "audio"){
            int index = ofToInt(tmpItemPtr->getItemIndex());
            items[index] = tmpItemPtr;
        } else {
            items[i] = tmpItemPtr;
        }
    }
}

void Pager::setPositions(){
    firstItem = currentPage * numItemsPerPage;
    lastItem = firstItem + numItemsPerPage;
    if(lastItem > items.size()){
        lastItem = items.size();
    }
    for(int i = 0; i < items.size(); i++){
        int x = i % (int)pageDims.x;
        int y = (i % numItemsPerPage) / (int)pageDims.x;
        items[i]->setPosition(x,y);
        items[i]->setDisplaySize(itemDims.x, itemDims.y);
        items[i]->setPagerPadding(pos.x - 10, pos.y - 10);
        items[i]->setItemPadding(25,53);
    }
}

bool Pager::processMouse(int _x, int _y, int _state){
    if(_state == MOUSE_STATE_DOWN){
		for(int i = 0; i < items.size(); i++){
            if(items[i]->isInside(_x,_y)){
				items[i]->execute();
				return true;
			}
		}
		if(exit->isInside(_x, _y)){
			cout << "inside exit" << endl;
			exit->execute();
			return true;
		}
	}
	return false;
}

bool Pager::isInside(int _x, int _y){
    for(int i = 0; i < items.size(); i++){
        if(items[i]->isInside(_x,_y)){
            items[i]->execute();
        }
    }
    if(exit->isInside(_x,_y)){
        exit->execute();
    }
}

void Pager::setAllAttr(string _attr, string _val){
   for(vector<PagerItem*>::size_type i  = firstItem; i < lastItem; i++){
        items[i]->setAttr(_attr,_val);
    }
}
void Pager::reload(){
    items.clear();
    lister.allowExt("xml");
    lister.listDir(dir);
    string folder = dir + "/";
    int counterStart;
    if(lister.size() > 15){
        counterStart = lister.size() - 15;
    } else {
        counterStart = 0;
    }
    for(int i = counterStart; i < lister.size(); i++){
        items[i] = new PagerItem(lister.getPath(i));
        items[i]->setReloader();
    }
    setPositions();
}

void Pager::draw(){
    for(int i = 0; i < items.size(); i++){
        items[i]->draw();
    }
    SceneManager::Instance()->drawTimeRemaining();
    /*
    if(bReloader && (items.size() == 0)){
        SceneManager::Instance()->drawNoCustom();
    }
    */
    if(items.size() == 0){
        SceneManager::Instance()->drawNoCustom();
    }
}

void Pager::update(string _subName, Subject *_sub){
    if(_subName == "audio-recording-complete"){
        if(bReloader){
            reload();
        }
    }
}

void PagerItem::setPosition(int _x, int _y){
    basePos.x = _x;
    basePos.y = _y;
}

void PagerItem::setDisplaySize(int _x, int _y){
    displaySize.x = _x;
    displaySize.y = _y;
}

void PagerItem::setPagerPadding(int _x, int _y){
    pagerPadding.x = _x;
    pagerPadding.y = _y;
}

void PagerItem::setItemPadding(int _x, int _y){
    itemPadding.x = _x;
    itemPadding.y = _y;
}

bool PagerItem::processMouse(int _x, int _y, int _state){
	return false;
}

PagerItem::PagerItem(string _path){
    string name = _path;
    XMLLoader loader(name);
    setAttr("path",_path);
    setAttr("name",name);
    setAttr("type",loader.getValue("type"));
    setAttr("sfx","button11");
    itemType = attrs["type"];
    duration = loader.getValue("duration");
    setAttr("preview", loader.getValue("preview"));
    if(itemType == "audio"){
        itemType = "audio";
        itemIndex = loader.getValue("position");
    }
    makePreview();
    SubObMediator::Instance()->addObserver("time-remaining", this);
    bReloader = false;
}

void PagerItem::makePreview(){
    if(itemType != "audio"){
        preview.loadImage(attrs["preview"]);
    }
    mediaPreview = new GuiMediaPreview(attrs);
    if(itemType == "audio"){
        mediaPreview->setPosition(ofVec2f(-80,-80));
    } else {
        mediaPreview->setPosition(ofVec2f(407,278));
    }
    durationInt = ofToInt(duration);
    previewSheet = new GuiSheet();
    previewSheet->addNode(mediaPreview);
    GuiConfigurator::Instance()->addSheet(attrs["path"],previewSheet);
    size.x = 253;
    size.y = 148;
}

void PagerItem::execute(){
    int timeRemaining = SceneManager::Instance()->getTimeRemainingOnTrack();
    cout << "Item clicked.  Have " << timeRemaining << " remaining on track." << endl;
    if(durationInt <= timeRemaining){
        MediaCabinet::Instance()->addClip(attrs["path"],attrs["path"]);
        GuiConfigurator::Instance()->openSheet(attrs["path"]);
        SubObMediator::Instance()->update("pager-item", this);
    }
}

void PagerItem::draw(){
    pos = (basePos * ofVec2f(size.x,size.y)) + pagerPadding + (itemPadding * basePos);
    if(itemType != "audio"){
        preview.draw(pos.x + 5, pos.y + 5, displaySize.x, displaySize.y);
        SceneManager::Instance()->drawFrame(pos.x, pos.y);
    }
    if(bReloader){
        SceneManager::Instance()->drawFrame(pos.x, pos.y);
    }
    SceneManager::Instance()->drawNumber(durationInt, pos.x + 193, pos.y + 105, 41, 25,0);
}

/*
*
*Utilities used by VideoItem
*
*/

ofTexture getVideoPreview(string _path, float _pos){
    //cout << "getting preview for video - " << _path << endl;
    ofVideoPlayer player;
    ofTexture preview;
    player.loadMovie(_path);
    preview.allocate(player.getWidth(), player.getHeight(), GL_RGB);
    player.setPosition(_pos);
    preview.loadData(player.getPixels(), player.getWidth(), player.getHeight(), GL_RGB);
    player.close();
    return preview;
}
