//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_FFMEDIAPLAYER_H
#define FMPEG_FFMEDIAPLAYER_H

#include "VideoDecoder.h"
#include "AudioDecoder.h"
#include "OpenSLRender.h"
#include "MediaPlayer.h"

class FFMediaPlayer: public MediaPlayer {
public:
    FFMediaPlayer(){};
    virtual ~FFMediaPlayer() {  };

    virtual void init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) override;

    virtual void unInit() override;

    virtual void play() override;

    virtual void pause() override;

    virtual void stop() override;

    virtual void seekToPosition(float position) override;

    virtual long getMediaParams(int paramType) override;

    virtual void setMediaParams(int paramType, jobject obj) override;



private:
    virtual JNIEnv *getJNIEnv(bool *isAttach) override;

    virtual jobject getJavaObj() override;

    virtual JavaVM *getJavaVM() override;

    static void postMessage(void *context, int msgType, float msgCode);

    VideoDecoder *mVideoDecoder = nullptr;
    AudioDecoder *mAudioDecoder = nullptr;

    VideoRender *mVideoRender = nullptr;
    AudioRender *mAudioRender = nullptr;
};


#endif //FMPEG_FFMEDIAPLAYER_H
