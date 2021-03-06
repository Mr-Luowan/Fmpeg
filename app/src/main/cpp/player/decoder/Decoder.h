//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_DECODER_H
#define FMPEG_DECODER_H
typedef void (*MessageCallback)(void *, int, float);
typedef long (*AVSyncCallback)(void *);

class Decoder {
public:
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual float getDuration() = 0;
    virtual void seekToPosition(float position) = 0;
    virtual float getCurrentPosition() = 0;
    virtual void setMessageCallback(void *context, MessageCallback callback) = 0;
};
#endif //FMPEG_DECODER_H
