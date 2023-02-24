#include <jni.h>
#include <string>
#include "utils/logUtils.h"
#include "player/PlayerWrapper.h"
#include "opensl/OpenSLAudioPlay.h"

extern "C" {
#include <libavcodec/avcodec.h>
}


FILE * pcmFile;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1GetFFmpegVersion(JNIEnv *env, jclass clazz) {
    char strBuffer[1024 * 4] = {0};
    strcat(strBuffer, "avcodecConfig:-->");
    strcat(strBuffer, avcodec_configuration());
    return env->NewStringUTF(strBuffer);
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1Init(JNIEnv *env, jobject obj, jstring jurl,
                                              jint player_type, jint render_type, jobject surface) {
    const char *url = env->GetStringUTFChars(jurl, nullptr);
    PlayerWrapper *player = new PlayerWrapper();
    player->init(env, obj, const_cast<char *>(url), player_type, render_type, surface);
    env->ReleaseStringUTFChars(jurl, url);
    return reinterpret_cast<jlong>(player);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1play(JNIEnv *env, jobject thiz, jlong player_handle) {
    if(player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper*>(player_handle);
        if (player) {
            player->play();
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1pause(JNIEnv *env, jobject thiz,
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
Java_com_lis_pplayer_FfmpegUtils_native_1stop(JNIEnv *env, jobject thiz, jlong player_handle) {
    if(player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper*>(player_handle);
        if (player) {
            player->stop();
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1unInit(JNIEnv *env, jobject thiz,
                                                     jlong player_handle) {
    if(player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper*>(player_handle);
        if (player) {
            player->unInit();
        }
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_lis_pplayer_FfmpegUtils_stringFromJNI(JNIEnv *env, jobject thiz) {
    const char * hello = "String From C++";
    return env->NewStringUTF(hello);
}

void* playThreadFunc(void* arg) {
    auto* slAudioPlayer = reinterpret_cast<OpenSLAudioPlay*>(arg);
    const int bufferSize = 2048;
    short buffer[bufferSize];
    while (!feof(pcmFile)) {
        fread(buffer,1, bufferSize, pcmFile);
        slAudioPlayer->enqueueSample(buffer, bufferSize);
    }
    return 0;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1playPCMVoice(JNIEnv *env, jobject thiz,
                                                          jstring pcm_path) {
    //将 Java 传递过来的 String 转为 C 中的 char *
    const char * _pcmPath = env->GetStringUTFChars(pcm_path, nullptr);
    auto* slAudioPlay = new OpenSLAudioPlay(44100, SAMPLE_FORMAT_16, 1);
    slAudioPlay->init();
    pcmFile = fopen(_pcmPath, "r");
    pthread_t playThread;
    pthread_create(&playThread, nullptr, playThreadFunc, (void*)slAudioPlay);
    env->ReleaseStringUTFChars(pcm_path, _pcmPath);
    return reinterpret_cast<jlong>(slAudioPlay);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1pausePCMVoice(JNIEnv *env, jobject thiz,
                                                           jlong player_handle) {
    auto* slAudioPlayer = reinterpret_cast<OpenSLAudioPlay*>(player_handle);
    slAudioPlayer->pause();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1stopPCMVoice(JNIEnv *env, jobject thiz,
                                                          jlong player_handle) {
    auto* slAudioPlayer = reinterpret_cast<OpenSLAudioPlay*>(player_handle);
    if (slAudioPlayer) {
        slAudioPlayer->stop();
        delete slAudioPlayer;
        slAudioPlayer = nullptr;
    }
    if (pcmFile) {
        fclose(pcmFile);
        pcmFile = nullptr;
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1getPlayStatus(JNIEnv *env, jobject thiz,
                                                           jlong player_handle) {
    auto* slAudioPlayer = reinterpret_cast<OpenSLAudioPlay*>(player_handle);
    unsigned int playStatus = -1;
    slAudioPlayer->getStatus(&playStatus);
    return jint(playStatus);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1reStartPCMVoice(JNIEnv *env, jobject thiz,
                                                             jlong player_handle) {
    auto* slAudioPlayer = reinterpret_cast<OpenSLAudioPlay*>(player_handle);
    slAudioPlayer->reStart();
}