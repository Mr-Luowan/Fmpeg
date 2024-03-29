//
// Created by lipengfei6 on 2023/3/30.
//

#include "guide_line.h"
#include "GLTools.h"
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include "logUtils.h"

void guide_line::init() {
    m_iProgramObj = GLTools::initProgram("glsl/guide_line.vsh", "glsl/guide_line.fsh");


}

void guide_line::draw(int width, int height) {
    glUseProgram(m_iProgramObj);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vec.size());
}

void guide_line::destroy() {

}

void guide_line::OnSurfaceChanged(int width, int height) {
    GLSampleBase::OnSurfaceChanged(width, height);
    for (int i = 0; i <= m_SurfaceWidth; i += m_SurfaceWidth / 20) {
        float pointY = ((float) i / (float) m_SurfaceWidth * 2) - 1.0F;
        vec.push_back(-1.0F);
        vec.push_back(pointY);
        vec.push_back(1.0F);
        vec.push_back(pointY);
    }
    for (int i = 0; i <= m_SurfaceHeight; i += m_SurfaceHeight / 20) {
        float pointX = ((float) i / (float) m_SurfaceHeight * 2) - 1.0F;
        vec.push_back(pointX);
        vec.push_back(-1.0F);
        vec.push_back(pointX);
        vec.push_back(1.0F);
    }
    float vertices[vec.size()];
    memcpy(vertices, &vec[0], vec.size() * sizeof(vec[0]));
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
}
