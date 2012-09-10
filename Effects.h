#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

#include "ofMain.h"

enum{
    VIDEO_EFFECT_NONE,
    VIDEO_EFFECT_ROTATE,
    VIDEO_EFFECT_WIPE,
    VIDEO_EFFECT_FADE,
    VIDEO_EFFECT_CROSS
};

enum{
    AUDIO_EFFECT_NONE,
    AUDIO_EFFECT_FADE_IN_OUT,
    AUDIO_EFFECT_VOLUME_DOWN,
    AUDIO_EFFECT_VOLUME_UP,
    AUDIO_EFFECT_ECHO,
    AUDIO_EFFECT_MUTE,
    AUDIO_EFFECT_FADE_IN_OUT_TWO,
    AUDIO_EFFECT_VOLUME_DOWN_TWO,
};

class Effect{

public:
    Effect(float _duration){clipDuration = _duration;}
    Effect(float _clipDuration, float _effectLength, float _effectStart, float _multiplier){clipDuration = _clipDuration;
                                                                                            effectLength = _effectLength;
                                                                                            effectStart = _effectStart;
                                                                                            multiplier = _multiplier;
                                                                                            }
    void setClipDuration(float _clipDuration){clipDuration = _clipDuration;}
    void setEffectLength(float _effectsLength){effectLength = _effectsLength;}
    void setEffectStart(float _effectStart){effectStart = _effectStart;}
    void setMultiplier(float _multiplier){multiplier = _multiplier;}
    virtual void process(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pct) = 0;
    virtual ~Effect(){}

protected:
    float clipDuration;
    float effectLength;
    float effectStart;
    float multiplier;
};

class FadeToBlack : public Effect{

public:
    FadeToBlack(float _clipDuration);
    FadeToBlack(float _clipDuration, float _effectLength, float _effectStart, float _multiplier);
    void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);

private:
    void fadeIn(unsigned char* _buf, unsigned char* _oBuf, int _w, int _h, float _pos);
    void fadeOut(unsigned char* _buf, unsigned char* _oBuf, int _w, int _h, float _pos);
};

class FadeIn : public Effect{
    public:
        FadeIn(float _clipDuration);
        void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);
};

class FadeOut : public Effect{
    public:
        FadeOut(float _clipDuration);
        void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);
};

class Greyscale : public Effect{

public:
    Greyscale(float _clipDuration);
    Greyscale(float _clipDuration, float _effectLength, float _effectStart, float _multiplier);
    void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);

};

class Wipe : public Effect{

public:
    Wipe(float _clipDuration);
    Wipe(float _clipDuration, float _effectLength, float _effectStart, float _multiplier);
    void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);

private:
    void wipeIn(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos);
    void wipeOut(unsigned char* _buf, unsigned char * _oBuf, int _w, int _h, float _pos);
};

class Rotate : public Effect{

public:
    Rotate(float _clipDuration);
    Rotate(float _clipDuration, float _effectLength, float _effectStart, float _multiplier);
    void process(unsigned char* _buf,  unsigned char * _oBuf, int _w, int _h, float _pos);

private:
    unsigned char* processedBuffer;

};

#endif // EFFECTS_H_INCLUDED
