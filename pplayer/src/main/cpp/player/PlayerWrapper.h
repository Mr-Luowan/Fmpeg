//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_PLAYERWRAPPER_H
#define FMPEG_PLAYERWRAPPER_H

#include <jni.h>
#include <FFMediaPlayer.h>

static const int FFMEDIA_PLAYER = 0;
class PlayerWrapper {
public:
    PlayerWrapper() {};
    virtual ~PlayerWrapper() {};
    void init(JNIEnv * env, jobject obj, char * url, int playerType, int renderType, jobject surface);
    void unInit();

    void play();
    void pause();
    void stop();
    void seekToPosition(float position);
    long getMediaParams(int paramType);
    void setMediaParams(int paramType, jobject obj);

private:
    MediaPlayer *mediaPlayer = nullptr;

};


#endif //FMPEG_PLAYERWRAPPER_H
