//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_VIDEODECODER_H
#define FMPEG_VIDEODECODER_H

#include <render/video/VideoRender.h>
#include "DecoderBase.h"
#include "../../utils/logUtils.h"

extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/jni.h>
};

class VideoDecoder: public DecoderBase{
public:
    VideoDecoder(char *url) {
        init(url, AVMEDIA_TYPE_VIDEO);
    };

    virtual ~VideoDecoder() {
        unInit();
    }
    int getVideoWidth()
    {
        return mVideoWidth;
    }
    int getVideoHeight()
    {
        return mVideoHeight;
    }

    void setVideoRender(VideoRender *videoRender)
    {
        mVideoRender = videoRender;
    }


private:
    virtual void onDecodeReady();

    virtual void onDecodeDone();

    virtual void onFrameAvailable(AVFrame *frame);

    const AVPixelFormat DST_PIXEL_FORMAT = AV_PIX_FMT_RGBA;

    int mVideoWidth = 0;
    int mVideoHeight = 0;

    int mRenderWidth = 0;
    int mRenderHeight = 0;

    AVFrame *mRGBAFrame = nullptr;
    uint8_t *mFrameBuffer = nullptr;

    VideoRender *mVideoRender = nullptr;
    SwsContext *mSwsContext = nullptr;
//    SingleVideoRecorder *m_pVideoRecorder = nullptr;
};


#endif //FMPEG_VIDEODECODER_H
