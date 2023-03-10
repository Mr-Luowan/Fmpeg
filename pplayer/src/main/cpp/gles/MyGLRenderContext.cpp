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
    glSampleBase->init();
    LOGD(TAG, "MyGLRenderContext::OnSurfaceCreated Over");
}

void MyGLRenderContext::OnSurfaceChanged(int width, int height) {
    LOGD(TAG, "MyGLRenderContext::OnSurfaceChanged");
    glViewport(0,0,width,height);
    m_ScreenW = width;
    m_ScreenH = height;
    glSampleBase->OnSurfaceChanged(width, height);
    LOGD(TAG, "MyGLRenderContext::OnSurfaceChanged Over");
}

void MyGLRenderContext::OnDrawFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glSampleBase->draw(m_ScreenW, m_ScreenH);
}

void MyGLRenderContext::switchContent(int type) {
    switch (type) {
        case 0:{
            glSampleBase = new DoubleTriangleSample();
            break;
        }
        case 1:{
            glSampleBase = new PictureSample();
            break;
        }
        case 2:{
            break;
        }
        case 3:{
            glSampleBase = new NV21PicSample();
            break;
        }
    }
}

void MyGLRenderContext::setImageData(int format, int width, int height, uint8_t *imgData) {
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = imgData;
    switch (format)
    {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }
    if (glSampleBase) {
        glSampleBase->LoadImage(&nativeImage);
    }

}

void MyGLRenderContext::updateTransformMatrix(float angleX, float angleY, float scaleX,
                                              float scaleY) {
    if (glSampleBase) {
        glSampleBase->updateTransformMatrix(angleX, angleY, scaleX, scaleY);
    }
}
