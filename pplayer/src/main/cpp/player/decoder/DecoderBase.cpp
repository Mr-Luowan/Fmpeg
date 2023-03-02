//
// Created by Mr_Lee on 2022/6/18.
//

#include "DecoderBase.h"
#include "../utils/logUtils.h"

int DecoderBase::init(const char *url, AVMediaType avMediaType) {
    LOGI(TAG, "DecoderBase::init url is --> %s", url)
    strcpy(mUrl, url);
    mMediaType = avMediaType;
    return 0;
}

void DecoderBase::start() {
    LOGI(TAG, "DecoderBase::start")
    if (mThread == nullptr) {
        startDecodingThread();
    } else {
        std::unique_lock<std::mutex> lock(mMutex);
        mDecoderState = STATE_DECODING;
        mCond.notify_all();
    }
}

void DecoderBase::pause() {
    LOGI(TAG, "DecoderBase::pause")
    std::unique_lock<std::mutex> lock(mMutex);
    mDecoderState = STATE_PAUSE;
}

void DecoderBase::stop() {
    LOGI(TAG, "DecoderBase::stop")
    std::unique_lock<std::mutex> lock(mMutex);
    mDecoderState = STATE_STOP;
    mCond.notify_all();
}

float DecoderBase::getDuration() {
    //ms to s
    return mDuration * 1.0f / 1000;
}

void DecoderBase::seekToPosition(float position) {
    LOGI(TAG, "DecoderBase::seekToPosition %f", position)
    std::unique_lock<std::mutex> lock(mMutex);
    mSeekPosition = position;
    mDecoderState = STATE_DECODING;
    mCond.notify_all();
}

float DecoderBase::getCurrentPosition() {
    LOGI(TAG, "DecoderBase::getCurrentPosition %ld", mCurTimeStamp)
    return mCurTimeStamp;
}

void DecoderBase::setMessageCallback(void *context, MessageCallback callback) {
    LOGI(TAG, "DecoderBase::setMessageCallback")
    mMsgContext = context;
    messageCallback = callback;
}

void DecoderBase::startDecodingThread() {
    LOGI(TAG, "DecoderBase::startDecodingThread")
    mThread = new thread(doAVDecoding, this);
}

int DecoderBase::initFFDecoder() {
    LOGI(TAG, "DecoderBase::initFFDecoder")
    int result = -1;
    LOGI(TAG, "initFFDecoder !!!")
    do {
        //1.创建封装格式上下文
        mAVFormatContext = avformat_alloc_context();

        //2.打开文件
        if (avformat_open_input(&mAVFormatContext, mUrl, NULL, NULL) != 0) {
            LOGE(TAG, "avformat_open_input open file failed")
            break;
        }
        LOGI(TAG, "avformat_open_input open file success !!!")

        //3.获取音视频流信息
        if (avformat_find_stream_info(mAVFormatContext, NULL) != 0) {
            LOGE(TAG, "avformat_find_stream_info find info failed")
            break;
        }

        LOGI(TAG, "avformat_find_stream_info success !!!")
        //4.获取音视频流索引
        for (int i = 0; i < mAVFormatContext->nb_streams; ++i) {
            if (mAVFormatContext->streams[i]->codecpar->codec_type == mMediaType) {
                mStreamIndex = i;
                break;
            }
        }
        if (mStreamIndex == -1) {
            LOGE(TAG, "DecoderBase__获取音视频流索引 find info failed")
            break;
        }

        LOGI(TAG, "DecoderBase__获取音视频流索引  success !!!")
        //5.获取解码器参数
        AVCodecParameters *avCodecParameters = mAVFormatContext->streams[mStreamIndex]->codecpar;

        //6.获取解码器
        mAVCodec = avcodec_find_decoder(avCodecParameters->codec_id);
        if (mAVCodec == nullptr) {
            LOGE(TAG, "avcodec_find_decoder failed")
            break;
        }

        LOGI(TAG, "DecoderBase__avcodec_find_decoder  success !!!")
        //7.创建解码器上下文
        mAVCodecContext = avcodec_alloc_context3(mAVCodec);
        if (avcodec_parameters_to_context(mAVCodecContext, avCodecParameters) != 0) {
            LOGE(TAG, "DecoderBase__avcodec_parameters_to_context failed")
            break;
        }

        LOGI(TAG, "DecoderBase__avcodec_parameters_to_context success !!!")
        AVDictionary *pAVDictionary = nullptr;
        av_dict_set(&pAVDictionary, "buffer_size", "1024000", 0);
        av_dict_set(&pAVDictionary, "stimeout", "20000000", 0);
        av_dict_set(&pAVDictionary, "max_delay", "30000000", 0);
        av_dict_set(&pAVDictionary, "rtsp_transport", "tcp", 0);

        //8.打开解码器
        result = avcodec_open2(mAVCodecContext, mAVCodec, &pAVDictionary);
        if (result < 0) {
            LOGE(TAG, "DecoderBase__avcodec_open2 failed  result=%d", result)
            break;
        }
        LOGI(TAG, "DecoderBase__avcodec_open2 success !!!")
        result = 0;
        mDuration = mAVFormatContext->duration / AV_TIME_BASE * 1000;

        //创建 AVPacket 存放编码数据
        mPacket = av_packet_alloc();
        //创建 AVFrame 存放解码后的数据
        mFrame = av_frame_alloc();
    } while (false);
    if (result != 0 && mMsgContext && messageCallback) {
        messageCallback(mMsgContext, MSG_DECODER_INIT_ERROR, 0);
    }
    return result;
}

void DecoderBase::unInitDecoder() {
    if (mFrame != nullptr) {
        av_frame_free(&mFrame);
        mFrame = nullptr;
    }
    if (mPacket != nullptr) {
        av_packet_free(&mPacket);
        mPacket = nullptr;
    }
    if (mAVCodecContext != nullptr) {
        avcodec_close(mAVCodecContext);
        avcodec_free_context(&mAVCodecContext);
        mAVCodecContext = nullptr;
        mAVCodec = nullptr;
    }

    if (mAVFormatContext != nullptr) {
        avformat_close_input(&mAVFormatContext);
        avformat_free_context(mAVFormatContext);
        mAVFormatContext = nullptr;
    }
}

void DecoderBase::decodingLoop() {
    LOGI(TAG, "DecoderBase__解码循环 decodingLoop mMediaType=%d", mMediaType)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mDecoderState = STATE_DECODING;
        lock.unlock();
    }
    for (;;) {
        while (mDecoderState == STATE_PAUSE) {
            std::unique_lock<std::mutex> lock(mMutex);
            mCond.wait_for(lock, std::chrono::milliseconds(10));
            mStartTimeStamp = GetSysCurrentTime() - mCurTimeStamp;
        }
        if (mDecoderState == STATE_STOP) {
            break;
        }
        if (mStartTimeStamp == -1) {
            mStartTimeStamp = GetSysCurrentTime();
        }
        if (decodeOnePacket() != 0) {
            //解码结束，暂停解码器
            std::unique_lock<std::mutex> lock(mMutex);
            mDecoderState = STATE_PAUSE;
        }
    }
}

void DecoderBase::updateTimeStamp() {
    LOGI(TAG, "DecoderBase::updateTimeStamp")
    std::unique_lock<std::mutex> lock(mMutex);
    if (mFrame->pkt_dts != AV_NOPTS_VALUE) {
        mCurTimeStamp = mFrame->pkt_dts;
    } else if (mFrame->pts != AV_NOPTS_VALUE) {
        mCurTimeStamp = mFrame->pts;
    } else {
        mCurTimeStamp = 0;
    }
    LOGI(TAG, "updateTimeStamp__::mCurTimeStamp us--> %ld", mCurTimeStamp)
    mCurTimeStamp = (int64_t) (
            (mCurTimeStamp * av_q2d(mAVFormatContext->streams[mStreamIndex]->time_base)) * 1000);
    LOGI(TAG, "updateTimeStamp__::mCurTimeStamp ms--> %ld", mCurTimeStamp)
    if (mSeekPosition > 0 && mSeekSuccess) {
        mStartTimeStamp = GetSysCurrentTime() - mCurTimeStamp;
        mSeekPosition = 0;
        mSeekSuccess = false;
    }
}

long DecoderBase::doAVSync() {
    LOGI(TAG, "doAVSync__音视频同步")
    long curSysTime = GetSysCurrentTime();
    LOGI(TAG, "doAVSync__curSysTime ==> %ld", curSysTime)
    //基于系统时钟计算从开始播放流逝的时间
    LOGI(TAG, "doAVSync__mCurTimeStamp ==> %ld", mCurTimeStamp)
    long elapsedTime = curSysTime - mStartTimeStamp;
    LOGI(TAG, "doAVSync__elapsedTime ==> %ld", elapsedTime)
    if (mMsgContext && messageCallback && mMediaType == AVMEDIA_TYPE_AUDIO) {
        messageCallback(mMsgContext, MSG_DECODING_TIME, mCurTimeStamp * 1.0f / 1000);
    }
    long delay = 0;
    //向系统时钟同步
    if (mCurTimeStamp > elapsedTime) {
        //休眠时间 单位： ms
        auto sleepTime = static_cast<unsigned int>(mCurTimeStamp - elapsedTime);
        //限制休眠时间不能过长
        sleepTime = sleepTime > DELAY_THRESHOLD ? DELAY_THRESHOLD : sleepTime;
        av_usleep(sleepTime * 1000);
    }
    delay = elapsedTime - mCurTimeStamp;
    return delay;
}

int DecoderBase::decodeOnePacket() {
    LOGI(TAG, "DecoderBase::DecodeOnePacket mMediaType=%d", mMediaType);
    if (mSeekPosition > 0) {

    }
    int result = av_read_frame(mAVFormatContext, mPacket);
    while (result == 0) {
        if (mPacket->stream_index == mStreamIndex) {
            if (avcodec_send_packet(mAVCodecContext, mPacket) == AVERROR_EOF) {
                result = -1;
                LOGI(TAG, "解码结束")
                goto __EXIT;
            }
            //一个 packet 包含多少 frame?
            int frameCount = 0;
            while (avcodec_receive_frame(mAVCodecContext, mFrame) == 0) {
                //更新时间戳
                updateTimeStamp();
                //同步
                doAVSync();
                //渲染
                onFrameAvailable(mFrame);
                frameCount++;
            }
            LOGE(TAG, "BaseDecoder::DecodeOneFrame frameCount=%d", frameCount);
            //判断一个 packet 是否解码完成
            if (frameCount > 0) {
                result = 0;
                goto __EXIT;
            }
        }
        av_packet_unref(mPacket);
        result = av_read_frame(mAVFormatContext, mPacket);
    }

    __EXIT:
    av_packet_unref(mPacket);
    return 0;
}

void DecoderBase::doAVDecoding(DecoderBase *decoder) {
    LOGI("DecoderBase.cpp", "doAVDecoding")
    do {
        if (decoder->initFFDecoder() != 0) {
            break;
        }
        decoder->onDecodeReady();
        decoder->decodingLoop();
    } while (false);
    decoder->unInitDecoder();
    decoder->onDecodeDone();
}


void DecoderBase::unInit() {
    if (mThread) {
        stop();
        mThread->join();
        delete mThread;
        mThread = nullptr;
    }
}

