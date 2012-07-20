#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>

using namespace std;

class Subject
{
    public:
        Subject() {}
        Subject(string _handle) {handle = _handle;}
        virtual ~Subject(){}
        void update();
        void setVal(int _val){iVal = _val;}
        void setVal(float _val){fVal = _val;}
        void setVal(bool _val){bVal = _val;}
        void setVal(const char* _val){sVal = _val;}
        virtual string getAttr(const char* _key){string ret = _key; return ret;}
        int getIntVal(){return iVal;}
        float getFloatVal(){return fVal;}
        bool getBoolVal(){return bVal;}
        string getStringVal(){return sVal;}
    protected:
    private:
        string handle;
        int iVal;
        float fVal;
        bool bVal;
        string sVal;
};

#endif // SUBJECT_H
