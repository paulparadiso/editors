#ifndef GUICONFIGURATOR_H_INCLUDED
#define GUICONFIGURATOR_H_INCLUDED

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "GuiCreator.h"
#include "GuiButton.h"
#include "SceneManager.h"

class GuiConfigurator{

public:
    GuiConfigurator();
    GuiConfigurator(string _file);
    void getTags();
    void makeGUI();
    void makeNode(string _handle, map<string, string> &_attrs);
    void print();
    void draw();
    /*
    Do this next.
    */
    void testGUI();
    void click(int _x, int _y);

private:
    ofxXmlSettings mXML;
    void addAttributeMap(string _sheet, vector<string> &_names, vector<string> &_values);

    map<string, vector<map<string,string> > > attrs;
    map<string, vector<GuiNode*> > guiNodes;
    map<string,GuiSheet*> sheets;
    GuiCreator* gCr;
};


#endif // GUICONFIGURATOR_H_INCLUDED
