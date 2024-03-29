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
#include "VideoGLRender.h"
#include <pthread.h>
#include <fcntl.h>
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
    strcat(strBuffer, av_version_info());
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

jclass g_ResoutUtilsClazz;

void* tThread(void* args)
{
    pthread_t subID = pthread_self();
    LOGE("tThread", "测试线程开始运行 子线程ID = %ld", subID)
    int attach;
    JNIEnv * env = EnvManager::getInstance()->getCurrentEnv(&attach);
    if (env != nullptr) {
        jmethodID mID = env->GetStaticMethodID(g_ResoutUtilsClazz, "sayHello",
                                                            "()V");
        if (mID == nullptr) {
            LOGE("tThread", "获取java静态方法失败")
        }
        env->CallStaticVoidMethod(g_ResoutUtilsClazz, mID);
        env->DeleteGlobalRef(g_ResoutUtilsClazz);
    }
    LOGE("tThread", "子线程获取JNIEnv %d ", attach)

    return nullptr;
}
//todo
JNIEXPORT void JNICALL test(JNIEnv *env, jobject instance) {
    pthread_t mainID = pthread_self();
    LOGE("test", "响应点击动作 主线程ID = %ld", mainID)
    jclass clazz = env->FindClass("com/lis/pplayer/util/ResourceUtils");
    g_ResoutUtilsClazz = (jclass)env->NewGlobalRef(clazz);
    pthread_t testThread;
    pthread_create(&testThread, nullptr, tThread, nullptr);
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

JNIEXPORT void JNICALL updateTransformMatrix(JNIEnv *env, jobject instance,
                                             jfloat angleX, jfloat angleY, jfloat scaleX,
                                             jfloat scaleY) {
    MyGLRenderContext::GetInstance()->updateTransformMatrix(angleX, angleY, scaleX, scaleY);
}

static JNINativeMethod g_RenderMethods[] = {
        {"testNative",                   "()V",      (void *) (test)},
        {"native_OnInit",                "()V",      (void *) (native_OnInit)},
        {"native_OnUnInit",              "()V",      (void *) (native_OnUnInit)},
        {"native_OnSurfaceCreated",      "()V",      (void *) (native_OnSurfaceCreated)},
        {"native_OnSurfaceChanged",      "(II)V",    (void *) (native_OnSurfaceChanged)},
        {"native_OnDrawFrame",           "()V",      (void *) (native_OnDrawFrame)},
        {"native_switchContent",         "(I)V",     (void *) (switchContent)},
        {"native_setImageData",          "(III[B)V", (void *) (setImageData)},
        {"native_UpdateTransformMatrix", "(FFFF)V",  (void *) (updateTransformMatrix)},
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


extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1onSurfaceCreated(JNIEnv *env, jobject thiz,
                                                          jlong player_handle) {
    auto *player = reinterpret_cast<PlayerWrapper *>(player_handle);
    auto *mediaPlayer = player->getMediaPlayer();
    if (typeid(*mediaPlayer) == typeid(FFMediaPlayer)) {
        auto *ffMediaPlayer = dynamic_cast<FFMediaPlayer *>(mediaPlayer);
        if (ffMediaPlayer->getVideoRender()->getRenderType() == VIDEO_RENDER_OPENGL)
        {
            auto* videoRender = ffMediaPlayer->getVideoRender();
            if (typeid(*videoRender) == typeid(VideoGLRender)) {
                auto glRender = dynamic_cast<VideoGLRender*>(videoRender);
                glRender->init();
            }
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1onSurfaceChanged(JNIEnv *env, jobject thiz,
                                                          jlong player_handle, jint width,
                                                          jint height) {
    auto *player = reinterpret_cast<PlayerWrapper *>(player_handle);
    auto *mediaPlayer = player->getMediaPlayer();
    if (typeid(*mediaPlayer) == typeid(FFMediaPlayer)) {
        auto *ffMediaPlayer = dynamic_cast<FFMediaPlayer *>(mediaPlayer);
        if (ffMediaPlayer->getVideoRender()->getRenderType() == VIDEO_RENDER_OPENGL)
        {
            auto* videoRender = ffMediaPlayer->getVideoRender();
            if (typeid(*videoRender) == typeid(VideoGLRender)) {
                auto glRender = dynamic_cast<VideoGLRender*>(videoRender);
                glRender->OnSurfaceChanged(width, height);
            }
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_native_1onDrawFrame(JNIEnv *env, jobject thiz,
                                                     jlong player_handle) {
    auto *player = reinterpret_cast<PlayerWrapper *>(player_handle);
    auto *mediaPlayer = player->getMediaPlayer();
    if (typeid(*mediaPlayer) == typeid(FFMediaPlayer)) {
        auto *ffMediaPlayer = dynamic_cast<FFMediaPlayer *>(mediaPlayer);
        if (ffMediaPlayer->getVideoRender()->getRenderType() == VIDEO_RENDER_OPENGL)
        {
            auto* videoRender = ffMediaPlayer->getVideoRender();
            if (typeid(*videoRender) == typeid(VideoGLRender)) {
                auto glRender = dynamic_cast<VideoGLRender*>(videoRender);
                glRender->draw(0,0);
            }
        }
    }
}

static int char2uint(char *input, uint8_t *output)
{
    for(int i = 0; i < 24; i++) {
        output[i] &= 0x00;
        for (int j = 1; j >= 0; j--) {
            char hb = input[i*2 + 1 - j];
            if (hb >= '0' && hb <= '9') {
                output[i] |= (uint8_t)((hb - '0') << (4*j));
            } else if (hb >= 'a' && hb <= 'f') {
                output[i] |= (int8_t)((hb - 'a' + 10) << (4*j));
            } else if (hb >= 'A' && hb <= 'F') {
                output[i] |= (int8_t)((hb - 'A' + 10) << (4*j));
            }  else {
                return -1;
            }
        }
    }
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lis_pplayer_FfmpegUtils_test(JNIEnv *env, jobject thiz, jstring file_path) {
    const char *filepath = env->GetStringUTFChars(file_path, 0);

    const char * filep = "/storage/emulated/0/Android/data/com.lis.fmpeg/files/audio/a.txt";
    int g_hShmFifo = open(filep, O_RDWR);
    if (g_hShmFifo<0)
    {
        LOGE("测试内容", "打开本地文件失败");
    } else {
        LOGE("测试内容", "打开本地文件成功");
    }


    close(g_hShmFifo);

//    FILE *file = fopen(filepath, "r+");
//    if (!file)
//    {
//        LOGE("测试内容", "打开本地文件失败");
//        return;
//    }
//    char Buffer[100];
//    while (fgets(Buffer, 100, file))//读取文件中的内存，按照指定的长度读取到缓存中去
//    {
//        LOGE("测试内容", "读取文件内容 %s", Buffer);
//    }
//
//    fclose(file);
//    env->ReleaseStringUTFChars(file_path, filepath);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_lis_pplayer_NativePCMPlayer_native_1recordPCMVoice(JNIEnv *env, jobject thiz,
                                                            jstring file_path) {

    return 0;
}