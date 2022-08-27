//
// Created by Mr_Lee on 2022/6/18.
//

#include "FFMediaPlayer.h"
#include "render/video/NativeRender.h"
#include "../utils/logUtils.h"


void FFMediaPlayer::init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) {
    jniEnv->GetJavaVM(&mJavaVM);
    mJavaObj = jniEnv->NewGlobalRef(obj);

    //todo 创建视频解码器 还需要音频解码器
    mVideoDecoder = new VideoDecoder(url);
    if (renderType == VIDEO_RENDER_AN_WINDOW) {
        mVideoRender = new NativeRender(jniEnv, surface);
        mVideoDecoder->setVideoRender(mVideoRender);
    }
    mVideoDecoder->setMessageCallback(this, postMessage);
}

void FFMediaPlayer::unInit() {

}

void FFMediaPlayer::play() {
    LOGI(TAG, "FFMediaPlayer::play")
    if (mVideoDecoder) {
        mVideoDecoder->start();
    }
}

void FFMediaPlayer::pause() {
    LOGI(TAG, "FFMediaPlayer::pause")
    if (mVideoDecoder) {
        mVideoDecoder->pause();
    }
}

void FFMediaPlayer::stop() {
    LOGI(TAG, "FFMediaPlayer::pause")
    if (mVideoDecoder) {
        mVideoDecoder->stop();
    }
}

void FFMediaPlayer::seekToPosition(float position) {
    if (mVideoDecoder) {
        mVideoDecoder->seekToPosition(position);
    }
}

long FFMediaPlayer::getMediaParams(int paramType) {
    return 0;
}

void FFMediaPlayer::setMediaParams(int paramType, jobject obj) {
    MediaPlayer::setMediaParams(paramType, obj);
}

JNIEnv *FFMediaPlayer::getJNIEnv(bool *isAttach) {
    return nullptr;
}

jobject FFMediaPlayer::getJavaObj() {
    return nullptr;
}

JavaVM *FFMediaPlayer::getJavaVM() {
    return nullptr;
}

void FFMediaPlayer::postMessage(void *context, int msgType, float msgCode) {

}
