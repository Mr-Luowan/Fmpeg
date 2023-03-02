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
#include "logUtils.h"

class GLTools {

public:

    static unsigned int createOESTexture() {
        unsigned int textureIDs[1] = {0};
        glGenTextures(1,textureIDs);
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
    static int initShader(const char *source, int type) {
        int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            char * typeStr = "GL_VERTEX_SHADER";
            if (type == GL_FRAGMENT_SHADER){
                typeStr = "GL_FRAGMENT_SHADER";
            }
            LOGD(TAG, "ERROR:: %s SHADER::initShader::COMPILATION_FAILED %s", typeStr, infoLog)
        }
        return shader;
    }

};


#endif //FMPEG_GLTOOLS_H
