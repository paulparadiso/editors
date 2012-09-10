#include "FFmpegVideoPlayer.h"

FFmpegVideoPlayer::FFmpegVideoPlayer()
{
    //ctor
}

FFmpegVideoPlayer::FFmpegVideoPlayer(const char * inFile)
{

}

FFmpegVideoPlayer::~FFmpegVideoPlayer()
{
    closeMovie();
}

void FFmpegVideoPlayer::closeMovie(){
    if(m_VideoFrame != NULL){
        av_free(m_VideoFrame);
        m_VideoFrame = NULL;
    }
    if(m_VideoFrameRGB != NULL){
        av_free(m_VideoFrameRGB);
        m_VideoFrameRGB = NULL;
    }
    if(m_VideoCodecCtx != NULL){
        avcodec_close(m_VideoCodecCtx);
        m_VideoCodecCtx = NULL;
    }
    if(m_FormatCtx != NULL){
        avformat_close_input(&m_FormatCtx);
        m_FormatCtx = NULL;
    }
}

bool FFmpegVideoPlayer::loadMovie(const char * inFile){

    printf("Mult = %f\n", (float) AV_TIME_BASE / 34.8);

    /*
    * Call to register all FFmpeg codecs.
    */

    av_register_all();

    /*
    * Open the input file.
    */

    printf("Attempting to open %s.\n", inFile);

    m_FormatCtx = NULL;

    if(avformat_open_input(&m_FormatCtx, inFile, NULL, NULL) != 0){
        printf("Fail.\n");
        printf("Could not open %s\n", inFile);
        return false;
    }

    /*
    * Confirm that the stream info has been found before continuing.
    */

    if(avformat_find_stream_info(m_FormatCtx,NULL) < 0){
        printf("Could not find stream info for %s\n", inFile);
        //return false;
    }

    /*
    * Get the stream index for the video stream.
    */

    for(int i = 0; i < m_FormatCtx->nb_streams; i++){
        if(m_FormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            videoStreamIndex = i;
        }
    }

    //printf("%s has a duration of %d.",inFile, m_FormatCtx->duration * AV_TIME_BASE);
    //float duration = (m_FormatCtx->streams[videoStreamIndex]->time_base) * 1000.0;
    duration = m_FormatCtx->duration / (float)AV_TIME_BASE;
    printf("Video duration = %f\n", duration);
    framerate = rationalToFloat(m_FormatCtx->streams[videoStreamIndex]->r_frame_rate);
    printf("Video framerate = %f\n", framerate);
    numFrames = m_FormatCtx->streams[videoStreamIndex]->nb_frames;
    printf("have %d frames\n", numFrames);
    /*
    * Get the pointer to the video stream codec.
    */

    m_VideoCodecCtx = m_FormatCtx->streams[videoStreamIndex]->codec;
    AVCodec *pAVCodec = avcodec_find_decoder(m_VideoCodecCtx->codec_id);
    if(pAVCodec == NULL){
        printf("Could not find codec for %s\n", inFile);
        return false;
    }

    /*
    * Now open the codec.
    */

    AVDictionary *options = NULL;
    if(avcodec_open2(m_VideoCodecCtx, pAVCodec, &options) < 0){
        printf("Could not open codec for %s\n", inFile);
    }

    /*
    * Allocate the two frames.
    */

    m_VideoFrame = avcodec_alloc_frame();
    m_VideoFrameRGB = avcodec_alloc_frame();

    m_VideoBuffer = new uint8_t[avpicture_get_size(PIX_FMT_RGB24,m_VideoCodecCtx->width, m_VideoCodecCtx->height)];
    avpicture_fill((AVPicture*)m_VideoFrameRGB, m_VideoBuffer, PIX_FMT_RGB24, m_VideoCodecCtx->width, m_VideoCodecCtx->height);

    //avpicture_fill((AVPicture*)m_VideoFrameRGB, m_VideoBuffer, PIX_FMT_RGB24, m_VideoCodecCtx->width, m_VideoCodecCtx->height);

    /*
    * Initialize the scaling context.
    */

    m_SwScalingContext = sws_getContext(m_VideoCodecCtx->width,
                                        m_VideoCodecCtx->height,
                                        m_VideoCodecCtx->pix_fmt,
                                        m_VideoCodecCtx->width,
                                        m_VideoCodecCtx->height,
                                        PIX_FMT_RGB24,
                                        SWS_BICUBIC,
                                        NULL,NULL,NULL);

    currentFrame = 0;

}

void FFmpegVideoPlayer::update(){
    AVPacket *pAVPacket = NULL;
    pAVPacket = new AVPacket();
    int isFrameDecoded = 0;
    while(!isFrameDecoded){
        if(av_read_frame(m_FormatCtx, pAVPacket) >= 0){
            if(pAVPacket->stream_index == videoStreamIndex){
                avcodec_decode_video2(m_VideoCodecCtx, m_VideoFrame, &isFrameDecoded, pAVPacket);
            }
            if(isFrameDecoded){
                //cout << "Frame decoded." << endl;
                sws_scale(m_SwScalingContext,
                          m_VideoFrame->data,
                          m_VideoFrame->linesize,
                          0,
                          m_VideoCodecCtx->height,
                          m_VideoFrameRGB->data,
                          m_VideoFrameRGB->linesize);
            }
        }
        //
    }
    av_free_packet(pAVPacket);
}

void FFmpegVideoPlayer::setFrame(int frame){
    //printf("Seeking to - %d.\n", frame);
    currentFrame = frame;
    int timeMs = duration * ((float)currentFrame / (float)numFrames) * 1000.0;
    int desiredFrame = av_rescale(timeMs, m_FormatCtx->streams[videoStreamIndex]->time_base.den, m_FormatCtx->streams[videoStreamIndex]->time_base.num);
    desiredFrame /= 1000;
    //int desiredFrame = currentFrame * 990;
    if(avformat_seek_file(m_FormatCtx, videoStreamIndex, 0, desiredFrame, desiredFrame, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD) < 0){
        //printf("Video could not seek.\n");
        return;
    /*
    if(av_seek_frame(m_FormatCtx, videoStreamIndex, currentFrame, 0) < 0){
        printf("Video could not seek.\n");
        return;
    */
    } else {
        avcodec_flush_buffers(m_VideoCodecCtx);
        //printf("Video set to frame %d.  Video at %d\n", desiredFrame, m_FormatCtx->pb->pos);
    }
}

int FFmpegVideoPlayer::getWidth(){
    return m_VideoCodecCtx->width;
}

int FFmpegVideoPlayer::getHeight(){
    return m_VideoCodecCtx->height;
}

unsigned char * FFmpegVideoPlayer::getPixels(){
    return (unsigned char *)m_VideoFrameRGB->data[0];
}

int FFmpegVideoPlayer::getFrameRate(){
    return (int)floor(framerate + 0.5);
}

float FFmpegVideoPlayer::rationalToFloat(AVRational r){
    if(r.num == 0 || r.den == 0){
        return 0.0;
    } else {
        return (float)r.num / (float)r.den;
    }
}
