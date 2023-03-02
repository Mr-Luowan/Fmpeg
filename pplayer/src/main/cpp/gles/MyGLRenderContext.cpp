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
    LOGD(TAG, "GetInstance");
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
    glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
    triangleSample = new TriangleSample();

    LOGD(TAG, "MyGLRenderContext::OnSurfaceCreated Over");
}

void MyGLRenderContext::OnSurfaceChanged(int width, int height) {
    LOGD(TAG, "MyGLRenderContext::OnSurfaceChanged");
    glViewport(0,0,width,height);
    m_ScreenW = width;
    m_ScreenH = height;
}

void MyGLRenderContext::OnDrawFrame() {
    LOGD(TAG, "MyGLRenderContext::OnDrawFrame");
    if (triangleSample) {
        triangleSample->init();
    }
    //triangleSample->draw(m_ScreenW, m_ScreenH);
}
