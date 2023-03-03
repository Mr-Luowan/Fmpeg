//
// Created by LPF on 2023/3/2.
//

#ifndef FMPEG_MYGLRENDERCONTEXT_H
#define FMPEG_MYGLRENDERCONTEXT_H
#include <GLES3/gl3.h>
#include "TriangleSample.h"
#include "DoubleTriangleSample.h"

class MyGLRenderContext {

private:
    static MyGLRenderContext *m_pContext;
    int m_ScreenW;
    int m_ScreenH;
    TriangleSample* triangleSample;
//    DoubleTriangleSample* doubleTriangleSample;

public:


    MyGLRenderContext();

    ~MyGLRenderContext();

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    static MyGLRenderContext* GetInstance();

    static void DestroyInstance();


};


#endif //FMPEG_MYGLRENDERCONTEXT_H
