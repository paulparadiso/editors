#include "AudioRenderer.h"
#include "sndfile.h"

AudioRenderer::AudioRenderer()
{
    //ctor
}

void AudioRenderer::renderAudio(string _fileName, vector<Timeline*> &_timelines){
    vector<Timeline*>::iterator tIter;
    vector<int> lengths;
    vector<float*>buffers;
    float* masterBuffer;
    int maxBufferSize = 0;
    for(tIter = _timelines.begin(); tIter != _timelines.end(); ++tIter){
        int totalLength = (*tIter)->getTotalLength();
        if(totalLength > maxBufferSize){
            maxBufferSize = totalLength;
        }
        cout << "Timline has " << totalLength << " samples." << endl;
        (*tIter)->assembleData();
        lengths.push_back(totalLength);
        buffers.push_back((*tIter)->getData());
        //writeFile("timeline" + ofToString(totalLength) + ".wav", (*tIter)->getData(), totalLength);
    }
    masterBuffer = (float*)malloc(maxBufferSize * sizeof(float));
    for(int i = 0; i < maxBufferSize; i++){
        float finalSample = 0.0;
        int finalSampleCount = 0;
        for(int j = 0; j < buffers.size(); j++){
            if(i < lengths[j]){
                finalSample += buffers[j][i];
                finalSampleCount++;
            }
        }
        masterBuffer[i] = finalSample;
    }
    writeFile(_fileName,masterBuffer,maxBufferSize);
}

void AudioRenderer::writeFile(string _file, float* _buf, int _numSamples){
    SF_INFO wInfo;
    wInfo.samplerate = 44100;
    wInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_32;
    wInfo.channels = 1;
    SNDFILE *wFile = sf_open(ofToDataPath(_file).c_str(), SFM_RDWR, &wInfo);
    sf_write_sync(wFile);
    int wrote = sf_write_float(wFile, _buf, _numSamples);
    cout << "wrote " << wrote << " to file " << _file << endl;
    sf_close(wFile);
}

AudioRenderer::~AudioRenderer()
{
    //dtor
}


