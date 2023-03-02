//
// Created by LPF on 2023/3/2.
//

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include "logUtils.h"
#include "TriangleSample.h"

void TriangleSample::init() {
    FUN_BEGIN_TIME("TriangleSample::init")
    const char *vertexShaderSource = "#version 300 es\n"
                                     "layout (location = 0) in vec4 vPosition;\n"
                                     "void main() {\n"
                                     "     gl_Position  = vPosition;\n"
                                     "     gl_PointSize = 10.0;\n"
                                     "}\n";
    const char *fragmentShaderSource = "#version 300 es\n"
                                       "precision mediump float;\n"
                                       "out vec4 fragColor;\n"
                                       "void main() {\n"
                                       "     fragColor = vec4(1.0,1.0,1.0,1.0);\n"
                                       "}\n";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOGD(TAG, "ERROR::SHADER::glGetShaderiv::COMPILATION_FAILED %s", infoLog)
        return;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOGD(TAG, "ERROR::SHADER::glGetShaderiv::COMPILATION_FAILED %s", infoLog)
        return;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        LOGD(TAG, "ERROR::SHADER::PROGRAM::LINKING_FAILED %s", infoLog)
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f, 0.5f, 0.0f  // top
    };
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    //生成一个顶点缓冲对象
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    //新创建的缓冲绑定到GL_ARRAY_BUFFER目标
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //把用户定义的数据复制到当前绑定缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    FUN_END_TIME("TriangleSample::init");
}

void TriangleSample::draw(int width, int height) {
    if(shaderProgram < 0){
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

TriangleSample::TriangleSample() : shaderProgram(0), VAO(0) {}

TriangleSample::~TriangleSample() {

}
