//
// Created by LPF on 2023/2/23.
//

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "OpenSLAudioPlay.h"

// 一帧音频播放完毕后就会回调这个函数
void playCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    auto *player = (OpenSLAudioPlay *) context;
    pthread_mutex_unlock(&player->mMutex);
}


OpenSLAudioPlay::OpenSLAudioPlay(int sampleRate, int sampleFormat, int channels) {
    mAudioEngine = new AudioEngine();
    mPlayerObj = nullptr;
    mPlayer = nullptr;
    mBufferQueue = nullptr;
    mEffectSend = nullptr;
    mVolume = nullptr;
    mSampleRate = (SLmilliHertz) sampleRate * 1000;
    mSampleFormat = sampleFormat;
    mChannels = channels;
    mBufferSize = 0;
    mIndex = 0;
    mMutex = PTHREAD_MUTEX_INITIALIZER;
    mBuffers[0] = nullptr;
    mBuffers[1] = nullptr;
}

OpenSLAudioPlay::~OpenSLAudioPlay() {

}

bool OpenSLAudioPlay::init() {
    SLresult result;
    //第三大步 创建播放器
    //3.1 配置输入声音信息
    //创建buffer缓冲类型的队列 2个队列
    SLDataLocator_AndroidSimpleBufferQueue locBufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    // pcm数据格式
    // SL_DATAFORMAT_PCM：数据格式为pcm格式
    // 2：双声道
    // SL_SAMPLINGRATE_44_1：采样率为44100（44.1赫兹 应用最广的，兼容性最好的）
    // SL_PCMSAMPLEFORMAT_FIXED_16：采样格式为16bit （16位）(2个字节)
    // SL_PCMSAMPLEFORMAT_FIXED_16：数据大小为16bit （16位）（2个字节）
    // SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT：左右声道（双声道）  （双声道 立体声的效果）
    // SL_BYTEORDER_LITTLEENDIAN：小端模式
    SLDataFormat_PCM formatPcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32) mChannels,
            mSampleRate,
            (SLuint32) mSampleFormat,
            (SLuint32) mSampleFormat,
            mChannels == 2 ? 0 : SL_SPEAKER_FRONT_CENTER,
            SL_BYTEORDER_LITTLEENDIAN
    };
    if (mSampleRate) {
        formatPcm.samplesPerSec = mSampleRate;
    }
    // 数据源 将上述配置信息放到这个数据源中
    SLDataSource audioSrc = {&locBufq, &formatPcm};
    // 3.2 配置音轨（输出）
    // 设置混音器
    SLDataLocator_OutputMix locOutputMix = {SL_DATALOCATOR_OUTPUTMIX, mAudioEngine->outputMixObj};
    SLDataSink audioSink = {&locOutputMix, nullptr};
    //  需要的接口 操作队列的接口
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_EFFECTSEND};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    //3.3创建播放器
    result = (*mAudioEngine->engine)->CreateAudioPlayer(
            mAudioEngine->engine,   //引擎对象本身
            &mPlayerObj,            //输出的播放器对象，同样是SLObjectItf类型
            &audioSrc,               //数据的来源
            &audioSink,             //数据的去处,和SLDataSource是相对的
            mSampleRate ? 2
                        : 3,    //sizeof(SLInterfaceID),与下面的SLInterfaceID和SLboolean配合使用,用于标记SLInterfaceID数组和SLboolean的大小
            ids,                    //这里需要传入一个数组,指定创建的播放器会包含哪些Interface
            req);                   //这里也是一个数组,用来标记每个需要包含的Interface);
    checkSL(result, "CreateAudioPlayer");
    //  3.4 初始化播放器：mPlayerObj
    result = (*mPlayerObj)->Realize(mPlayerObj, SL_BOOLEAN_FALSE);
    checkSL(result, "(*mPlayerObj)->Realize");
    //  3.5 获取播放器接口：SLPlayItf mPlayerObj
    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_PLAY, &mPlayer);
    checkSL(result, "(*mPlayerObj)->GetInterface SL_IID_PLAY");
    // TODO 第四大步：设置播放回调函数
    // 4.1 获取播放器队列接口：SLAndroidSimpleBufferQueueItf mBufferQueue
    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_BUFFERQUEUE, &mBufferQueue);
    checkSL(result, "(*mPlayerObj)->GetInterface SL_IID_BUFFERQUEUE");
    // 4.2 设置回调 void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
    result = (*mBufferQueue)->RegisterCallback(mBufferQueue, playCallback, this);
    checkSL(result, "(*mBufferQueue)->RegisterCallback");
    mEffectSend = nullptr;
    if (mSampleRate == 0) {
        result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_EFFECTSEND, &mEffectSend);
        checkSL(result, "(*mPlayerObj)->GetInterface SL_IID_EFFECTSEND");
    }
    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_VOLUME, &mVolume);
    checkSL(result, "(*mPlayerObj)->GetInterface SL_IID_VOLUME");
    // TODO 第五大步：设置播放器状态为播放状态
    result = (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
    checkSL(result, "(*mPlayer)->SetPlayState");
    return true;
}

void OpenSLAudioPlay::enqueueSample(void *data, size_t length) {
    // 必须等待一帧音频播放完毕后才可以 Enqueue 第二帧音频
    pthread_mutex_lock(&mMutex);
    if (mBufferSize < length) {
        mBufferSize = length;
        if (mBuffers[0]) {
            delete[] mBuffers[0];
        }
        if (mBuffers[1]) {
            delete[] mBuffers[1];
        }
        mBuffers[0] = new uint8_t[mBufferSize];
        mBuffers[1] = new uint8_t[mBufferSize];
    }
    memcpy(mBuffers[mIndex], data, length);
    //TODO 第六步 手动激活回调函数
    (*mBufferQueue)->Enqueue(mBufferQueue, mBuffers[mIndex], length);
    mIndex = 1 - mIndex;
}

void OpenSLAudioPlay::release() {
    pthread_mutex_lock(&mMutex);
    if (mPlayerObj) {
        (*mPlayerObj)->Destroy(mPlayerObj);
        mPlayerObj = nullptr;
        mPlayer = nullptr;
        mBufferQueue = nullptr;
        mEffectSend = nullptr;
        mVolume = nullptr;
    }

    if (mAudioEngine) {
        delete mAudioEngine;
        mAudioEngine = nullptr;
    }

    if (mBuffers[0]) {
        delete[] mBuffers[0];
        mBuffers[0] = nullptr;
    }

    if (mBuffers[1]) {
        delete[] mBuffers[1];
        mBuffers[1] = nullptr;
    }

    pthread_mutex_unlock(&mMutex);
    pthread_mutex_destroy(&mMutex);
}

bool OpenSLAudioPlay::checkSL(SLresult code, std::string msg) {
    if (code != SL_RESULT_SUCCESS) {
        LOGE("OpenSLAudioPlay", "%s", msg.c_str());
        return false;
    }
    return true;
}

int OpenSLAudioPlay::getStatus(unsigned int *status) {
    if (mPlayer) {
        (*mPlayer)->GetPlayState(mPlayer, status);
    }
    return 0;
}

void OpenSLAudioPlay::pause() {
    if (mPlayer) {
        (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PAUSED);
    }
}
void OpenSLAudioPlay::stop() {
    if (mPlayer) {
        (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_STOPPED);
    }
    release();
}

void OpenSLAudioPlay::reStart() {
    if (mPlayer) {
        (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
    }
}



