//
// Created by LPF on 2023/3/9.
//

#ifndef FMPEG_NV21PICSAMPLE_H
#define FMPEG_NV21PICSAMPLE_H
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
//矩阵运算库
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "GLSampleBase.h"
#include "GLTools.h"

class NV21PicSample: public GLSampleBase{

public:
    virtual void init() override;

    virtual void draw(int width, int height) override;

    virtual void destroy() override;

    void OnSurfaceChanged(int width, int height) override;

    void LoadImage(_tag_NativeImage *pImage) override;

private:
    NativeImage m_RenderImage;
};


#endif //FMPEG_NV21PICSAMPLE_H
