//
// Created by lipengfei6 on 2023/9/25.
//

#include "OpenSLAudioRecord.h"

void OpenSLAudioRecord::record() {
    if (mAudioEngine == nullptr)
    {
        LOGE("OpenSLAudioRecord::record", "【ERROR】 mAudioEngine is null return");
        return;
    }
    SLDataLocator_IODevice ioDevice = {
            SL_DATALOCATOR_IODEVICE,
            SL_IODEVICE_AUDIOINPUT,
            SL_DEFAULTDEVICEID_AUDIOINPUT,
            nullptr
        };
    SLDataSource recSource = {&ioDevice, nullptr};
    SLDataLocator_AndroidSimpleBufferQueue recBufferQueue = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
            1
    };
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSink dataSink = {&recBufferQueue, &pcm};
    SLInterfaceID iids[2] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION};
    SLboolean required[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    SLboolean result = (*mAudioEngine->engine)->CreateAudioRecorder(
            mAudioEngine->engine,
            &mRecordObj,
            &recSource,
            &dataSink,
            2,
            iids,
            required);
    mAudioEngine->checkSL(result, "CreateAudioRecorder");
    result = (*mRecordObj)->GetInterface(mRecordObj, SL_IID_ANDROIDCONFIGURATION, &mConfigItf);
    mAudioEngine->checkSL(result, "GetInterface( mConfigItf");
    result = (*mRecordObj)->Realize(mRecordObj, SL_BOOLEAN_FALSE);
    mAudioEngine->checkSL(result, "Realize(mRecordObj");
    result = (*mRecordObj)->GetInterface(mRecordObj, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, (void*) &mBufferQueue);
    mAudioEngine->checkSL(result, "GetInterface( mBufferQueue");
    result = (*mRecordObj)->GetInterface(mRecordObj, SL_IID_RECORD, &mRecorder);
    mAudioEngine->checkSL(result, "GetInterface mRecorder");
    auto* buffer = new uint8_t[mBufferSize];
    result = (*mBufferQueue)->Enqueue(mBufferQueue, buffer, mBufferSize);
    mAudioEngine->checkSL(result, "Enqueue mBufferSize");
    result = (*mRecorder)->SetRecordState(mRecorder, SL_RECORDSTATE_RECORDING);
    mAudioEngine->checkSL(result, "SetRecordState SL_RECORDSTATE_RECORDING");
}

void OpenSLAudioRecord::pause() {

}

void OpenSLAudioRecord::stop() {

}

OpenSLAudioRecord::OpenSLAudioRecord(): mAudioEngine(new AudioEngine)
{
    mBufferSize = 2048;
}

