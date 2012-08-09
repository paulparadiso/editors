#include "Effects.h"

FadeToBlack::FadeToBlack(float _duration) : Effect(_duration){
}

FadeToBlack::FadeToBlack(float _clipDuration, float _effectLength, float _effectStart, float _multiplier) : Effect(_clipDuration,_effectLength,_effectStart,_multiplier){
}

void FadeToBlack::process(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos){
    if(multiplier > 0.0){
        fadeOut(_buf,_oBuf,_w,_h,_pos);
    } else {
        fadeIn(_buf,_oBuf,_w,_h,_pos);
    }
}

void FadeToBlack::fadeOut(unsigned char* _buf, unsigned char* _oBuf, int _w, int _h, float _pos){
    if(_pos > 0.75){
        float pct = (_pos - 0.75) / 0.25;
        for(int y = 0; y < _h; y += 1){
            for(int x = 0; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _buf[index] = (_buf[index] * (1.0 - pct));
                _buf[index + 1] = (_buf[index + 1] * (1.0 - pct));
                _buf[index + 2] = (_buf[index + 2] * (1.0 - pct));
            }
        }
    }
}

void FadeToBlack::fadeIn(unsigned char* _buf, unsigned char* _oBuf, int _w, int _h, float _pos) {
    if(_pos < 0.1){
        float pct = _pos / 0.25;
        for(int y = 0; y < _h; y += 1){
            for(int x = 0; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _buf[index] = _buf[index] * pct + _oBuf[index] * (1.0 - pct);
                _buf[index + 1] = _buf[index + 1] * pct + _oBuf[index + 1] * (1.0 - pct);
                _buf[index + 2] = _buf[index + 2] * pct + _oBuf[index + 2] * (1.0 - pct);
            }
        }
    }
}

FadeIn::FadeIn(float _clipDuration) : Effect(_clipDuration){
}

void FadeIn::process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos){
    if(_pos < 1.0){
        float pct = _pos / 1.0;
        for(int y = 0; y < _h; y += 1){
            for(int x = 0; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _oBuf[index] = _buf[index] * pct;
                _oBuf[index + 1] = _buf[index + 1] * pct;
                _oBuf[index + 2] = _buf[index + 2] * pct;
            }
        }
    } else {
        memcpy(_oBuf, _buf, _w * _h * 3);
    }
}

FadeOut::FadeOut(float _clipDuration) : Effect(_clipDuration){
}

void FadeOut::process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos){
    if(_pos > clipDuration - 1.0){
        float progress = clipDuration - _pos;
        float pct = progress / 1.0;
        for(int y = 0; y < _h; y += 1){
            for(int x = 0; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _oBuf[index] = _buf[index] * pct;
                _oBuf[index + 1] = _buf[index + 1] * pct;
                _oBuf[index + 2] = _buf[index + 2] * pct;
            }
        }
    } else {
        memcpy(_oBuf, _buf, _w * _h * 3);
    }
}

Greyscale::Greyscale(float _duration) : Effect(_duration){
}

void Greyscale::process(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos){
    float pct = (_pos - 0.75) / 0.25;
    for(int y = 0; y < _h; y += 1){
        for(int x = 0; x < _w * 3; x += 3){
            int index = y * (_w * 3) + x;
            int average = (_buf[index] + _buf[index + 1] + _buf[index + 2]) / 3;
            _buf[index] = average;
            _buf[index + 1] = average;
            _buf[index + 2] = average;
        }
    }
}

Wipe::Wipe(float _duration) : Effect(_duration){
}

Wipe::Wipe(float _clipDuration, float _effectLength, float _effectStart, float _multiplier) : Effect(_clipDuration,_effectLength,_effectStart,_multiplier){}

void Wipe::process(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos){
    if(_pos < 1.0){
        memset(_oBuf,0,_w * _h * 3);
        float pct = _pos / 1.0;
        int startX = _w * 3  - (_w * 3 * pct);
        for(int y = 0; y < _h; y += 1){
            for(int x = startX; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _oBuf[index] = _buf[index];
                _oBuf[index + 1] = _buf[index + 1];
                _oBuf[index + 2] = _buf[index + 2];
            }
        }
    } else {
        memcpy(_oBuf, _buf, _w * _h * 3);
    }
}

void Wipe::wipeOut(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos){
    if(_pos > effectStart){
        float pct = (_pos - effectStart) / effectLength;
        int maxX = _w * pct;
        for(int y = 0; y < _h; y += 1){
            for(int x = 0; x < maxX * 3; x += 3){
                int index = y * (_w * 3) + x;
                _buf[index] = _oBuf[index];
                _buf[index + 1] = _oBuf[index + 1];
                _buf[index + 2] = _oBuf[index + 2];
            }
        }
    }
}

void Wipe::wipeIn(unsigned char* _buf, unsigned char *_oBuf, int _w, int _h, float _pos){
    if(_pos < effectLength){
        float pct = _pos / effectLength;
        int minX = _w * 3 * pct;
        for(int y = 0; y < _h; y += 1){
            for(int x = minX; x < _w * 3; x += 3){
                int index = y * (_w * 3) + x;
                _buf[index] = _oBuf[index];
                _buf[index + 1] = _oBuf[index + 1];
                _buf[index + 2] = _oBuf[index + 2];
            }
        }
    }
}

Rotate::Rotate(float _duration) : Effect(_duration){
    processedBuffer = new unsigned char[960 * 540 * 3];
}

void Rotate::process(unsigned char* _buf, unsigned char *_oBuf, int _w, int _h, float _pos){
    //memset(processedBuffer,0,_w * _h * 3);
    memset(_oBuf,0,_w * _h * 3);
    for(int y = 0; y < _h ; y++){
        for(int x = 0; x < _w * 3 ; x+=3){
            int oldIndex = y * _w * 3 + x;
            //int oldIndex2 = y * _w * 3 + (x + 1);
            //int oldIndex3 = y * _w * 3 + (x + 2);
            //ex get(10,12) --> 10 * (_w * 3) + 12  =====  get(12,10) --> 12 * (_w * 3) + 10
            //int newIndex = (y * (_w * 3)) * cos(90.0) - x * sin(90.0) + x * cos(90.0) + (y * (_w * 3)) * sin(90.0);
            //int newX = x * cos(-PI/2) + y * sin(-PI/2);
            //int newY = y * cos(-PI/2) + x *sin(-PI/2);

            //90 degrees
            //int newIndex = x * _w + (y + 190) * 3;

            //180 degrees
            int newIndex = (_h - y) * _w * 3 + _w * 3 - x;

            //int newIndex2 = (x + 2) * _w + y * 3;
            //int newIndex3 = (x + 3) * _w + y * 3;
            //cout << "newIndex = " << newIndex << ". Max = "  << _w * _h * 3 << endl;
            if(newIndex > 0 && newIndex < _w * _h * 3){
                _oBuf[newIndex] = _buf[oldIndex];
                _oBuf[newIndex + 1] = _buf[oldIndex + 1];
                _oBuf[newIndex + 2] = _buf[oldIndex + 2];
            }
            //if(newIndex2 > 0 && newIndex2 < _w * _h * 3){
            //    processedBuffer[newIndex2] = _buf[oldIndex2];
                //processedBuffer[newIndex + 1] = _buf[oldIndex + 1];
                //processedBuffer[newIndex + 1] = _buf[oldIndex + 2];
            //}
            //if(newIndex3 > 0 && newIndex3 < _w * _h * 3){
            //    processedBuffer[newIndex3] = _buf[oldIndex3];
                //processedBuffer[newIndex + 1] = _buf[oldIndex + 1];
                //processedBuffer[newIndex + 1] = _buf[oldIndex + 2];
            //}

        }
    }
    //memcpy(_buf,processedBuffer,853 * 480 * 3);
}
