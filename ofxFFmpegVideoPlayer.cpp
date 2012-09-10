#include "ofxFFmpegVideoPlayer.h"

ofxFFmpegVideoPlayer::ofxFFmpegVideoPlayer()
{
    player = NULL;
    bPlaying = false;
    playTime = 0.0;
    bNewFrame = false;
}

ofxFFmpegVideoPlayer::~ofxFFmpegVideoPlayer()
{
    if(player != NULL){
        delete player;
        player = NULL;
    }
}

void ofxFFmpegVideoPlayer::loadMovie(string _file){
    if(player != NULL){
        delete player;
        player = NULL;
    }
    player = new FFmpegVideoPlayer();
    player->loadMovie(ofToDataPath(_file).c_str());
    maxFrames = player->getTotalNumFrames();
    vTex.allocate(player->getWidth(), player->getHeight(), GL_RGB);
    nextFrameWait = 1.0 / (float)player->getFrameRate();
}

void ofxFFmpegVideoPlayer::setFrame(int _frame){
    player->setFrame(_frame);
    player->update();
}

void ofxFFmpegVideoPlayer::update(){
    if(bPlaying){
        if(ofGetElapsedTimef() - timeOfLastNewFrame > nextFrameWait){
            player->update();
            bNewFrame = true;
            vTex.loadData(player->getPixels(), player->getWidth(), player->getHeight(), GL_RGB);
            timeOfLastNewFrame = ofGetElapsedTimef();
            playTime += nextFrameWait;
            if(playTime >= player->getDuration() - 0.3){
                bPlaying = false;
                cout << "stopping video." << endl;
            }
        }
    }
}

bool ofxFFmpegVideoPlayer::isFrameNew(){
    if(bNewFrame){
        bNewFrame = false;
        return true;
    } else {
        return false;
    }
}

void ofxFFmpegVideoPlayer::draw(){
    draw(0,0,player->getWidth(), player->getHeight());
}

void ofxFFmpegVideoPlayer::draw(int _x, int _y){
    draw(_x, _y, player->getWidth(), player->getHeight());
}

void ofxFFmpegVideoPlayer::draw(int _x, int _y, int _sx, int _sy){
    vTex.draw(_x,_y,_sx,_sy);
}

void ofxFFmpegVideoPlayer::play(){
    bPlaying = true;
    timeOfLastNewFrame = ofGetElapsedTimef();
}

void ofxFFmpegVideoPlayer::stop(){
    bPlaying = false;
}

void ofxFFmpegVideoPlayer::rewind(){
    player->setFrame(0);
    playTime = 0.0;
}

void ofxFFmpegVideoPlayer::setPaused(bool _p){
    bPlaying = _p;
}
