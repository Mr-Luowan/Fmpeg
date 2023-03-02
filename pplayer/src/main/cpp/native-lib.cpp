#include <jni.h>
#include <string>
#include "utils/logUtils.h"
#include "player/PlayerWrapper.h"
#include "opensl/OpenSLAudioPlay.h"
#include "PlayCallback.h"
#include "gles_play.h"
#include "MyGLRenderContext.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

#define NATIVE_RENDER_CLASS_NAME "com/lis/pplayer/gles3/NativeRender"

FILE *pcmFile;

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
    if (player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper *>(player_handle);
        if (player) {
            player->play();
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1pause(JNIEnv *env, jobject thiz,
                                               jlong player_handle) {
    if (player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper *>(player_handle);
        if (player) {
            player->pause();
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1stop(JNIEnv *env, jobject thiz, jlong player_handle) {
    if (player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper *>(player_handle);
        if (player) {
            player->stop();
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1unInit(JNIEnv *env, jobject thiz,
                                                jlong player_handle) {
    if (player_handle != 0) {
        auto *player = reinterpret_cast<PlayerWrapper *>(player_handle);
        if (player) {
            player->unInit();
        }
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_lis_pplayer_FfmpegUtils_stringFromJNI(JNIEnv *env, jobject thiz) {
    const char *hello = "String From C++";
    return env->NewStringUTF(hello);
}

void *playThreadFunc(void *arg) {
    auto *slAudioPlayer = reinterpret_cast<OpenSLAudioPlay *>(arg);
    const int bufferSize = 2048;
    short buffer[bufferSize];
    while (!feof(pcmFile)) {
        fread(buffer, 1, bufferSize, pcmFile);
        slAudioPlayer->enqueueSample(buffer, bufferSize);
    }
    return 0;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1playPCMVoice(JNIEnv *env, jobject thiz,
                                                          jstring pcm_path) {
    //将 Java 传递过来的 String 转为 C 中的 char *
    const char *_pcmPath = env->GetStringUTFChars(pcm_path, nullptr);
    auto *slAudioPlay = new OpenSLAudioPlay(44100, SAMPLE_FORMAT_16, 1);
    slAudioPlay->init();
    pcmFile = fopen(_pcmPath, "r");
    pthread_t playThread;
    pthread_create(&playThread, nullptr, playThreadFunc, (void *) slAudioPlay);
    env->ReleaseStringUTFChars(pcm_path, _pcmPath);
    return reinterpret_cast<jlong>(slAudioPlay);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1pausePCMVoice(JNIEnv *env, jobject thiz,
                                                           jlong player_handle) {
    auto *slAudioPlayer = reinterpret_cast<OpenSLAudioPlay *>(player_handle);
    slAudioPlayer->pause();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1stopPCMVoice(JNIEnv *env, jobject thiz,
                                                          jlong player_handle) {
    auto *slAudioPlayer = reinterpret_cast<OpenSLAudioPlay *>(player_handle);
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
    auto *slAudioPlayer = reinterpret_cast<OpenSLAudioPlay *>(player_handle);
    unsigned int playStatus = -1;
    slAudioPlayer->getStatus(&playStatus);
    return jint(playStatus);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1reStartPCMVoice(JNIEnv *env, jobject thiz,
                                                             jlong player_handle) {
    auto *slAudioPlayer = reinterpret_cast<OpenSLAudioPlay *>(player_handle);
    slAudioPlayer->reStart();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_YUVPlay_nativeGlesPlay(JNIEnv *env, jobject thiz, jstring yuv420pPath_,
                                            jobject surface) {
    const char *yuv420pPath = env->GetStringUTFChars(yuv420pPath_, 0);

    auto *callback = new PlayCallback(env, thiz);
    auto gles_play = new Gles_play(env, thiz, callback, yuv420pPath, surface);
    LOGE("TEST_LPF", "%s", yuv420pPath);
    //这里prepare 内部会开启一个子线程，由于开启会造成 堆栈溢出 固取消了  JNI 中开启
//    gles_play->prepare();
    gles_play->start();
    env->ReleaseStringUTFChars(yuv420pPath_, yuv420pPath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_YUVPlay_onDestory(JNIEnv *env, jobject thiz) {
}

JNIEXPORT void JNICALL native_OnInit(JNIEnv *env, jobject instance) {
}

JNIEXPORT void JNICALL native_OnUnInit(JNIEnv *env, jobject instance) {
}
unsigned int program;
TriangleSample* triangleSample;

JNIEXPORT void JNICALL native_OnSurfaceCreated(JNIEnv *env, jobject instance) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    triangleSample = new TriangleSample();
    triangleSample->init();
}

JNIEXPORT void JNICALL
native_OnSurfaceChanged(JNIEnv *env, jobject instance, jint width, jint height) {
    glViewport(0, 0, width, height);
}

JNIEXPORT void JNICALL native_OnDrawFrame(JNIEnv *env, jobject instance) {
    triangleSample->draw(0,0);
}

static JNINativeMethod g_RenderMethods[] = {
        {"native_OnInit",           "()V",   (void *) (native_OnInit)},
        {"native_OnUnInit",         "()V",   (void *) (native_OnUnInit)},
        {"native_OnSurfaceCreated", "()V",   (void *) (native_OnSurfaceCreated)},
        {"native_OnSurfaceChanged", "(II)V", (void *) (native_OnSurfaceChanged)},
        {"native_OnDrawFrame",      "()V",   (void *) (native_OnDrawFrame)},
};

static int
RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum) {
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodNum) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnregisterNativeMethods(JNIEnv *env, const char *className) {
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        return;
    }
    env->UnregisterNatives(clazz);
}

extern "C" jint JNI_OnLoad(JavaVM *jvm, void *p) {
    jint jniRet = JNI_ERR;
    JNIEnv *env = nullptr;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return jniRet;
    }
    jint regRet = RegisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME, g_RenderMethods,
                                        sizeof(g_RenderMethods) / sizeof(g_RenderMethods[0]));
    if (regRet != JNI_TRUE) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
extern "C" void JNI_OnUnload(JavaVM *jvm, void *p) {
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return;
    }

    UnregisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME);
}
