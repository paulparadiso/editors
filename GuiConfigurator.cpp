#include "GuiConfigurator.h"

GuiConfigurator* GuiConfigurator::mInstance = NULL;

GuiConfigurator::GuiConfigurator(){
    bFirstStart = true;
    SubObMediator::Instance()->addObserver("button", this);
    SubObMediator::Instance()->addObserver("clip-selected", this);
}

GuiConfigurator* GuiConfigurator::Instance(){
    if(!mInstance){
        mInstance = new GuiConfigurator();
    }
    return mInstance;
}

void GuiConfigurator::addFile(string _file){
    mXML.loadFile(_file);
}

void GuiConfigurator::update(string _subName, Subject* _sub){
    if(_subName == "button"){
        string target = _sub->getAttr("target");
        string action = _sub->getAttr("action");
        string name = _sub->getAttr("name");
        cout << "got a GUI update from " << name << " for target " << target << " with action " << action << endl;
        if(action == "open"){
            openSheet(target);
        }
        if(action == "close"){
            closeSheet(target);
        }
        if(action == "replace"){
            replaceSheet(target);
        }
        if(action == "add"){
            closeSheet(target);
        }
        if(action == "reset"){
            SubObMediator::Instance()->update("reset", this);
            bFirstStart = true;
        }
    }
    if(_subName == "clip-selected"){
        closeSheet("none");
        closeSheet("none");
    }
}

void GuiConfigurator::getTags(){
    mXML.pushTag("gui");
    int numSheets = mXML.getNumTags("sheet");
    cout << "have " << numSheets << " sheets." << endl;
    for(int i = 0; i < numSheets; i++){
        string sheetName = mXML.getAttribute("sheet","name","none",i);
        string sheetType = mXML.getAttribute("sheet","type","none",i);
        sheetTypes[sheetName] = sheetType;
        cout << "starting sheet - " << sheetName << endl;
        mXML.pushTag("sheet", i);
        int numNodes = mXML.getNumTags("node");
        cout << "have " << numNodes << " nodes." << endl;
        for(int j = 0; j < numNodes; j++){
            vector<string> attrs, vals;
            mXML.getAttributeNames("node",attrs, j);
            for(int k = 0; k < attrs.size(); k++){
                vals.push_back(mXML.getAttribute("node",attrs[k], "none", j));
            }
            mXML.pushTag("node",j);
            string pos = mXML.getValue("pos", "none");
            string size = mXML.getValue("size", "none");
            attrs.push_back("pos");
            vals.push_back(pos);
            attrs.push_back("size");
            vals.push_back(size);
            mXML.popTag();
            addAttributeMap(sheetName, attrs, vals);
        }
        mXML.popTag();
    }
    mXML.popTag();
}

void GuiConfigurator::addAttributeMap(string _sheet, vector<string> &_names, vector<string> &_values){
    //cout << "adding map to - " << _sheet << endl;
    map<string,string> tmpAttr;
    vector<string>::iterator namesIter, valsIter;
    for(namesIter = _names.begin(), valsIter = _values.begin(); namesIter < _names.end() && valsIter < _values.end(); namesIter++, valsIter++){
        //cout << "--->adding " << (*namesIter) << ", " << (*valsIter) << " to - " << _sheet << endl;
        tmpAttr[(*namesIter)] = (*valsIter);
    }
    attrs[_sheet].push_back(tmpAttr);
}

void GuiConfigurator::makeGUI(){
    map<string, vector<map<string,string> > >::iterator sIter;
    vector<map<string, string> >::iterator gIter;
    for(sIter = attrs.begin(); sIter != attrs.end(); sIter++){
        string tmpName = (*sIter).first;
        //GuiSheet* tmpSheetPtr = new GuiSheet()
        //tmpGuiSheet->setName(tmpName);
        sheets[tmpName] = new GuiSheet();
        sheets[tmpName]->setName(tmpName);
        sheets[tmpName]->setType(sheetTypes[tmpName]);
        //vector<map<string,string> > tmpAttrs = (*sIter).second;
        for(gIter = (*sIter).second.begin(); gIter != (*sIter).second.end(); gIter++){
            makeNode(tmpName,(*gIter));
        }
    }
	SceneManager::Instance()->addMask(sheets["screen"]);
	SceneManager::Instance()->addInfoMask(sheets["info-screen"]);
    SceneManager::Instance()->pushSheet(sheets["attract"]);
}

void GuiConfigurator::makeNode(string _handle, map<string,string> &_attrs){
    string type = _attrs["type"];
    if(type == "scrubber"){
        sheets[_handle]->addNode(new GuiScrubber(_attrs));
        //cout << "CREATING SCRUBBER" << endl;
    } else if(type == "image"){
        sheets[_handle]->addNode(new GuiImage(_attrs));
    } else if(type == "video"){
        sheets[_handle]->addNode(new GuiVideo(_attrs));
    } else if(type == "button"){
        sheets[_handle]->addNode(new GuiButton(_attrs));
    } else if(type == "media-preview"){
        sheets[_handle]->addNode(new GuiMediaPreview(_attrs));
    } else if(type == "timeline"){
        sheets[_handle]->addNode(new GuiTimeline(_attrs));
    } else if(type == "viewport"){
        sheets[_handle]->addNode(new GuiViewport(_attrs));
    } else if(type == "video-pager"){
        sheets[_handle]->addNode(new VideoPager(_attrs));
    } else if(type == "loosie") {
        sheets[_handle]->setLoosie();
    }
}

void GuiConfigurator::addSheet(string _handle, GuiSheet* _sheet){
    sheets[_handle] = _sheet;
}

void GuiConfigurator::addLoosie(GuiNode* _loosie){
    loosie = _loosie;
}

GuiNode* GuiConfigurator::getLoosie(){
    return loosie;
}

void GuiConfigurator::print(){
    map<string, vector<map<string,string> > >::iterator mIter;
    vector<map<string, string> >::iterator vIter;
    //cout << "printing map of size - " << attrs.size() << endl;
    for(mIter = attrs.begin(); mIter != attrs.end(); mIter++){
        //cout << "sheet - " << (*mIter).first << endl;
        for(vIter = (*mIter).second.begin(); vIter != (*mIter).second.end(); vIter++){
            map<string,string>::iterator mpIter;
            //cout << "NEW NODE" << endl;
            for(mpIter = (*vIter).begin(); mpIter != (*vIter).end(); mpIter++){
                string first = (*mpIter).first;
                string second = (*mpIter).second;
                //cout << "--->" << first << "==>" << second << endl;
            }
        }
    }
}

void GuiConfigurator::deliverMessage(map<string,string> _msg){
    map<string,GuiSheet*>::iterator sIter;
    for(sIter = sheets.begin(); sIter != sheets.end(); ++sIter){
        if((*sIter).second->deliverMessage(_msg)){
            return;
        }
    }
}

GuiSheet* GuiConfigurator::openSheet(string _name){
    SceneManager::Instance()->pushSheet(sheets[_name]);
    if(bFirstStart && _name == "main"){
        SceneManager::Instance()->pushSheet(sheets["start-overlay"]);
        bFirstStart = false;
    }
    return sheets[_name];
}

void GuiConfigurator::setGlobal(string _handle, string _val){
    globals[_handle] = _val;
}

string GuiConfigurator::getGlobal(string _handle){
    return globals[_handle];
}

void GuiConfigurator::closeSheet(string _name){
    SceneManager::Instance()->popSheet();
}

void GuiConfigurator::replaceSheet(string _name){
    SceneManager::Instance()->popSheet();
    SceneManager::Instance()->pushSheet(sheets[_name]);
}

void GuiConfigurator::draw(){
}

void GuiConfigurator::click(int _x, int _y){
}

void GuiConfigurator::reset(){
    bFirstStart = true;
    SceneManager::Instance()->pushSheet(sheets["attract"]);
}
