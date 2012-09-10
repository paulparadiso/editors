#include "Utils.h"

ofVec2f stringToVec2f(string _in){
    //cout << "stringToVec2f - " << _in << endl;
    vector<string> res = ofSplitString(_in, ",");
    return ofVec2f(ofToFloat(res[0]),ofToFloat(res[1]));
}

ofVec3f stringToVec3f(string _in){
    vector<string> res = ofSplitString(_in, ",");
    return ofVec3f(ofToFloat(res[0]), ofToFloat(res[1]), ofToFloat(res[2]));
}
