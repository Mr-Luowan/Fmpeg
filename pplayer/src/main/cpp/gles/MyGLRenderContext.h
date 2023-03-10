//
// Created by LPF on 2023/3/2.
//

#ifndef FMPEG_MYGLRENDERCONTEXT_H
#define FMPEG_MYGLRENDERCONTEXT_H
#include <GLES3/gl3.h>
#include "TriangleSample.h"
#include "DoubleTriangleSample.h"
#include "PictureSample.h"
#include "NV21PicSample.h"

class MyGLRenderContext {

private:
    static MyGLRenderContext *m_pContext;
    int m_ScreenW;
    int m_ScreenH;
    GLSampleBase* glSampleBase;

public:


    MyGLRenderContext();

    ~MyGLRenderContext();

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    static MyGLRenderContext* GetInstance();

    static void DestroyInstance();


    void switchContent(int type);

    void setImageData(int format, int width, int height, uint8_t *imgData);

    void updateTransformMatrix(float angleX, float angleY, float scaleX, float scaleY);
};


#endif //FMPEG_MYGLRENDERCONTEXT_H
