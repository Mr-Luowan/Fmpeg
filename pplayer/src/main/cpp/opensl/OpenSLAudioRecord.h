//
// Created by lipengfei6 on 2023/9/25.
//

#ifndef FMPEG_OPENSLAUDIORECORD_H
#define FMPEG_OPENSLAUDIORECORD_H

#include "AudioEngine.h"
class AudioContext
{
    FILE* pFile;
    uint8_t *buffer;
    size_t bufferSize;
public:
    AudioContext(FILE *file, uint8_t *buffer, size_t bufferSize)
    {
        this->pFile = file;
        this->buffer = buffer;
        this->bufferSize = bufferSize;
    }
};


class OpenSLAudioRecord {
private:
    AudioEngine* mAudioEngine = nullptr;
    SLObjectItf mRecordObj = nullptr;
    SLRecordItf mRecorder = nullptr;
    SLAndroidSimpleBufferQueueItf mBufferQueue = nullptr;
    SLAndroidConfigurationItf mConfigItf = nullptr;
    SLuint32 mBufferSize;
public:

    OpenSLAudioRecord();

    void record();

    void pause();

    void stop();
};


#endif //FMPEG_OPENSLAUDIORECORD_H
