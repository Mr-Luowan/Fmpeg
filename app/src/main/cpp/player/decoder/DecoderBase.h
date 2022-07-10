//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_DECODERBASE_H
#define FMPEG_DECODERBASE_H

#include "Decoder.h"
#include <thread>

extern "C" {
#include <libavutil/time.h>
#include <libavutil/frame.h>
#include <libavcodec/jni.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

#define MAX_PATH   2048
#define DELAY_THRESHOLD 100 //100ms
using namespace std;

enum DecoderState {
    STATE_UNKNOWN,
    STATE_DECODING,
    STATE_PAUSE,
    STATE_STOP
};

enum DecoderMsg {
    MSG_DECODER_INIT_ERROR,
    MSG_DECODER_READY,
    MSG_DECODER_DONE,
    MSG_REQUEST_RENDER,
    MSG_DECODING_TIME
};

class DecoderBase : public Decoder {
public:
    DecoderBase() {};

    virtual~ DecoderBase() {};

    //开始播放
    virtual void start();

    //暂停播放
    virtual void pause();

    //停止
    virtual void stop();

    //获取时长
    virtual float getDuration();

    //seek 到某个时间点播放
    virtual void seekToPosition(float position);

    //当前播放的位置，用于更新进度条和音视频同步
    virtual float getCurrentPosition();

    virtual void setMessageCallback(void *context, MessageCallback callback);

protected:
    void *mMsgContext = nullptr;
    MessageCallback messageCallback = nullptr;

    virtual int init(const char *url, AVMediaType avMediaType);

    virtual void unInit();

    virtual void onDecodeReady() = 0;

    virtual void onDecodeDone() = 0;

    virtual void onFrameAvailable(AVFrame *frame) = 0;

    AVCodecContext *getCodecContext() {
        return mAVCodecContext;
    }

private:
    int initFFDecoder();

    void unInitDecoder();

    //启动解码线程
    void startDecodingThread();

    //音视频解码循环
    void decodingLoop();

    //更新显示时间戳
    void updateTimeStamp();

    //音视频同步
    long doAVSync();

    //解码一个packet编码数据
    int decodeOnePacket();

    //线程函数
    static void doAVDecoding(DecoderBase *decoder);

    //封装格式上下文
    AVFormatContext *mAVFormatContext = nullptr;

    //解码器上下文
    AVCodecContext *mAVCodecContext = nullptr;

    //解码器
    AVCodec *mAVCodec = nullptr;

    //编码的数据包
    AVPacket *mPacket = nullptr;

    //解码的帧
    AVFrame *mFrame = nullptr;

    //数据流的类型
    AVMediaType mMediaType = AVMEDIA_TYPE_UNKNOWN;

    //文件地址
    char mUrl[MAX_PATH] = {0};
    //当前播放时间
    long mCurTimeStamp = 0;
    //播放的起始时间
    long mStartTimeStamp = -1;
    //总时长 ms
    long mDuration = 0;
    //数据流索引
    int mStreamIndex = -1;
    //锁和条件变量
    mutex mMutex;
    condition_variable mCond;
    thread *mThread = nullptr;
    //seek position
    volatile float mSeekPosition = 0;
    volatile bool mSeekSuccess = false;
    //解码器状态
    volatile int mDecoderState = STATE_UNKNOWN;
    void *mAVDecoderContext = nullptr;
    AVSyncCallback mAVSyncCallback = nullptr;//用作音视频同步
};


#endif //FMPEG_DECODERBASE_H
