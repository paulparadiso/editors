#ifndef GUINODE_H
#define GUINODE_H

#include "ofMain.h"

enum{
    MOUSE_STATE_DOWN,
    MOUSE_STATE_UP,
    MOUSE_STATE_DRAG
};

class GuiNode{

public:

    GuiNode();
    virtual ~GuiNode(){}
    //Test if a touchscreen selection is within the node.
    virtual bool isInside(int _x, int _y);
    virtual bool isDragging(int _x, int _y){return false;}
    virtual bool processMouse(int _x, int _y, int _state) = 0;
    virtual void setPosition(ofVec2f _pos){pos = _pos;}
    virtual void message(map<string,string> _msg){}
    virtual void sendMessage(){}
    virtual void reset(){}
    virtual void setAllAttr(string _attr, string _val){setAttr(_attr,_val);}
    virtual void unclick(){}
    virtual void click(){}
    virtual bool getClicked(){}
    void initialize();
    string getName(){return name;}
    string setName(string &_name){name = _name; cout << "setting name to " << name << endl;}
    void setName(const char * _name){name = _name;}
    string getType(){return type;}
    ofVec2f getPos(){return pos;}
    ofVec2f getSize(){return size;}
    void setSize(float x, float y){size.set(x,y);}
    void setAttr(string _attr, string _val){attrs.insert(pair<string,string>(_attr, _val));}
    string getAttr(string _attr){return attrs[_attr];}
    void setDur(int _dur){dur = _dur;}
    int getDur(){return dur;}
    void setParent(string _parent){parent = _parent;}

    //Virtual methods that each subclass is responsible for defining.
    virtual void execute() = 0;
    virtual void executeDrag(int _x, int _y){}
    virtual void draw() = 0;
    virtual void drawLoosie(){}
    virtual void update(){}

protected:

    map<string,string> attrs;
    ofVec2f pos;
    ofVec2f size;
    string name;
    string type;
    string parent;
    int dur;

};

ofVec2f stringToVec2f(string _in);
ofVec3f stringToVec3f(string _in);

#endif // GUINODE_H
