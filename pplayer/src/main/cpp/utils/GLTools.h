//
// Created by LPF on 2023/2/24.
//

#ifndef FMPEG_GLTOOLS_H
#define FMPEG_GLTOOLS_H

#include <jni.h>
#include <Egl/egl.h>
#include <Egl/eglext.h>
//使用 OpenGL ES 2.0 需要添加的头文件
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "logUtils.h"

class GLTools {

public:

    static unsigned int createOESTexture() {
        unsigned int textureIDs[1] = {0};
        glGenTextures(1, textureIDs);
        return textureIDs[0];
    }

    static int initProgram(const char *vshSource, const char *fragSource) {
        int success;
        char infoLog[512];
        int program = glCreateProgram();
        int vshShader = initShader(vshSource, GL_VERTEX_SHADER);
        int fragShader = initShader(fragSource, GL_FRAGMENT_SHADER);
        glAttachShader(program, vshShader);
        glAttachShader(program, fragShader);
        glLinkProgram(program);
        glLinkProgram(program);
        // check for linking errors
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            LOGD(TAG, "ERROR::SHADER::PROGRAM::LINKING_FAILED %s", infoLog)
        }
        glDeleteShader(vshShader);
        glDeleteShader(fragShader);
        return program;
    }

private:
    static int getShaderFromFile(const char *vertexPath, const char *fragmentPath) {
        // 1. 从文件路径中获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // 保证ifstream对象可以抛出异常：
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        try {
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件的缓冲内容到数据流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到string
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            LOGE(TAG, "error read shader from file")
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        LOGE(TAG, "vertex code = %s", vShaderCode)
        LOGE(TAG, "fragment code = %s", fShaderCode)
    }

    static int initShader(const char *source, int type) {
        int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            char *typeStr = "GL_VERTEX_SHADER";
            if (type == GL_FRAGMENT_SHADER) {
                typeStr = "GL_FRAGMENT_SHADER";
            }
            LOGD(TAG, "ERROR:: %s SHADER::initShader::COMPILATION_FAILED %s", typeStr, infoLog)
        }
        return shader;
    }

};


#endif //FMPEG_GLTOOLS_H
