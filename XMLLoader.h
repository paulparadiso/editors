#ifndef XMLLOADER_H
#define XMLLOADER_H

#include "ofMain.h"
#include "ofxXmlSettings.h"

class XMLLoader
{
    public:
        XMLLoader();
        XMLLoader(string _file);
        virtual ~XMLLoader();
        string getValue(string _tag);
        void loadFile(string _file);
    private:
        ofxXmlSettings XML;

};

#endif // XMLLOADER_H
