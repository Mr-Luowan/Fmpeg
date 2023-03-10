//
// Created by LPF on 2023/3/10.
//

#ifndef FMPEG_AUDIODECODER_H
#define FMPEG_AUDIODECODER_H
#include "DecoderBase.h"

extern "C" {
#include <libavutil/avutil.h>
};

class AudioDecoder: public DecoderBase {
public:
    virtual ~AudioDecoder();

    virtual void start();

    virtual void pause();

    virtual void stop();

    virtual float getDuration();

    virtual void seekToPosition(float position);

    virtual float getCurrentPosition();

    virtual void setMessageCallback(void *context, MessageCallback callback);

protected:
    virtual int init(const char *url, AVMediaType avMediaType);

    virtual void unInit();

    virtual void onDecodeReady();

    virtual void onDecodeDone();

};


#endif //FMPEG_AUDIODECODER_H
