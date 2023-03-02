//
// Created by LPF on 2023/2/24.
//

#ifndef FMPEG_GLES_PLAY_H
#define FMPEG_GLES_PLAY_H

#include <Egl/egl.h>
#include <Egl/eglext.h>
//使用 OpenGL ES需要添加的头文件
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "PlayCallback.h"
#include "GLTools.h"

class Gles_play {
public:
    Gles_play(JNIEnv *env, jobject thiz, PlayCallback *playCallback, const char *data_source, jobject surface);

    ~Gles_play();

    void playYUV(jobject surface);

    void release();

    void prepare();

    void start();


private:
    PlayCallback *playCallback = 0;

    void showMessage(JNIEnv *, const char *, bool);

    JNIEnv *env = 0;
    jobject thiz;
    pthread_t pid_prepare;
    char *data_source = 0;
    jobject surface;
    bool isPlay;
};


#endif //FMPEG_GLES_PLAY_H
