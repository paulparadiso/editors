#include "GuiConfigurator.h"

GuiConfigurator::GuiConfigurator(){
}

GuiConfigurator::GuiConfigurator(string _file){
    mXML.loadFile(_file);
    gCr = new GuiCreator();
}

void GuiConfigurator::getTags(){
    cout << "loading gui data." << endl;
    mXML.pushTag("gui");
    int numSheets = mXML.getNumTags("sheet");
    cout << "have " << numSheets << " sheets." << endl;
    for(int i = 0; i < numSheets; i++){
        string sheetName = mXML.getAttribute("sheet","name","none",i);
        cout << "starting sheet - " << sheetName << endl;
        mXML.pushTag("sheet");
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
    cout << "adding map to - " << _sheet << endl;
    map<string,string> tmpAttr;
    vector<string>::iterator namesIter, valsIter;
    for(namesIter = _names.begin(), valsIter = _values.begin(); namesIter < _names.end() && valsIter < _values.end(); namesIter++, valsIter++){
        cout << "--->adding " << (*namesIter) << ", " << (*valsIter) << " to - " << _sheet << endl;
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
        //vector<map<string,string> > tmpAttrs = (*sIter).second;
        for(gIter = (*sIter).second.begin(); gIter != (*sIter).second.end(); gIter++){
            makeNode(tmpName,(*gIter));
        }
    }
	SceneManager::Instance()->pushSheet(sheets["main"]);
}

void GuiConfigurator::makeNode(string _handle, map<string,string> &_attrs){
    string type = _attrs["type"];
    cout <<"GUI_CREATOR - making new " << type << endl;
    if(type == "menu")
        //guiNodes.push_back(new Menu(_attrs));
        sheets[_handle]->addNode(new Menu(_attrs));
    else if(type == "decoration")
        //guiNodes.push_back(new Decoration(_attrs));
        sheets[_handle]->addNode(new Decoration(_attrs));
    else if(type == "image")
        //guiNodes.push_back(new Image(_attrs));
        sheets[_handle]->addNode(new Image(_attrs));
    else if(type == "button")
        //guiNodes.push_back(new GuiButton(_attrs));
        sheets[_handle]->addNode(new GuiButton(_attrs));
    else if(type == "media-preview")
        //guiNodes.push_back(new MediaPreview(_attrs));
        sheets[_handle]->addNode(new MediaPreview(_attrs));
    else if(type == "timeline")
        //guiNodes.push_back(new Timeline(_attrs));
        sheets[_handle]->addNode(new Timeline(_attrs));
}


void GuiConfigurator::print(){
    map<string, vector<map<string,string> > >::iterator mIter;
    vector<map<string, string> >::iterator vIter;
    cout << "printing map of size - " << attrs.size() << endl;
    for(mIter = attrs.begin(); mIter != attrs.end(); mIter++){
        cout << "sheet - " << (*mIter).first << endl;
        for(vIter = (*mIter).second.begin(); vIter != (*mIter).second.end(); vIter++){
            map<string,string>::iterator mpIter;
            cout << "NEW NODE" << endl;
            for(mpIter = (*vIter).begin(); mpIter != (*vIter).end(); mpIter++){
                string first = (*mpIter).first;
                string second = (*mpIter).second;
                cout << "--->" << first << "==>" << second << endl;
            }
        }
    }
}

void GuiConfigurator::draw(){
}

void GuiConfigurator::click(int _x, int _y){
}
