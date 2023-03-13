//
// Created by Mr_Lee on 2022/6/18.
//

#include "FFMediaPlayer.h"
#include "render/video/NativeRender.h"
#include "../utils/logUtils.h"


void FFMediaPlayer::init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) {
    jniEnv->GetJavaVM(&mJavaVM);
    mJavaObj = jniEnv->NewGlobalRef(obj);
    //视频解码器
    mVideoDecoder = new VideoDecoder(url);
    //音频解码器
    mAudioDecoder = new AudioDecoder(url);
    if (renderType == VIDEO_RENDER_AN_WINDOW) {
        mVideoRender = new NativeRender(jniEnv, surface);
        mVideoDecoder->setVideoRender(mVideoRender);
    }
    mAudioRender = new OpenSLRender();
    mAudioDecoder->setAudioRender(mAudioRender);
    mVideoDecoder->setMessageCallback(this, postMessage);
}

void FFMediaPlayer::unInit() {
    if(mVideoDecoder) {
        delete mVideoDecoder;
        mVideoDecoder = nullptr;
    }

    if(mVideoRender) {
        delete mVideoRender;
        mVideoRender = nullptr;
    }

    if(mAudioDecoder) {
        delete mAudioDecoder;
        mAudioDecoder = nullptr;
    }

    if(mAudioRender) {
        delete mAudioRender;
        mAudioRender = nullptr;
    }

    bool isAttach = false;
    getJNIEnv(&isAttach)->DeleteGlobalRef(mJavaObj);
    if(isAttach){
        getJavaVM()->DetachCurrentThread();
    }

}

void FFMediaPlayer::play() {
    LOGI(TAG, "FFMediaPlayer::play")
    if (mVideoDecoder) {
        mVideoDecoder->start();
    }
    if (mAudioDecoder) {
        mAudioDecoder->start();
    }
}

void FFMediaPlayer::pause() {
    LOGI(TAG, "FFMediaPlayer::pause")
    if (mVideoDecoder) {
        mVideoDecoder->pause();
    }
    if (mAudioDecoder) {
        mAudioDecoder->pause();
    }
}

void FFMediaPlayer::stop() {
    LOGI(TAG, "FFMediaPlayer::pause")
    if (mVideoDecoder) {
        mVideoDecoder->stop();
    }
    if (mAudioDecoder) {
        mAudioDecoder->stop();
    }
}

void FFMediaPlayer::seekToPosition(float position) {
    if (mVideoDecoder) {
        mVideoDecoder->seekToPosition(position);
    }
    if (mAudioDecoder) {
        mAudioDecoder->seekToPosition(position);
    }
}

long FFMediaPlayer::getMediaParams(int paramType) {
    return 0;
}

void FFMediaPlayer::setMediaParams(int paramType, jobject obj) {
    MediaPlayer::setMediaParams(paramType, obj);
}

JNIEnv *FFMediaPlayer::getJNIEnv(bool *isAttach) {
    JNIEnv *env;
    int status;
    if (nullptr == mJavaVM) {
        return nullptr;
    }
    *isAttach = false;
    status = mJavaVM->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (status != JNI_OK) {
        status = mJavaVM->AttachCurrentThread(&env, nullptr);
        if (status != JNI_OK) {
            return nullptr;
        }
        *isAttach = true;
    }
    return env;
}

jobject FFMediaPlayer::getJavaObj() {
    return mJavaObj;
}

JavaVM *FFMediaPlayer::getJavaVM() {
    return mJavaVM;
}

void FFMediaPlayer::postMessage(void *context, int msgType, float msgCode) {
    LOGI(TAG, "msgCode = %f", msgCode);
//    if(context != nullptr)
//    {
//        FFMediaPlayer *player = static_cast<FFMediaPlayer *>(context);
//        bool isAttach = false;
//        JNIEnv *env = player->getJNIEnv(&isAttach);
//        if(env == nullptr)
//            return;
//        jobject javaObj = player->getJavaObj();
//        jmethodID mid = env->GetMethodID(env->GetObjectClass(javaObj), JAVA_PLAYER_EVENT_CALLBACK_API_NAME, "(IF)V");
//        env->CallVoidMethod(javaObj, mid, msgType, msgCode);
//        if(isAttach){
//            player->getJavaVM()->DetachCurrentThread();
//        }
//    }
}
