#ifndef GUICREATOR_H_INCLUDED
#define GUICREATOR_H_INCLUDED

#include "ofMain.h"
#include "Transition.h"

/*

Abstract GuiNode class.  Menus, buttons, timelines, previews,
images and decorations will all inherit from this class.

*/

class GuiNode{

public:

    GuiNode(){}
    virtual ~GuiNode(){}
    //Test if a touchscreen selection is within the node.
    bool isInside(int _x, int _y);
    void initialize();
    string getName(){return name;}
    string getType(){return type;}
    ofVec2f getPos(){return pos;}
    ofVec2f getSize(){return size;}

    //Virtual methods that each subclass is responsible for defining.
    virtual void execute() = 0;
    virtual void draw() = 0;

protected:

    map<string,string> attrs;
    ofVec2f pos;
    ofVec2f size;
    string name;
    string type;
};

class GuiSheet{

public:
    GuiSheet(){}
    void addNode(GuiNode* _node);
    void pushToStack();
    void popFromStack();
    void draw();
    void setName(string _name){name = _name;}
    string getName(){return name;}

private:
    vector<GuiNode*> nodes;
    string name;

};

class GuiCreator{

public:

    GuiCreator();
    //GuiNode* makeNode(map<string,string> &_attrs);

private:
    vector<string> nodes;


};

/*

Menu subclass of GuiNode.  Holds references to other
GuiNode like sub-menus and buttons and contains methods
for adding and deleting references.

*/

class Menu : public GuiNode{

public:

    Menu(map<string,string> &_attrs);
    void addNode(GuiCreator* _gc, map<string, string> &_attrs);
    void deleteNode();

    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();


private:

    vector<GuiNode*> menuItems;
    bool isOpen;
    int state;
    Transition *transition;
    int transitionTime;

};

class GuiButton: public GuiNode{

public:
    GuiButton(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();

private:
    ofImage bg;

};

/*

The Decoration GuiNode adds shapes and colors used in highlighting
to the GUI.

*/

class Decoration : public GuiNode{

public:
    Decoration(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();

private:
    ofVec3f color;

};

class Image : public GuiNode{

public:
    Image(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();

private:
    ofImage img;

};

class MediaPreview : public GuiNode{

public:
    MediaPreview(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();

};

class Timeline : public GuiNode{

public:
    Timeline(map<string,string> &_attrs);
    //void initialize(map<string, string> &_attrs);
    void execute();
    void draw();

};

/*

A few utility functions

*/

ofVec2f stringToVec2f(string _in);
ofVec3f stringToVec3f(string _in);

#endif // GUICREATOR_H_INCLUDED
