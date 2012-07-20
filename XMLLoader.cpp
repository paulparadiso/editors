#include "XMLLoader.h"

XMLLoader::XMLLoader()
{

}

XMLLoader::XMLLoader(string _file){
    loadFile(_file);
}

void XMLLoader::loadFile(string _file){
    XML.loadFile(_file);
}

string XMLLoader::getValue(string _tag){
    return XML.getValue(_tag, "none");
}

XMLLoader::~XMLLoader()
{
    //dtor
}
