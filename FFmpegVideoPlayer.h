#ifndef FFMPEGVIDEOPLAYER_H
#define FFMPEGVIDEOPLAYER_H

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
}

class FFmpegVideoPlayer
{
    public:
        FFmpegVideoPlayer();
        FFmpegVideoPlayer(const char * inFile);
        virtual ~FFmpegVideoPlayer();
        bool loadMovie(const char * inFile);
        void closeMovie();
        void update();
        int getWidth();
        int getHeight();
        void setFrame(int frameNum);
        int getFrameRate();
        int getTotalNumFrames(){return numFrames - 7;}
        int getCurrentFrame(){}
        float getDuration(){return duration;}

        unsigned char * getPixels();

    protected:
    private:

        /*
        FFmpeg components
        */

        float rationalToFloat(AVRational in); //Convert AVRations to float.

        AVFormatContext *m_FormatCtx;       //The format context
        AVCodecContext *m_VideoCodecCtx;    //The Codec context
        AVFrame *m_VideoFrame;              //The Raw (YUV) frame data
        AVFrame *m_VideoFrameRGB;           //The RGB frame
        SwsContext *m_SwScalingContext;     //Scaling context, used to convert from YUV to RGB.

        int videoStreamIndex;

        unsigned char * m_VideoBuffer;

        /*
        *Video data.
        */

        float duration;
        float framerate;
        int numFrames;
        int currentFrame;
};

#endif // FFMPEGVIDEOPLAYER_H
