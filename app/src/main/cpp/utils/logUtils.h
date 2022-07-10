//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_LOGUTILS_H
#define FMPEG_LOGUTILS_H

#include <android/log.h>
#include <sys/time.h>
#include <jni.h>

#define LOG

static const char * TAG = "DecoderBase";
#ifdef LOG
    #define LOGD(TAG, FORMAT,...) __android_log_print(ANDROID_LOG_DEBUG,TAG,FORMAT,##__VA_ARGS__);
    #define LOGI(TAG, FORMAT,...) __android_log_print(ANDROID_LOG_INFO,TAG,FORMAT,##__VA_ARGS__);
    #define LOGW(TAG, FORMAT,...) __android_log_print(ANDROID_LOG_WARN,TAG,FORMAT,##__VA_ARGS__);
    #define LOGE(TAG, FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,TAG,FORMAT,##__VA_ARGS__);
#endif

static long long getSysCurrentTime()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    long long curTime = ((long long)(time.tv_sec))*1000+time.tv_usec/1000;
    return curTime;
}
#endif //FMPEG_LOGUTILS_H
