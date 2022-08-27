//
// Created by Mr_Lee on 2022/6/18.
//

#ifndef FMPEG_VIDEORENDER_H
#define FMPEG_VIDEORENDER_H

#include "../../../utils/NativeImage.h"

#define VIDEO_RENDER_OPENGL             0
#define VIDEO_RENDER_AN_WINDOW           1

class VideoRender {
public:
    VideoRender(int type) {
        mRenderType = type;
    };
    virtual ~VideoRender(){}

    virtual void init(int videoWidth, int videoHeight, int *dstSize) = 0;
    virtual void renderVideoFrame(NativeImage *pImage) = 0;
    virtual void unInit() = 0;

    int getRenderType() {
        return mRenderType;
    }
private:

    int mRenderType = VIDEO_RENDER_AN_WINDOW;
};


#endif //FMPEG_VIDEORENDER_H
