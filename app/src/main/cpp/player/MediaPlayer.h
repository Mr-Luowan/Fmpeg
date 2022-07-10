//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_MEDIAPLAYER_H
#define FMPEG_MEDIAPLAYER_H
#include <jni.h>
class MediaPlayer {
public:
    MediaPlayer(){};
    virtual ~MediaPlayer(){};

    virtual void init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) = 0;
    virtual void unInit() = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void seekToPosition(float position) = 0;
    virtual long getMediaParams(int paramType) = 0;
    virtual void setMediaParams(int paramType, jobject obj){}

    virtual JNIEnv *getJNIEnv(bool *isAttach) = 0;
    virtual jobject getJavaObj() = 0;
    virtual JavaVM *getJavaVM() = 0;

    JavaVM *mJavaVM = nullptr;
    jobject mJavaObj = nullptr;

};
#endif //FMPEG_MEDIAPLAYER_H
