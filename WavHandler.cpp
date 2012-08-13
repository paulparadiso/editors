#include "WavHandler.h"

WavHandler::WavHandler(){
}

WavHandler::~WavHandler(){
    delete[] sndBuffer;
    delete[] rawBuffer16;
}

bool WavHandler::loadWavFile(const char* _file)
{
    bool bIndescernibleDataLength = false;
    wavFile = fopen(_file, "rb");
    int fileSize = getFileSize(wavFile);
    getWavHeader();
    if(!checkWavHeader()){
        printf("Bad wavHeader.\n");
        return false;
    }
    getFmtHeader();
    if(!checkFmtHeader()){
        printf("Bad fmtHeader.\n");
        return false;
    }
    printf("m_fmtHeader.fmtChuckSize = %d\n", m_fmtHeader.fmtChunkSize);
    if(m_fmtHeader.fmtChunkSize == 18){
        bIndescernibleDataLength = true;
        int16_t extra;
        fread(&extra, sizeof(uint16_t), 1, wavFile);
    }
    getDataHeader();
    if(!checkDataHeader()){
        printf("Bad dataHeader.\n");
        return false;
    }
    if(bIndescernibleDataLength){
        m_dataHeader.dataSize = fileSize - 9000;
        printf("Setting dataSize to %d\n", m_dataHeader.dataSize);
    }
    numSamples = m_dataHeader.dataSize / (m_fmtHeader.bitsPerSample / 8);
    getRawSamples();
    loadSamples();
    fclose(wavFile);
    return true;
}


/*
Only writes 44100, 16-bit, mono.
*/

bool WavHandler::writeWavFile(const char* _file,
                                   float *_buffer,
                                   int _numSamples)
{
    wavFile = fopen(_file, "wb");
    setWavHeader(_numSamples);
    setFmtHeader(1,1, 44100, 16);
    setDataHeader(_numSamples, 1, 16);
    loadRawBuffer(_buffer, _numSamples);
    fwrite(&m_wavHeader,sizeof(wavHeader),1,wavFile);
    fwrite(&m_fmtHeader,sizeof(fmtHeader),1,wavFile);
    fwrite(&m_dataHeader,sizeof(dataHeader),1,wavFile);
    fwrite(rawBuffer16, sizeof(int16_t), _numSamples, wavFile);
    fclose(wavFile);
    return true;
}

int WavHandler::getFileSize(FILE *_infile)
{
    fseek(_infile,0,SEEK_END);
    int fSize = ftell(_infile);
    fseek(_infile,0,SEEK_SET);
    return fSize;
}

void WavHandler::getWavHeader()
{
    fread(&m_wavHeader,sizeof(wavHeader),1,wavFile);
}

void WavHandler::setWavHeader(int _numSamples){
    memcpy(&m_wavHeader.RIFF[0],"RIFF",4);
    m_wavHeader.chunkSize = _numSamples + 36;
    memcpy(&m_wavHeader.chunkId, "WAVE ", 4);
}

bool WavHandler::checkWavHeader(){
    if(m_wavHeader.RIFF[0] != 'R' &&
       m_wavHeader.RIFF[1] != 'I' &&
       m_wavHeader.RIFF[2] != 'F' &&
       m_wavHeader.RIFF[3] != 'F'){
        printf("Bad RIFF.\n");
        return false;
    }
    if(m_wavHeader.chunkId[0] != 'W' &&
       m_wavHeader.chunkId[1] != 'A' &&
       m_wavHeader.chunkId[2] != 'V' &&
       m_wavHeader.chunkId[3] != 'E'){
        printf("Bad WAVE.\n");
        return false;
    }
    return true;
}

void WavHandler::getFmtHeader(){
    fread(&m_fmtHeader,sizeof(fmtHeader),1,wavFile);
}

void WavHandler::setFmtHeader(int _audioFmt,
                              int _numChannels,
                              int _sampleRate,
                              int _bitsPerSample)
{
    memcpy(&m_fmtHeader.fmtChunkId[0],"fmt ",4);
    m_fmtHeader.audioFmt = 1;
    m_fmtHeader.numChannels = _numChannels;
    m_fmtHeader.sampleRate = _sampleRate;
    m_fmtHeader.byteRate = _sampleRate * _numChannels * (_bitsPerSample / 8);
    m_fmtHeader.blockAlign = _numChannels * (_bitsPerSample / 8);
    m_fmtHeader.bitsPerSample = _bitsPerSample;
}

bool WavHandler::checkFmtHeader(){
    if(m_fmtHeader.fmtChunkId[0] != 'f' &&
       m_fmtHeader.fmtChunkId[1] != 'm' &&
       m_fmtHeader.fmtChunkId[2] != 't' &&
       m_fmtHeader.fmtChunkId[3] != ' '){
        printf("Bad fmt .\n");
        return false;
    }
    return true;
}

void WavHandler::getDataHeader(){
    fread(&m_dataHeader,sizeof(dataHeader),1,wavFile);
}

void WavHandler::setDataHeader(int _numSamples,
                               int _numChannels,
                               int _bitsPerSample)
{
    memcpy(&m_dataHeader.dataChunkId,"data",4);
    m_dataHeader.dataSize = _numSamples * _numChannels * (_bitsPerSample / 8);
}

bool WavHandler::checkDataHeader(){
    if(m_dataHeader.dataChunkId[0] != 'd' &&
       m_dataHeader.dataChunkId[1] != 'a' &&
       m_dataHeader.dataChunkId[2] != 't' &&
       m_dataHeader.dataChunkId[3] != 'a'){
        printf("Bad fmt .\n");
        return false;
    }
    return true;
}

void WavHandler::getRawSamples(){
    rawBuffer16 = new int16_t[numSamples];
    printf("numSamples before = %d\n", numSamples);
    int r = fread(&rawBuffer16[0], sizeof(int16_t), numSamples, wavFile);
    printf("numSamples after = %d.\n Read %d samples", numSamples, r);
}

void WavHandler::loadSamples(){
    printf("Loading %d samples\n", numSamples);
    sndBuffer = new float[numSamples];
    for(int i = 0; i < numSamples; i++){
        sndBuffer[i] = (float)rawBuffer16[i] / 0x8000;
    }
}

void WavHandler::loadRawBuffer(float *_buffer, int _numSamples){
    rawBuffer16 = new int16_t[_numSamples];
    for(int i = 0; i < _numSamples; i++){
        rawBuffer16[i] = _buffer[i] * 0x8000;
    }
}

int WavHandler::getNumSamples(){
    return numSamples;
}

float* WavHandler::getSamples(){
    return sndBuffer;
}

int WavHandler::getSampleRate(){
    return m_fmtHeader.sampleRate;
}

int WavHandler::getNumChannels(){
    return m_fmtHeader.numChannels;
}
