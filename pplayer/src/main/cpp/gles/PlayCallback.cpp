//
// Created by LPF on 2023/2/24.
//

#include "PlayCallback.h"
#include "logUtils.h"

PlayCallback::PlayCallback(JNIEnv *env, jobject job) {

}

PlayCallback::~PlayCallback() {

}

void PlayCallback::onSucceed(const char * msg) {
    LOGE("TEST_LPF", "%s", msg)
}

void PlayCallback::onError(const char * msg) {
    LOGE("TEST_LPF", "%s", msg)
}

void PlayCallback::toJavaMessage(const char *message) {

}
