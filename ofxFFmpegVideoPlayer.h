#ifndef OFXFFMPEGVIDEOPLAYER_H
#define OFXFFMPEGVIDEOPLAYER_H

#include "ofMain.h"
#include "FFmpegVideoPlayer.h"

class ofxFFmpegVideoPlayer
{
    public:
        ofxFFmpegVideoPlayer();
        virtual ~ofxFFmpegVideoPlayer();
        void loadMovie(string _file);
        void closeMovie();
        void update();
        void draw();
        void draw(int _x, int _y);
        void draw(int _x, int _y, int _sx, int _sy);
        void play();
        void stop();
        void rewind();
        void setPaused(bool _p);
        void setFrame(int _frame);
        int getWidth(){return player->getWidth();}
        int getHeight(){return player->getHeight();}
        int getTotalNumFrames(){return player->getTotalNumFrames();}
        int getCurrentFrame(){return player->getCurrentFrame();}
        float getDuration(){return player->getDuration();}
        float getPosition(){return (float)player->getCurrentFrame() / (float)player->getTotalNumFrames();}
        unsigned char * getPixels(){return player->getPixels();}

        void setVolume(float _vol){}
        void setPosition(float _pos){setFrame(maxFrames * _pos);}

        bool isPlaying(){return bPlaying;}
        bool isFrameNew();

    protected:
    private:
        ofTexture vTex;
        FFmpegVideoPlayer *player;

        int maxFrames;

        /*
        * Whether the video is currently playing on it's own.
        */

        bool bPlaying;
        bool bNewFrame;

        /*
        * Wait time between frames.  Not as accurate as using a separate timer to drive
        * the play method but it will work.  Especially since there is no audio.
        */

        float nextFrameWait;
        float timeOfLastNewFrame;

        float playTime;
};

#endif // OFXFFMPEGVIDEOPLAYER_H
