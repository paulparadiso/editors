#ifndef WAVHANDLER_H
#define WAVHANDLER_H

/*
A simple wav file loader.  Written for just one application
it isn't very robust.  It expects 44100, 16-bit, mono
and only writes that format.  In the case of wav files with an
fmtHeader.fmtChuckSize of 18 and an indescernible dataHeader.dataSize
the size of the data is extrapolated rather inaccurately from the size of
the file.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
/*
The wav header.
*/

typedef struct WAV_HEADER{
    char RIFF[4];
    uint32_t chunkSize;
    char chunkId[4];
} wavHeader;

/*
The fmt header.
*/

typedef struct FMT_HEADER{
    char fmtChunkId[4];
    uint32_t fmtChunkSize;
    uint16_t audioFmt;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
} fmtHeader;

/*
The data header.
*/

typedef struct DATA_HEADER{
    char dataChunkId[4];
    uint32_t dataSize;
} dataHeader;

class WavHandler
{
    public:
        WavHandler();
        bool loadWavFile(const char* _inFile);
        bool writeWavFile(const char* _file,
                          float *_buffer,
                          int _numSamples);
        float* getSamples();
        int getNumSamples();
        int getNumChannels();
        int getSampleRate();
        virtual ~WavHandler();
    protected:
    private:
        FILE *wavFile;
        int16_t *rawBuffer16;
        float *sndBuffer;
        int numSamples;
        wavHeader m_wavHeader;
        fmtHeader m_fmtHeader;
        dataHeader m_dataHeader;
        int getFileSize(FILE *_infile);
        void getWavHeader();
        void setWavHeader(int _numSamples);
        bool checkWavHeader();
        void getFmtHeader();
        void setFmtHeader(int _audioFmt,
                          int _numChannels,
                          int _sampleRate,
                          int _bitsPerSample);
        bool checkFmtHeader();
        void getDataHeader();
        void setDataHeader(int _numSamples,
                           int _numChannels,
                           int _bitsPerSample);
        bool checkDataHeader();
        void getRawSamples();
        void loadSamples();
        void loadRawBuffer(float *_buffer, int _numSamples);
};

#endif // WAVHANDLER_H
