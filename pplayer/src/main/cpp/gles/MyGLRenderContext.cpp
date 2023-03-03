//
// Created by LPF on 2023/3/2.
//

#include "MyGLRenderContext.h"
#include "logUtils.h"

MyGLRenderContext* MyGLRenderContext::m_pContext = nullptr;

MyGLRenderContext::MyGLRenderContext() {

}

MyGLRenderContext::~MyGLRenderContext() {

}

MyGLRenderContext *MyGLRenderContext::GetInstance() {
    if (m_pContext == nullptr) {
        m_pContext = new MyGLRenderContext();
    }
    return m_pContext;
}

void MyGLRenderContext::DestroyInstance() {
    if (m_pContext)
    {
        delete m_pContext;
        m_pContext = nullptr;
    }
}

void MyGLRenderContext::OnSurfaceCreated() {
    LOGD(TAG, "MyGLRenderContext::OnSurfaceCreated");
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    LOGD(TAG, "最大可用的四分量顶点数目 %d ", nrAttributes)
//    triangleSample = new TriangleSample();
//    triangleSample->init();
//    doubleTriangleSample = new DoubleTriangleSample();
//    doubleTriangleSample->init();
    pictureSample = new PictureSample();
    pictureSample->init();
    LOGD(TAG, "MyGLRenderContext::OnSurfaceCreated Over");
}

void MyGLRenderContext::OnSurfaceChanged(int width, int height) {
    LOGD(TAG, "MyGLRenderContext::OnSurfaceChanged");
    glViewport(0,0,width,height);
    m_ScreenW = width;
    m_ScreenH = height;
    LOGD(TAG, "MyGLRenderContext::OnSurfaceChanged Over");
}

void MyGLRenderContext::OnDrawFrame() {
    glClear(GL_COLOR_BUFFER_BIT);
//    triangleSample->draw(m_ScreenW, m_ScreenH);
//    doubleTriangleSample->draw(m_ScreenW,m_ScreenH);
    pictureSample->draw(m_ScreenW, m_ScreenH);
}
