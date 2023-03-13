//
// Created by LPF on 2023/3/10.
//

#ifndef FMPEG_AUDIODECODER_H
#define FMPEG_AUDIODECODER_H
#include "DecoderBase.h"
#include "Decoder.h"
#include "AudioRender.h"
#include "logUtils.h"

extern "C" {
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
};
// 音频编码采样率
static const int AUDIO_DST_SAMPLE_RATE = 44100;
// 音频编码通道数
static const int AUDIO_DST_CHANNEL_COUNTS = 2;
// 音频编码声道格式
static const uint64_t AUDIO_DST_CHANNEL_LAYOUT = AV_CH_LAYOUT_STEREO;
// 音频编码比特率
static const int AUDIO_DST_BIT_RATE = 64000;
// ACC音频一帧采样数
static const int ACC_NB_SAMPLES = 1024;

class AudioDecoder: public DecoderBase {
public:
    AudioDecoder(char * url);

    virtual ~AudioDecoder();

    void setAudioRender(AudioRender *audioRender);

private:
    const AVSampleFormat DST_SAMPLT_FORMAT = AV_SAMPLE_FMT_S16;
    AudioRender  *m_AudioRender = nullptr;
    //audio resample context
    SwrContext   *m_SwrContext = nullptr;

    uint8_t      *m_AudioOutBuffer = nullptr;

    //number of sample per channel
    int           m_nbSamples = 0;

    //dst frame data size
    int           m_DstFrameDataSze = 0;
    virtual void ClearCache();
    void onFrameAvailable(AVFrame *frame) override;

protected:
    virtual void onDecodeReady();

    virtual void onDecodeDone();

};


#endif //FMPEG_AUDIODECODER_H
