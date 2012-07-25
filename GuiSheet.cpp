#include "GuiSheet.h"

GuiSheet::GuiSheet(){
    haveLoosie = false;
}

void GuiSheet::addNode(GuiNode* _node){
    nodes.push_back(_node);
    _node->setParent(name);
}

/*
void GuiSheet::pushToStack(){
    SceneManager::Instance()->pushSheet(this);
}
*/

void GuiSheet::activate(){
    /*
    if(type == "effects"){
        Compositor::Instance()->rewind();
        effectTarget = GuiConfigurator::Instance()->getGlobal("effect-target");
        cout << name << " of type " << type << " activating.  Adding effects to " << effectTarget << endl;
        int effectStatus = Compositor::Instance()->getEffectStatus(effectTarget);
        cout << "Effect status = " << effectStatus << endl;
        string effectButton = "effect_";
        effectButton += ofToString(effectStatus);
        vector<GuiNode*>::iterator gIter;
        for(gIter = nodes.begin(); gIter != nodes.end(); ++gIter){
            if((*gIter)->getName() == effectButton)
                (*gIter)->click();
        }
        //int clipEffectStatus = MediaCabinet::Instance()->getClip(effectTarget)->getEffectStatus();
        //cout << "current effect status of " << effectTarget << " = " << clipEffectStatus << endl;
    }
    if(type == "pager"){
        Compositor::Instance()->rewind();
    }
    */

}

/*
void GuiSheet::popFromStack(){
    SceneManager::Instance()->popSheet();
}
*/

bool GuiSheet::checkNodes(int _x, int _y, string _state){
    cout << name << " checking nodes.  Nodes size = " << nodes.size() << endl;
    vector<GuiNode*>::iterator gIter;
    int state;
    if(_state == "down"){
        state = MOUSE_STATE_DOWN;
    }
    if(_state == "up"){
        state = MOUSE_STATE_UP;
    }
    if(_state == "drag"){
        state = MOUSE_STATE_DRAG;
    }
    for(gIter = nodes.begin(); gIter != nodes.end(); gIter++){
        /*
        if((*gIter)->isInside(_x,_y)){
            activatedNode = (*gIter);
            vector<GuiNode*>::iterator hIter;
            for(hIter = nodes.begin(); hIter != nodes.end(); hIter++){
                if((*hIter) != activatedNode){
                    (*hIter)->unclick();
                }
            }
            /*
            if(attrs["type"] == "effects" && activatedNode->getAttr("target") == "effects"){
                if(!activatedNode->getClicked()){
                    vector<string> tokens = ofSplitString(activatedNode->getName(),"_");
                    if(tokens.size() > 1){
                        cout << "setting effect status of " << effectTarget << endl;
                        Compositor::Instance()->setEffectStatus(effectTarget,ofToInt(tokens[1]));
                        activatedNode->click();
                    }
                } else {
                    Compositor::Instance()->setEffectStatus(effectTarget,0);
                    activatedNode->unclick();
                }
            }
            */
            /*
            return true;
        }
        */
        cout << "checking node - " << (*gIter)->getName() << endl;;
        if((*gIter)->processMouse(_x, _y, state)){
            cout << "done checking node." << endl;
			return true;
        }
    }
    return false;
}

bool GuiSheet::checkNodesForDrag(int _x, int _y){
    vector<GuiNode*>::iterator gIter;
    for(gIter = nodes.begin(); gIter != nodes.end(); gIter++){
        if((*gIter)->isDragging(_x,_y)){
            activatedNode = (*gIter);
            return true;
        }
    }
    return false;
}

bool GuiSheet::deliverMessage(map<string,string> _msg){
    vector<GuiNode*>::iterator nIter;
    for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
        if((*nIter)->getName() == _msg["target"]){
            (*nIter)->message(_msg);
            return true;
        }
    }
    return false;
}

void GuiSheet::reset(){
    vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
		(*nIter)->reset();
	}
}

void GuiSheet::setAllAttr(string _attr,string _val){
    vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
		(*nIter)->setAllAttr(_attr,_val);
	}
}

void GuiSheet::update(){
    vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
		(*nIter)->update();
	}
}

void GuiSheet::draw(){
	vector<GuiNode*>::iterator nIter;
	for(nIter = nodes.begin(); nIter != nodes.end(); ++nIter){
		(*nIter)->draw();
	}
    if(haveLoosie){
        //GuiConfigurator::Instance()->getLoosie()->drawLoosie();
    }
    /*
    if(attrs["name"] == "audio-recorder"){
        ofImage ball;
        ball.loadImage("cuts/ball_clipped.png");
        float pct = Compositor::Instance()->getRecordingPosition();
        int maxX = 837;
        int xDiff = maxX * pct;
        ofRect(711 + xDiff, 566,maxX - xDiff ,31);
        ball.draw(720 + xDiff - (ball.getWidth() / 2),561);
    }
    */
}

