//
// Created by Mr_Lee on 2022/6/18.
//

#include "VideoDecoder.h"

void VideoDecoder::onDecodeReady() {
    LOGE(TAG, "VideoDecoder::onDecodeReady 开始渲染！")
    mVideoWidth = getCodecContext()->width;
    mVideoHeight = getCodecContext()->height;
    if (mMsgContext && messageCallback) {
        messageCallback(mMsgContext, MSG_DECODER_READY, 0);
    }
    if (mVideoRender != nullptr) {
        int dstSize[2] = {0};
        mVideoRender->init(mVideoWidth, mVideoHeight, dstSize);
        mRenderWidth = dstSize[0];
        mRenderHeight = dstSize[1];

        if (mVideoRender->getRenderType() == VIDEO_RENDER_AN_WINDOW) {
            int fps = 25;
            long videoBitRate = mRenderWidth * mRenderHeight * fps * 0.2;
        }
        mRGBAFrame = av_frame_alloc();
        int bufferSize = av_image_get_buffer_size(DST_PIXEL_FORMAT, mRenderWidth, mRenderHeight, 1);
        mFrameBuffer = static_cast<uint8_t *>(av_malloc(bufferSize * sizeof(u_int8_t)));
        av_image_fill_arrays(
                mRGBAFrame->data,
                mRGBAFrame->linesize,
                mFrameBuffer,
                DST_PIXEL_FORMAT,
                mRenderWidth,
                mRenderHeight,
                1
                );
        mSwsContext = sws_getContext(mVideoWidth, mVideoHeight, getCodecContext()->pix_fmt,
                                     mRenderWidth, mRenderHeight, DST_PIXEL_FORMAT,
                                     SWS_FAST_BILINEAR, NULL, NULL, NULL);

        LOGE(TAG, "初始化mSwsContext mVideoWidth ==> %d", mVideoWidth)
        LOGE(TAG, "初始化mSwsContext mVideoHeight ==> %d", mVideoHeight)
        if (getCodecContext() == nullptr) {
            LOGE(TAG, "初始化mSwsContext getCodecContext() is null")
        } else {
            LOGE(TAG, "初始化mSwsContext getCodecContext() getCodecContext()->pix_fmt ==》 %d", getCodecContext()->pix_fmt)
        }
        LOGE(TAG, "初始化mSwsContext mRenderWidth ==> %d", mRenderWidth)
        LOGE(TAG, "初始化mSwsContext mRenderHeight ==> %d", mRenderHeight)
        if (mSwsContext == nullptr) {
            LOGE(TAG, "初始化mSwsContext 失败")
        } else {
            LOGE(TAG, "初始化mSwsContext 成功")
        }
    } else {
        LOGE(TAG, "空指针异常！")
    }
}

void VideoDecoder::onDecodeDone() {

}

void VideoDecoder::onFrameAvailable(AVFrame *frame) {
    LOGI(TAG, "VideoDecoder::OnFrameAvailable frame=%p", frame)
    if (mVideoRender != nullptr && frame != nullptr) {
        NativeImage image;
        if (mVideoRender->getRenderType() == VIDEO_RENDER_AN_WINDOW) {
            sws_scale(mSwsContext, frame->data, frame->linesize, 0,
                      mVideoHeight, mRGBAFrame->data, mRGBAFrame->linesize);
            image.format = IMAGE_FORMAT_RGBA;
            image.width = mRenderWidth;
            image.height = mRenderHeight;
            image.ppPlane[0] = mRGBAFrame->data[0];
            image.pLineSize[0] = image.width * 4;
        }
        mVideoRender->renderVideoFrame(&image);
    }
    if(mMsgContext && messageCallback)
        messageCallback(mMsgContext, MSG_REQUEST_RENDER, 0);
}
