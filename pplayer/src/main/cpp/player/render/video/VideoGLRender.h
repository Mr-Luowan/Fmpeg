//
// Created by LPF on 2023/3/13.
//

#ifndef FMPEG_VIDEOGLRENDER_H
#define FMPEG_VIDEOGLRENDER_H
#include "GLSampleBase.h"
#include "VideoRender.h"
#include <mutex>

class VideoGLRender: public VideoRender, public GLSampleBase{
public:
    VideoGLRender() : VideoRender(VIDEO_RENDER_OPENGL) {
    }
    ~VideoGLRender() override;

    void init(int videoWidth, int videoHeight, int *dstSize) override;

    void renderVideoFrame(NativeImage *pImage) override;

    void unInit() override;

    void init() override;

    void draw(int width, int height) override;

    void destroy() override;

    void OnSurfaceChanged(int width, int height) override;

    void updateTransformMatrix(float angleX, float angleY, float scaleX, float scaleY) override;

    void LoadImage(_tag_NativeImage *pImage) override;

private:
    unsigned int m_TextureID;
    NativeImage m_RenderImage;
    std::mutex m_Mutex;
};



#endif //FMPEG_VIDEOGLRENDER_H
