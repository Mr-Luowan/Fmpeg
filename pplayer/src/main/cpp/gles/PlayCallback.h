//
// Created by LPF on 2023/2/24.
//

#ifndef FMPEG_PLAYCALLBACK_H
#define FMPEG_PLAYCALLBACK_H
#include <jni.h>


class PlayCallback {
public:
    PlayCallback(JNIEnv *env, jobject job);

    ~PlayCallback();

    void onSucceed(const char *);

    void onError(const char *);

    void toJavaMessage(const char *message);

private:
    JavaVM *javaVm = 0;
    JNIEnv *env = 0;
    jobject instance;

    jmethodID jmd_showMessage;
};


#endif //FMPEG_PLAYCALLBACK_H
