//
// Created by LPF on 2023/3/13.
//

#include "OpenSLRender.h"
#include <unistd.h>

OpenSLRender::~OpenSLRender() {

}
void OpenSLRender::Init() {
    int result = -1;
    do {
        result = CreateEngine();
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateEngine failed");
            break;
        }

        result = CreateOutputMixer();
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateOutputMixer failed");
            break;
        }

        result = CreateAudioPlayer();
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateAudioPlayer failed");
            break;
        }

        m_thread = new std::thread(CreateSLWaitingThread, this);

    } while (false);

    if(result != SL_RESULT_SUCCESS) {
        LOGE(OpenSL, "openSL init failed");
        UnInit();
    }
}

void OpenSLRender::ClearAudioCache() {
    std::unique_lock<std::mutex> lock(m_Mutex);
    for (int i = 0; i < m_AudioFrameQueue.size(); ++i) {
        AudioFrame *audioFrame = m_AudioFrameQueue.front();
        m_AudioFrameQueue.pop();
        delete audioFrame;
    }
}

void OpenSLRender::RenderAudioFrame(uint8_t *pData, int dataSize) {
    if(m_AudioPlayerPlay) {
        if (pData != nullptr && dataSize > 0) {
            //temp resolution, when queue size is too big.
            while(GetAudioFrameQueueSize() >= MAX_QUEUE_BUFFER_SIZE && !m_Exit)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }
            std::unique_lock<std::mutex> lock(m_Mutex);
            AudioFrame *audioFrame = new AudioFrame(pData, dataSize);
            m_AudioFrameQueue.push(audioFrame);
            m_Cond.notify_all();
            lock.unlock();
        }
    } else {
        LOGE(OpenSL, "m_AudioPlayerPlay is null")
    }
}

void OpenSLRender::UnInit() {
    if (m_AudioPlayerPlay) {
        (*m_AudioPlayerPlay)->SetPlayState(m_AudioPlayerPlay, SL_PLAYSTATE_STOPPED);
        m_AudioPlayerPlay = nullptr;
    }

    std::unique_lock<std::mutex> lock(m_Mutex);
    m_Exit = true;
    m_Cond.notify_all();
    lock.unlock();

    if (m_AudioPlayerObj) {
        (*m_AudioPlayerObj)->Destroy(m_AudioPlayerObj);
        m_AudioPlayerObj = nullptr;
        m_BufferQueue = nullptr;
    }

    if (m_OutputMixObj) {
        (*m_OutputMixObj)->Destroy(m_OutputMixObj);
        m_OutputMixObj = nullptr;
    }

    if (m_EngineObj) {
        (*m_EngineObj)->Destroy(m_EngineObj);
        m_EngineObj = nullptr;
        m_EngineEngine = nullptr;
    }

    lock.lock();
    for (int i = 0; i < m_AudioFrameQueue.size(); ++i) {
        AudioFrame *audioFrame = m_AudioFrameQueue.front();
        m_AudioFrameQueue.pop();
        delete audioFrame;
    }
    lock.unlock();

    if(m_thread != nullptr)
    {
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }
}

int OpenSLRender::CreateEngine() {
    SLresult result = SL_RESULT_SUCCESS;
    do {
        result = slCreateEngine(&m_EngineObj, 0, nullptr, 0, nullptr, nullptr);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "slCreateEngine failed");
            break;
        }

        result = (*m_EngineObj)->Realize(m_EngineObj, SL_BOOLEAN_FALSE);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "(*m_EngineObj)->Realize failed");
            break;
        }

        result = (*m_EngineObj)->GetInterface(m_EngineObj, SL_IID_ENGINE, &m_EngineEngine);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "(*m_EngineObj)->GetInterface failed");
            break;
        }

    } while (false);
    return result;
}

int OpenSLRender::CreateOutputMixer() {
    SLresult result = SL_RESULT_SUCCESS;
    do {
        const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
        const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

        result = (*m_EngineEngine)->CreateOutputMix(m_EngineEngine, &m_OutputMixObj, 1, mids, mreq);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateOutputMixer (*m_EngineEngine)->CreateOutputMix failed");
            break;
        }

        result = (*m_OutputMixObj)->Realize(m_OutputMixObj, SL_BOOLEAN_FALSE);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateOutputMixer (*m_OutputMixObj)->Realize failed");
            break;
        }

    } while (false);

    return result;
}

int OpenSLRender::CreateAudioPlayer() {
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,//format type 格式类型
            (SLuint32)2,//channel count 通道数
            SL_SAMPLINGRATE_44_1,//44100hz  采样率
            SL_PCMSAMPLEFORMAT_FIXED_16,// bits per sample  位宽
            SL_PCMSAMPLEFORMAT_FIXED_16,// container size
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,// channel mask  通道屏蔽
            SL_BYTEORDER_LITTLEENDIAN // endianness 字节顺序（大小端序）
    };
    SLDataSource slDataSource = {&android_queue, &pcm};
    // 针对数据接收器的输出混合定位器(混音器)
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, m_OutputMixObj};
    SLDataSink slDataSink = {&outputMix, nullptr};

    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    SLresult result;

    do {

        result = (*m_EngineEngine)->CreateAudioPlayer(m_EngineEngine, &m_AudioPlayerObj, &slDataSource, &slDataSink, 3, ids, req);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateAudioPlayer (*m_EngineEngine)CreateAudioPlayer-> failed");
            break;
        }

        result = (*m_AudioPlayerObj)->Realize(m_AudioPlayerObj, SL_BOOLEAN_FALSE);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateAudioPlayer (*m_AudioPlayerObj)-> Realize");
            break;
        }

        result = (*m_AudioPlayerObj)->GetInterface(m_AudioPlayerObj, SL_IID_PLAY, &m_AudioPlayerPlay);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateAudioPlayer (*m_AudioPlayerObj)-> GetInterface");
            break;
        }

        result = (*m_AudioPlayerObj)->GetInterface(m_AudioPlayerObj, SL_IID_BUFFERQUEUE, &m_BufferQueue);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateAudioPlayer (*m_AudioPlayerObj)-> GetInterface");
            break;
        }

        result = (*m_BufferQueue)->RegisterCallback(m_BufferQueue, AudioPlayerCallback, this);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateAudioPlayer (*m_BufferQueue)-> RegisterCallback");
            break;
        }

        result = (*m_AudioPlayerObj)->GetInterface(m_AudioPlayerObj, SL_IID_VOLUME, &m_AudioPlayerVolume);
        if(result != SL_RESULT_SUCCESS)
        {
            LOGE(OpenSL, "CreateAudioPlayer (*m_AudioPlayerObj)-> GetInterface");
            break;
        }

    } while (false);

    return result;
}

int OpenSLRender::GetAudioFrameQueueSize() {
    std::unique_lock<std::mutex> lock(m_Mutex);
    return m_AudioFrameQueue.size();
}

void OpenSLRender::StartRender() {
    while (GetAudioFrameQueueSize() < MAX_QUEUE_BUFFER_SIZE && !m_Exit) {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Cond.wait_for(lock, std::chrono::milliseconds(10));
        //m_Cond.wait(lock);
        lock.unlock();
    }

    (*m_AudioPlayerPlay)->SetPlayState(m_AudioPlayerPlay, SL_PLAYSTATE_PLAYING);
    AudioPlayerCallback(m_BufferQueue, this);
}

void OpenSLRender::HandleAudioFrameQueue() {
    if (m_AudioPlayerPlay == nullptr) return;

    while (GetAudioFrameQueueSize() < MAX_QUEUE_BUFFER_SIZE && !m_Exit) {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Cond.wait_for(lock, std::chrono::milliseconds(10));
    }

    std::unique_lock<std::mutex> lock(m_Mutex);
    AudioFrame *audioFrame = m_AudioFrameQueue.front();
    if (nullptr != audioFrame && m_AudioPlayerPlay) {
        SLresult result = (*m_BufferQueue)->Enqueue(m_BufferQueue, audioFrame->data, (SLuint32) audioFrame->dataSize);
        if (result == SL_RESULT_SUCCESS) {
            m_AudioFrameQueue.pop();
            delete audioFrame;
        }

    }
    lock.unlock();
}

void OpenSLRender::CreateSLWaitingThread(OpenSLRender *openSlRender) {
    openSlRender->StartRender();
}

void OpenSLRender::AudioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context) {
    auto *openSlRender = static_cast<OpenSLRender *>(context);
    openSlRender->HandleAudioFrameQueue();
}

