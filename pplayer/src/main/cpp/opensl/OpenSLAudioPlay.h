//
// Created by LPF on 2023/2/23.
//

#ifndef FMPEG_OPENSLAUDIOPLAY_H
#define FMPEG_OPENSLAUDIOPLAY_H

#define SAMPLE_FORMAT_16 16

#include <pthread.h>
#include "AudioEngine.h"

class OpenSLAudioPlay {
private:
    AudioEngine *mAudioEngine;
    SLObjectItf mPlayerObj;
    SLPlayItf mPlayer;
    SLAndroidSimpleBufferQueueItf mBufferQueue;
    SLEffectSendItf mEffectSend;
    SLVolumeItf mVolume;
    SLmilliHertz mSampleRate;
    int mSampleFormat;
    int mChannels;
    uint8_t *mBuffers[2];
    SLuint32 mBufferSize;
    int mIndex;
    pthread_mutex_t mMutex;
public:
    OpenSLAudioPlay(int sampleRate, int sampleFormat, int channels);

    virtual ~OpenSLAudioPlay();

    bool init();

    void enqueueSample(void *data, size_t length);

    void release();

    friend void playCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

    bool checkSL(SLresult code, std::string msg);

    int getStatus(unsigned int *status);

    void pause();

    void stop();

    void reStart();
};


#endif //FMPEG_OPENSLAUDIOPLAY_H
