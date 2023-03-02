//
// Created by LPF on 2023/3/2.
//

#include "DoubleTriangleSample.h"
#include "GLTools.h"
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

DoubleTriangleSample::DoubleTriangleSample() {}
void DoubleTriangleSample::init() {
    float vertices[] = {
            -0.5f,-0.2f,
            -0.25f,0.3f,
            0.0f,-0.2f,
            0.25f,0.3f,
            0.5f,-0.2f
    };
    unsigned int indices[] = {0,1,2,2,3,4};

    FUN_BEGIN_TIME("TriangleSample::init")
        const char *vertexShaderSource = "#version 300 es\n"
                                         "layout (location = 0) in vec2 vPosition;\n"
                                         "void main() {\n"
                                         "     gl_Position  = vec4(vPosition, 0.0f, 1.0f);\n"
                                         "}\n";
        const char *fragmentShaderSource = "#version 300 es\n"
                                           "precision mediump float;\n"
                                           "out vec4 fragColor;\n"
                                           "void main() {\n"
                                           "     fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                           "}\n";
        m_iProgramObj = GLTools::initProgram(vertexShaderSource, fragmentShaderSource);

        unsigned int VBO;
        unsigned int EBO;
        //生成一个顶点数组对象
        glGenVertexArrays(1, &VAO);
        //生成一个索引缓冲对象
        glGenBuffers(1, &EBO);
        //生成一个顶点缓冲对象
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        //新创建的缓冲绑定到GL_ARRAY_BUFFER目标
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //把用户定义的数据复制到当前绑定缓冲
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    FUN_END_TIME("TriangleSample::init");
}

void DoubleTriangleSample::draw(int width, int height) {
    glUseProgram(m_iProgramObj);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DoubleTriangleSample::destroy() {

}
