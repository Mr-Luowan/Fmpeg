//
// Created by LPF on 2023/3/13.
//

#include "VideoGLRender.h"
#include "GLTools.h"

VideoGLRender::~VideoGLRender() {

}

void VideoGLRender::init(int videoWidth, int videoHeight, int *dstSize) {
    dstSize[0] = videoWidth;
    dstSize[1] = videoHeight;
}

void VideoGLRender::renderVideoFrame(NativeImage *pImage) {
    std::unique_lock<std::mutex> lock(m_Mutex);
    if (m_RenderImage.ppPlane[0] != nullptr) {
        NativeImageUtil::FreeNativeImage(&m_RenderImage);
    }
    memset(&m_RenderImage,0, sizeof(NativeImage));
    m_RenderImage.format = pImage->format;
    m_RenderImage.width = pImage->width;
    m_RenderImage.height = pImage->height;
    NativeImageUtil::AllocNativeImage(&m_RenderImage);
    NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
}

void VideoGLRender::unInit() {

}

void VideoGLRender::init() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    m_iProgramObj = GLTools::initProgram("glsl/video.vsh", "glsl/video.fsh");
    float vertices[] = {
            //position  //texture
            0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f
    };
    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    unsigned int VBO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void VideoGLRender::draw(int width, int height) {
    std::unique_lock<std::mutex> lock(m_Mutex);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
                 m_RenderImage.width, m_RenderImage.height,
                 0, GL_RGBA,GL_UNSIGNED_BYTE,
                 m_RenderImage.ppPlane[0]);
    lock.unlock();
    glUseProgram(m_iProgramObj);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void VideoGLRender::destroy() {

}

void VideoGLRender::OnSurfaceChanged(int width, int height) {
    GLSampleBase::OnSurfaceChanged(width, height);
    glViewport(0,0,width, height);
}

void VideoGLRender::updateTransformMatrix(float angleX, float angleY, float scaleX, float scaleY) {
    GLSampleBase::updateTransformMatrix(angleX, angleY, scaleX, scaleY);
}

void VideoGLRender::LoadImage(_tag_NativeImage *pImage) {
    GLSampleBase::LoadImage(pImage);
}
