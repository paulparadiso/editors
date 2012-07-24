#ifndef GUICONFIGURATOR_H_INCLUDED
#define GUICONFIGURATOR_H_INCLUDED

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "GuiCreator.h"
#include "Pager.h"
#include "GuiButton.h"
#include "GuiImage.h"
#include "GuiVideo.h"
#include "GuiScrubber.h"
#include "GuiSheet.h"
#include "SceneManager.h"
#include "Observer.h"
#include "Subject.h"
#include "SubObMediator.h"

class GuiCreator;

class GuiConfigurator : public Observer{

public:

    static GuiConfigurator* Instance();
    void addFile(string _file);
    void getTags();
    void makeGUI();
    void makeNode(string _handle, map<string, string> &_attrs);
    void print();
    void draw();
    void addSheet(string _handle, GuiSheet *_sheet);
    GuiSheet* openSheet(string _name);
    void closeSheet(string _name);
    void replaceSheet(string _name);
    void deliverMessage(map<string, string> _msg);
    string getGlobal(string _handle);
    void setGlobal(string _handle, string _val);
    GuiNode* getLoosie();
    void addLoosie(GuiNode* _loosie);
    /*
    Subject/Observer interface
    */

    void update(Subject* _sub);

    /*
    Do this next.
    */
    void testGUI();
    void click(int _x, int _y);
    void reset();

private:

    GuiConfigurator();
    static GuiConfigurator* mInstance;

    ofxXmlSettings mXML;
    void addAttributeMap(string _sheet, vector<string> &_names, vector<string> &_values);

    map<string, vector<map<string,string> > > attrs;
    map<string, vector<GuiNode*> > guiNodes;
    map<string,GuiSheet*> sheets;
    map<string, string> globals;
    map<string,string>sheetTypes;
    GuiCreator* gCr;
    GuiNode* loosie;
    bool bFirstStart;
};


#endif // GUICONFIGURATOR_H_INCLUDED
