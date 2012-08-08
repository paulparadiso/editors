#include "Pager.h"
#include "GuiConfigurator.h"
#include "MediaCabinet.h"
#include "XMLLoader.h"

Pager::Pager(map<string, string> &_attrs) : GuiNode(){
    attrs = _attrs;
    pageDims.x = ofToInt(attrs["columns"]);
    pageDims.y = ofToInt(attrs["rows"]);
    numItemsPerPage = pageDims.x * pageDims.y;
    //cout << "PAGER DIMS = " << pageDims.x << ", " << pageDims.y << "NUMITEMS = " << numItemsPerPage << endl;
    //itemDims.x = size.x / pageDims.x;
    //itemDims.y = size.y / pageDims.y;
    itemDims.x = 245;
    itemDims.y = 138;
    currentPage = 0;
    initialize();
    exit = new GuiButton("cuts/exit_big.png");
    exit->setPosition(ofVec2f(pos.x + 1320,pos.y - 200));
    exit->setAttr("action", "close");
    exit->setName("button");
    exit->setChannel("button");
    texter.loadFont("fonts/GOTHICB.TTF", 16);
}

void Pager::setPositions(){
    firstItem = currentPage * numItemsPerPage;
    lastItem = firstItem + numItemsPerPage;
    if(lastItem > items.size()){
        lastItem = items.size();
    }
    cout << "Pager items = " << firstItem << ", " << lastItem << endl;
    /*
    for(vector<PagerItem*>::size_type i  = firstItem; i < lastItem; i++){
        int x = i % (int)pageDims.x;
        int y = (i % numItemsPerPage) / (int)pageDims.x;
        //cout << "setting position of " << i << " to " << x << ", " << y << endl;
        items[i]->setPosition(x,y);
        items[i]->setDisplaySize(itemDims.x, itemDims.y);
        items[i]->setPagerPadding(pos.x, pos.y);
        items[i]->setItemPadding(25,42);
    }
    */
    for(int i = 0; i < items.size(); i++){
        int x = i % (int)pageDims.x;
        int y = (i % numItemsPerPage) / (int)pageDims.x;
        //cout << "setting position of " << i << " to " << x << ", " << y << endl;
        items[i]->setPosition(x,y);
        items[i]->setDisplaySize(itemDims.x, itemDims.y);
        items[i]->setPagerPadding(pos.x, pos.y);
        items[i]->setItemPadding(25,42);
    }
    //cout << "positions set" << endl;
}

bool Pager::processMouse(int _x, int _y, int _state){
    if(_state == MOUSE_STATE_DOWN){
		/*
		for(vector<PagerItem*>::size_type i  = firstItem; i < lastItem; i++){
			if(items[i]->isInside(_x,_y)){
				items[i]->execute();
				return true;
			}
		}
		*/
		for(int i = 0; i < items.size(); i++){
            if(items[i]->isInside(_x,_y)){
				items[i]->execute();
				return true;
			}
		}
		if(exit->isInside(_x, _y)){
			exit->execute();
			return true;
		}
	}
	return false;
}

bool Pager::isInside(int _x, int _y){
    /*
    for(vector<PagerItem*>::size_type i  = firstItem; i < lastItem; i++){
        if(items[i]->isInside(_x,_y)){
            items[i]->execute();
        }
    }
    */
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

void Pager::draw(){
    /*
    for(vector<PagerItem*>::size_type i  = firstItem; i < lastItem; i++){
        items[i]->draw();
    }
    */
    for(int i = 0; i < items.size(); i++){
        items[i]->draw();
    }
    SceneManager::Instance()->drawTimeRemaining(activeTimeline);
    //exit->draw();
    //texter.drawString("time remaining on track :60", pos.x + 15, pos.y - 80);
}

void Pager::message(map<string,string> _msg){
    cout << "user-pager recieved message" << endl;
    if(_msg["action"] == "reload"){
        reload();
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

void PagerItem::update(string _subName, Subject *_sub){
    if(_subName == "time-remaining"){
        string target = _sub->getAttr("name");
        //cout << "PagerItem " << attrs["target"] << " got updated time remaining for " << target << endl;
        if(target == attrs["target"]){
            timeRemainingOnTrack = ofToInt(_sub->getAttr("time"));
        }
    }
}

VideoPager::VideoPager(map<string, string> & _attrs) : Pager(_attrs){
    dir = _attrs["directory"];
    activeTimeline = _attrs["target"];
    cout << "set activeTimeline to " << _attrs["target"] << endl;
    lister.allowExt("xml");
    lister.listDir(dir);
    string folder = dir + "/";
    for(int i = 0; i < lister.size(); i++){
        //xml.loadFile(lister.getPath(i));
        //string type = xml.getValue("type","none");
        //items.push_back(new VideoItem(xml.getValue("preview","none"), type));

        //items.push_back(new VideoItem(lister.getPath(i)));
        //items.back()->setAttr("target", activeTimeline);

        PagerItem *tmpItemPtr = new VideoItem(lister.getPath(i));
        tmpItemPtr->setAttr("target", activeTimeline);
        if(tmpItemPtr->getItemType() == "audio"){
            int index = ofToInt(tmpItemPtr->getItemIndex());
            //items[index] = tmpItemPtr;
            items[index] = tmpItemPtr;
        } else {
            //items.push_back(tmpItemPtr);
            items[i] = tmpItemPtr;
        }
    }
    setPositions();
}

void VideoPager::reload(){
    cout << "user-pager reloading" << endl;
    items.clear();
    lister.allowExt("xml");
    lister.listDir(dir);
    string folder = dir + "/";
    for(int i = 0; i < lister.size(); i++){
        //xml.loadFile(lister.getPath(i));
        //string type = xml.getValue("type","none");
        //items.push_back(new VideoItem(xml.getValue("preview","none"), type));
        items[i] = new VideoItem(lister.getPath(i));
    }
    setPositions();
}

VideoItem::VideoItem(string _path){
    string name = _path;
    XMLLoader loader(name);
    setAttr("path",_path);
    setAttr("name",name);
    setAttr("type",loader.getValue("type"));
    itemType = attrs["type"];
    duration = loader.getValue("duration");
    setAttr("preview", loader.getValue("preview"));
    //cout << "making a new item. " << name << ", " << type << ", " << path << endl;
    if(attrs["type"]== "video")
        setupVideo();
    else if(attrs["type"] == "image")
        setupImage();
    else if(attrs["type"] == "audio"){
        //setAttr("index",loader.getValue("position"));
        cout << "setting itemType to audio" << endl;
        itemType = "audio";
        itemIndex = loader.getValue("position");
        setupAudio();
    }
    texter.loadFont("fonts/GOTHICB.TTF",16);
    SubObMediator::Instance()->addObserver("time-remaining", this);
    timeRemainingOnTrack = 60;
}

void VideoItem::setupVideo(){
    preview.loadImage(attrs["preview"]);
    mediaPreview = new GuiMediaPreview(attrs);
    mediaPreview->setPosition(ofVec2f(407,278));
    msg["action"] = "add";
    msg["target"] = "video-timeline";
    msg["path"] = attrs["path"];
    msg["type"] = attrs["type"];
    msg["name"] = name;
    msg["duration"] = duration;
    durationInt = ofToInt(duration);
    if(dur < 10){
        drawDuration = "0" + ofToString(dur);
    } else {
        drawDuration = ofToString(dur);
    }
    mediaPreview->setSelectMessage(msg);
    previewSheet = new GuiSheet();
    previewSheet->addNode(mediaPreview);
    GuiConfigurator::Instance()->addSheet(attrs["path"],previewSheet);
    frame.loadImage("cuts/wide_frame.png");
    size.x = frame.getWidth();
    size.y = frame.getHeight();
}

void VideoItem::setupImage(){
    preview.loadImage(attrs["preview"]);
    mediaPreview = new GuiMediaPreview(attrs);
    mediaPreview->setPosition(ofVec2f(-80,-100));
    msg["action"] = "add";
    msg["target"] = "video-timeline";
    msg["path"] = attrs["path"];
    msg["type"] = attrs["type"];
    msg["duration"] = "5";
    mediaPreview->setSelectMessage(msg);
    previewSheet = new GuiSheet();
    previewSheet->addNode(mediaPreview);
    GuiConfigurator::Instance()->addSheet(attrs["path"],previewSheet);
    frame.loadImage("cuts/menu_frame_time.png");
    size.x = frame.getWidth();
    size.y = frame.getHeight();
}

void VideoItem::setupAudio(){
    mediaPreview = new GuiMediaPreview(attrs);
    mediaPreview->setPosition(ofVec2f(-80,-80));
    msg["action"] = "add";
    msg["path"] = attrs["path"];
    msg["type"] = attrs["type"];

    msg["duration"] = duration;
    int dur = ofToInt(duration);
    durationInt = ofToInt(duration);
    if(dur < 10){
        drawDuration = "0" + ofToString(dur);
    } else {
        drawDuration = ofToString(dur);
    }
    mediaPreview->setSelectMessage(msg);
    previewSheet = new GuiSheet();
    previewSheet->addNode(mediaPreview);
    GuiConfigurator::Instance()->addSheet(attrs["path"],previewSheet);
    frame.loadImage("cuts/audio_frame.png");
    size.x = frame.getWidth();
    size.y = frame.getHeight();
}

void VideoItem::execute(){
    if(attrs["type"] == "video"){
        executeVideo();
    } else if(attrs["type"] == "image"){
        executeImage();
    } else if(attrs["type"] == "audio"){
        executeAudio();
    }
}

void VideoItem::executeVideo(){
    if(durationInt < timeRemainingOnTrack){
        MediaCabinet::Instance()->addClip(attrs["path"],attrs["path"]);
        GuiConfigurator::Instance()->openSheet(attrs["path"]);
    }
}

void VideoItem::executeImage(){
    GuiConfigurator::Instance()->openSheet(attrs["path"]);
}

void VideoItem::executeAudio(){
    /*
    msg["target"] = GuiConfigurator::Instance()->getGlobal("timeline-target");
    button->setMessage(msg);
    button->execute();
    */
    msg["target"] = GuiConfigurator::Instance()->getGlobal("timeline-target");
    mediaPreview->setSelectMessage(msg);
    MediaCabinet::Instance()->addClip(attrs["path"],attrs["path"]);
    GuiConfigurator::Instance()->openSheet(attrs["path"]);
}

void VideoItem::draw(){
    if(attrs["type"] == "video")
        drawVideo();
    else if(attrs["type"] == "image")
        drawImage();
    else if(attrs["type"] == "audio")
        drawAudio();
    //preview.draw(basePos.x, basePos.y, 215, 121);

}

void VideoItem::drawVideo(){
    pos = (basePos * ofVec2f(frame.getWidth(),frame.getHeight())) + pagerPadding + (itemPadding * basePos);
    preview.draw(pos.x + 5, pos.y + 5, displaySize.x, displaySize.y);
    frame.draw(pos.x, pos.y);
    //texter.drawString(":" + drawDuration, pos.x + 193, pos.y + 125);
    SceneManager::Instance()->drawNumber(durationInt, pos.x + 193, pos.y + 105, 41, 25,0);
}

void VideoItem::drawImage(){
    pos = (basePos * ofVec2f(frame.getWidth(),frame.getHeight())) + pagerPadding + (itemPadding * basePos);
    preview.draw(pos.x, pos.y + 20, displaySize.x, displaySize.y);
    frame.draw(pos.x, pos.y);
    texter.drawString(":05", pos.x + 220, pos.y + 220);
}

void VideoItem::drawAudio(){
    pos = (basePos * ofVec2f(size.x,size.y)) + pagerPadding + (itemPadding * basePos);
    frame.draw(pos.x,pos.y);
    texter.drawString(":" + drawDuration, pos.x + 193, pos.y + 125);
}

/*
*
*Utilities used by VideoItem
*
*/

ofTexture getVideoPreview(string _path, float _pos){
    cout << "getting preview for video - " << _path << endl;
    ofVideoPlayer player;
    ofTexture preview;
    player.loadMovie(_path);
    preview.allocate(player.getWidth(), player.getHeight(), GL_RGB);
    player.setPosition(_pos);
    preview.loadData(player.getPixels(), player.getWidth(), player.getHeight(), GL_RGB);
    player.close();
    return preview;
}
