#ifndef AUDIORENDERER_H
#define AUDIORENDERER_H

#include "ofMain.h"
#include "Timeline.h"

class Timeline;

class AudioRenderer
{
    public:

        AudioRenderer();
        virtual ~AudioRenderer();
        void renderAudio(string _fileName, vector<Timeline*> &_timelines);
        int getRenderedAudioLength();
        float* getRenderedAudio();
        void writeFile(string _file, float* _buf, int _numSamples);

    protected:

    private:
        float *renderedAudio;

};

#endif // AUDIORENDERER_H
