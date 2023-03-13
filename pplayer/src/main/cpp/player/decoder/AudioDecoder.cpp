//
// Created by LPF on 2023/3/10.
//

#include "AudioDecoder.h"

AudioDecoder::~AudioDecoder() {
    unInit();
}

void AudioDecoder::onDecodeReady() {
    if(m_AudioRender) {
        AVCodecContext *codeCtx = getCodecContext();

        m_SwrContext = swr_alloc();

        av_opt_set_int(m_SwrContext, "in_channel_layout", codeCtx->channel_layout, 0);
        av_opt_set_int(m_SwrContext, "out_channel_layout", AUDIO_DST_CHANNEL_LAYOUT, 0);

        av_opt_set_int(m_SwrContext, "in_sample_rate", codeCtx->sample_rate, 0);
        av_opt_set_int(m_SwrContext, "out_sample_rate", AUDIO_DST_SAMPLE_RATE, 0);

        av_opt_set_sample_fmt(m_SwrContext, "in_sample_fmt", codeCtx->sample_fmt, 0);
        av_opt_set_sample_fmt(m_SwrContext, "out_sample_fmt", DST_SAMPLT_FORMAT,  0);

        swr_init(m_SwrContext);

        LOGI("audioDecoder", "AudioDecoder::OnDecoderReady audio metadata sample rate: %d, channel: %d, format: %d, frame_size: %d, layout: %lld",
                codeCtx->sample_rate, codeCtx->channels, codeCtx->sample_fmt, codeCtx->frame_size,codeCtx->channel_layout);

        // resample params
        m_nbSamples = (int)av_rescale_rnd(ACC_NB_SAMPLES, AUDIO_DST_SAMPLE_RATE, codeCtx->sample_rate, AV_ROUND_UP);
        m_DstFrameDataSze = av_samples_get_buffer_size(NULL, AUDIO_DST_CHANNEL_COUNTS,m_nbSamples, DST_SAMPLT_FORMAT, 1);

        LOGI("audioDecoder", "AudioDecoder::OnDecoderReady [m_nbSamples, m_DstFrameDataSze]=[%d, %d]", m_nbSamples, m_DstFrameDataSze);

        m_AudioOutBuffer = (uint8_t *) malloc(m_DstFrameDataSze);

        m_AudioRender->Init();

    } else {
        LOGI("audioDecoder", "AudioDecoder::OnDecoderReady m_AudioRender == null");
    }
}

void AudioDecoder::onDecodeDone() {
    if(m_AudioRender)
        m_AudioRender->UnInit();

    if(m_AudioOutBuffer) {
        free(m_AudioOutBuffer);
        m_AudioOutBuffer = nullptr;
    }

    if(m_SwrContext) {
        swr_free(&m_SwrContext);
        m_SwrContext = nullptr;
    }
}

void AudioDecoder::setAudioRender(AudioRender *audioRender) {
    m_AudioRender = audioRender;
}


void AudioDecoder::ClearCache() {
    if(m_AudioRender)
        m_AudioRender->ClearAudioCache();
}

AudioDecoder::AudioDecoder(char *url) {
    init(url,AVMEDIA_TYPE_AUDIO);
}

void AudioDecoder::onFrameAvailable(AVFrame *frame) {
    if(m_AudioRender) {
        int result = swr_convert(m_SwrContext, &m_AudioOutBuffer, m_DstFrameDataSze / 2, (const uint8_t **) frame->data, frame->nb_samples);
        if (result > 0 ) {
            m_AudioRender->RenderAudioFrame(m_AudioOutBuffer, m_DstFrameDataSze);
        }
    }
}

