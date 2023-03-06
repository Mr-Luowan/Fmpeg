//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_LOGUTILS_H
#define FMPEG_LOGUTILS_H

#include <android/log.h>
#include <sys/time.h>
#include <jni.h>


#define LOG

static const char *TAG = "LPF_TEST";
#ifdef LOG
#define LOGD(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG,TAG,FORMAT,##__VA_ARGS__);
#define LOGI(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,TAG,FORMAT,##__VA_ARGS__);
#define LOGW(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_WARN,TAG,FORMAT,##__VA_ARGS__);
#define LOGE(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,TAG,FORMAT,##__VA_ARGS__);
#endif
#define FUN_BEGIN_TIME(FUN) {\
    LOGD(TAG, "FUN_BEGIN_TIME %s:%d %s func estart", __FILE__, __LINE__, FUN); \
    long long t0 = GetSysCurrentTime();

#define FUN_END_TIME(FUN) \
    long long t1 = GetSysCurrentTime(); \
    LOGD(TAG, "FUN_END_TIME %s:%d %s func cost time %ldms", __FILE__,  __LINE__, FUN, (long)(t1-t0));}

#define BEGIN_TIME(FUN) {\
    LOGCATE("%s func start", FUN); \
    long long t0 = GetSysCurrentTime();

#define END_TIME(FUN) \
    long long t1 = GetSysCurrentTime(); \
    LOGCATE("%s func cost time %ldms", FUN, (long)(t1-t0));}

static long getCurTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec;
}
static long long GetSysCurrentTime()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    long long curTime = ((long long)(time.tv_sec))*1000+time.tv_usec/1000;
    return curTime;
}

#define GO_CHECK_GL_ERROR(...)   LOGD("GO_CHECK_GL_ERROR","CHECK_GL_ERROR %s glGetError = %d, line = %d, ",  __FUNCTION__, glGetError(), __LINE__)

#define DEBUG_LOGCATE(...) LOGCATE("DEBUG_LOGCATE %s line = %d",  __FUNCTION__, __LINE__)

#endif //FMPEG_LOGUTILS_H
