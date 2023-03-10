//
// Created by LPF on 2023/3/10.
//

#include "AudioDecoder.h"

AudioDecoder::~AudioDecoder() {

}

void AudioDecoder::start() {
    DecoderBase::start();
}

void AudioDecoder::pause() {
    DecoderBase::pause();
}

void AudioDecoder::stop() {
    DecoderBase::stop();
}

float AudioDecoder::getDuration() {
    return DecoderBase::getDuration();
}

void AudioDecoder::seekToPosition(float position) {
    DecoderBase::seekToPosition(position);
}

float AudioDecoder::getCurrentPosition() {
    return DecoderBase::getCurrentPosition();
}

void AudioDecoder::setMessageCallback(void *context, MessageCallback callback) {
    DecoderBase::setMessageCallback(context, callback);
}

int AudioDecoder::init(const char *url, AVMediaType avMediaType) {
    return DecoderBase::init(url, avMediaType);
}

void AudioDecoder::unInit() {
    DecoderBase::unInit();
}

void AudioDecoder::onDecodeReady() {

}

void AudioDecoder::onDecodeDone() {

}

