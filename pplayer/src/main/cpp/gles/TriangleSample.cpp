//
// Created by LPF on 2023/3/2.
//

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include "logUtils.h"
#include "TriangleSample.h"
#include "GLTools.h"

#define VERTEX_LENGTH (3)

void TriangleSample::init() {
    FUN_BEGIN_TIME("TriangleSample::init")
        shaderProgram = GLTools::initProgram("Triangle.vsh", "Triangle.fsh");
        float vertices[] = {
                -0.5f, -0.5f, 0.0f, // left bottom
                0.5f, -0.5f, 0.0f, // right bottom
                -0.5f, 0.5f, 0.0f,  // left top
                0.5f, 0.5f, 0.0f  //right top
        };
        unsigned int indices[] = {
                0, 1, 2,
                1, 2, 3
        };
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    FUN_END_TIME("TriangleSample::init");
}

void TriangleSample::draw(int width, int height) {
    if (shaderProgram < 0) {
        return;
    }

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //第二个参数 这里的6是绘制顶点的数量  第四个参数是偏移量
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
}

TriangleSample::TriangleSample() : shaderProgram(-1), VAO(-1) {}

TriangleSample::~TriangleSample() {

}
