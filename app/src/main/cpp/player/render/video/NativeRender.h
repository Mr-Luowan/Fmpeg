//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_NATIVERENDER_H
#define FMPEG_NATIVERENDER_H
#include "VideoRender.h"
#include "../../utils/logUtils.h"
#include <string>
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

class NativeRender: public VideoRender {
public:
    NativeRender(JNIEnv *env, jobject surface);

    virtual ~NativeRender();

    virtual void init(int videoWidth, int videoHeight, int *dstSize);

    virtual void renderVideoFrame(NativeImage *pImage);

    virtual void unInit();

private:
    ANativeWindow *mNativeWindow = nullptr;
    ANativeWindow_Buffer mNativeWindowBuffer;
    int mDstWidth;
    int mDstHeight;
};


#endif //FMPEG_NATIVERENDER_H
