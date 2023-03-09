//
// Created by LPF on 2023/3/9.
//

#include "NV21PicSample.h"

int m_ySamplerLoc;
int m_uvSamplerLoc;
unsigned int m_yTextureId;
unsigned int m_uvTextureId;

void NV21PicSample::init() {
    m_iProgramObj = GLTools::initProgram("glsl/nv21.vsh", "glsl/nv21.fsh");
    GLfloat verticesCoords[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
    };
    unsigned int VBO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_yTextureId = GLTools::createTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width, m_RenderImage.height, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    m_uvTextureId = GLTools::createTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, m_RenderImage.width >> 1,
                 m_RenderImage.height >> 1, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
                 m_RenderImage.ppPlane[1]);
    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(m_iProgramObj);
    m_ySamplerLoc = glGetUniformLocation(m_iProgramObj, "y_texture");
    m_uvSamplerLoc = glGetUniformLocation(m_iProgramObj, "uv_texture");
    glUniform1i(m_ySamplerLoc, 0);
    glUniform1i(m_uvSamplerLoc, 1);
}

void NV21PicSample::draw(int width, int height) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_yTextureId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_uvTextureId);

    glUseProgram(m_iProgramObj);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void NV21PicSample::destroy() {

}

void NV21PicSample::OnSurfaceChanged(int width, int height) {
    GLSampleBase::OnSurfaceChanged(width, height);
}

void NV21PicSample::LoadImage(_tag_NativeImage *pImage) {
    GLSampleBase::LoadImage(pImage);
    GLSampleBase::LoadImage(pImage);
    if (pImage) {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }
}
