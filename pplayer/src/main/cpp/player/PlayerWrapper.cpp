//
// Created by Mr_Lee on 2022/6/18.
//

#include "PlayerWrapper.h"
#include "FFMediaPlayer.h"

void PlayerWrapper::init(JNIEnv *env, jobject obj, char *url, int playerType, int renderType,
                         jobject surface) {
    mediaPlayer = new FFMediaPlayer();
    mediaPlayer->init(env, obj, url, renderType, surface);
}

void PlayerWrapper::unInit() {
    if (mediaPlayer) {
        mediaPlayer->unInit();
        delete mediaPlayer;
        mediaPlayer = nullptr;
    }

}

void PlayerWrapper::play() {
    if (mediaPlayer) {
        mediaPlayer->play();
    }
}

void PlayerWrapper::pause() {
    if (mediaPlayer) {
        mediaPlayer->pause();
    }
}

void PlayerWrapper::stop() {
    if (mediaPlayer) {
        mediaPlayer->stop();
    }
}

void PlayerWrapper::seekToPosition(float position) {
    if (mediaPlayer) {
        mediaPlayer->seekToPosition(position);
    }
}

long PlayerWrapper::getMediaParams(int paramType) {
    if (mediaPlayer) {
        return mediaPlayer->getMediaParams(paramType);
    }
    return 0;
}

void PlayerWrapper::setMediaParams(int paramType, jobject obj) {
    if (mediaPlayer) {
        mediaPlayer->setMediaParams(paramType, obj);
    }
}
