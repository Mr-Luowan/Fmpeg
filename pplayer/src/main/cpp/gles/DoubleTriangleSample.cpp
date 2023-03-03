//
// Created by LPF on 2023/3/2.
//

#include "DoubleTriangleSample.h"
#include "GLTools.h"
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <math.h>

DoubleTriangleSample::DoubleTriangleSample() {}

void DoubleTriangleSample::init() {
    //0-2位置  3-5颜色
    float vertices[] = {
            -0.5f, -0.2f, 1.0f, 0.0f, 0.0f,
            -0.25f, 0.3f, 0.0f, 1.0f, 0.0f,
            0.0f, -0.2f, 0.0f, 0.0f, 1.0f,
            0.25f, 0.3f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.2f, 1.0f, 0.0f, 0.0f,
    };
    unsigned int indices[] = {0, 1, 2, 2, 3, 4};

    FUN_BEGIN_TIME("TriangleSample::init")
        m_iProgramObj = GLTools::initProgram("doubleTriangle.vsh", "doubleTriangle.fsh");
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

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
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
