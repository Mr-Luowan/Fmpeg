#include <jni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
//矩阵运算库
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "utils/logUtils.h"
#include "player/PlayerWrapper.h"
#include "opensl/OpenSLAudioPlay.h"
#include "PlayCallback.h"
#include "gles_play.h"
#include "MyGLRenderContext.h"
#include "EnvManager.h"
#include "MyAssetManager.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

#define NATIVE_RENDER_CLASS_NAME "com/lis/pplayer/gles3/NativeRender"

FILE *pcmFile;

void testGlm() {
    //向量（1，0，0） 位移 （1，1，0）个单位
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
}

void testCallJavaMethod() {
    int attach;
    JNIEnv *env = EnvManager::getInstance()->getCurrentEnv(&attach);
    if (env != nullptr) {
        jclass clazz = env->FindClass("com/lis/pplayer/util/ResourceUtils");
        jmethodID getPicDataMethod = env->GetStaticMethodID(clazz, "getPicData",
                                                            "(Ljava/lang/String;)[B");
        jstring str = env->NewStringUTF("flower");
        auto byteArray = (jbyteArray) env->CallStaticObjectMethod(clazz, getPicDataMethod, str);
        jsize length = env->GetArrayLength(byteArray);
        auto *buf = new uint8_t[length];
        env->GetByteArrayRegion(byteArray, 0, length, reinterpret_cast<jbyte *>(buf));
        env->DeleteLocalRef(clazz);
        env->DeleteLocalRef(str);
    }
}


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
    MyGLRenderContext::GetInstance();
}

JNIEXPORT void JNICALL native_OnUnInit(JNIEnv *env, jobject instance) {
    MyGLRenderContext::DestroyInstance();
}

JNIEXPORT void JNICALL native_OnSurfaceCreated(JNIEnv *env, jobject instance) {
    MyGLRenderContext::GetInstance()->OnSurfaceCreated();
    testGlm();
    testCallJavaMethod();
}

JNIEXPORT void JNICALL
native_OnSurfaceChanged(JNIEnv *env, jobject instance, jint width, jint height) {
    MyGLRenderContext::GetInstance()->OnSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL native_OnDrawFrame(JNIEnv *env, jobject instance) {
    MyGLRenderContext::GetInstance()->OnDrawFrame();
}

JNIEXPORT void JNICALL switchContent(JNIEnv *env, jobject instance, jint index) {
    MyGLRenderContext::GetInstance()->switchContent(index);
}

JNIEXPORT void JNICALL
setImageData(JNIEnv *env, jobject instance, jint format, jint width, jint height,
             jbyteArray imgData) {
    int len = env->GetArrayLength(imgData);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(imgData, 0, len, reinterpret_cast<jbyte *>(buf));
    MyGLRenderContext::GetInstance()->setImageData(format, width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(imgData);
}

static JNINativeMethod g_RenderMethods[] = {
        {"native_OnInit",           "()V",      (void *) (native_OnInit)},
        {"native_OnUnInit",         "()V",      (void *) (native_OnUnInit)},
        {"native_OnSurfaceCreated", "()V",      (void *) (native_OnSurfaceCreated)},
        {"native_OnSurfaceChanged", "(II)V",    (void *) (native_OnSurfaceChanged)},
        {"native_OnDrawFrame",      "()V",      (void *) (native_OnDrawFrame)},
        {"native_switchContent",    "(I)V",     (void *) (switchContent)},
        {"native_setImageData",     "(III[B)V", (void *) (setImageData)},
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
    EnvManager::getInstance()->setGlobalJvm(jvm);
    return JNI_VERSION_1_6;
}
extern "C" void JNI_OnUnload(JavaVM *jvm, void *p) {
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return;
    }

    EnvManager::getInstance()->setGlobalJvm(nullptr);
    UnregisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_fmpeg_MainActivity_setNativeAssetsManager(JNIEnv *env, jobject thiz,
                                                       jobject assets_manager) {
    AAssetManager *assetManager = AAssetManager_fromJava(env, assets_manager);
    MyAssetManager::getInstance()->setAssetManager(assetManager);
}