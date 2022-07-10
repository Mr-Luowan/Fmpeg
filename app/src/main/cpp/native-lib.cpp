#include <jni.h>
#include <string>
#include "utils/logUtils.h"
#include "player/PlayerWrapper.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_lis_fmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    LOGI("LPF_TAG", hello.c_str(), 1)
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_lis_fmpeg_ffmpeg_FfmpegUtils_native_1GetFFmpegVersion(JNIEnv *env, jclass clazz) {
    char strBuffer[1024 * 4] = {0};
    strcat(strBuffer, "avcodecConfig:-->");
    strcat(strBuffer, avcodec_configuration());
    return env->NewStringUTF(strBuffer);
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_lis_fmpeg_ffmpeg_FfmpegUtils_native_1Init(JNIEnv *env, jobject obj, jstring jurl,
                                                   jint player_type, jint render_type,
                                                   jobject surface) {
    const char *url = env->GetStringUTFChars(jurl, nullptr);
    PlayerWrapper *player = new PlayerWrapper();
    player->init(env, obj, const_cast<char *>(url), player_type, render_type, surface);
    env->ReleaseStringUTFChars(jurl, url);
    return reinterpret_cast<jlong>(player);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_fmpeg_ffmpeg_FfmpegUtils_native_1play(JNIEnv *env, jobject thiz, jlong player_handle) {
    if(player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper*>(player_handle);
        if (player) {
            player->play();
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_fmpeg_ffmpeg_FfmpegUtils_native_1pause(JNIEnv *env, jobject thiz,
                                                    jlong player_handle) {
    if(player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper*>(player_handle);
        if (player) {
            player->pause();
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_fmpeg_ffmpeg_FfmpegUtils_native_1stop(JNIEnv *env, jobject thiz, jlong player_handle) {
    if(player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper*>(player_handle);
        if (player) {
            player->stop();
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_fmpeg_ffmpeg_FfmpegUtils_native_1unInit(JNIEnv *env, jobject thiz,
                                                     jlong player_handle) {
    if(player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper*>(player_handle);
        if (player) {
            player->unInit();
        }
    }
}
