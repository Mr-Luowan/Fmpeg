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
#include "MyAssetManager.h"
#include "logUtils.h"

class GLTools {

public:

    static unsigned int createTexture() {
        unsigned int textureIDs[1] = {0};
        glGenTextures(1, textureIDs);
        glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        return textureIDs[0];
    }

    static int initProgram(const char *vshFileName, const char *fragFileName) {
        int success;
        char infoLog[512];
        int program = glCreateProgram();
        int vshShader = initShader(vshFileName, GL_VERTEX_SHADER);
        int fragShader = initShader(fragFileName, GL_FRAGMENT_SHADER);
        glAttachShader(program, vshShader);
        glAttachShader(program, fragShader);
        glLinkProgram(program);
        glLinkProgram(program);
        // check for linking errors
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            LOGE(TAG, "ERROR::SHADER::PROGRAM::LINKING_FAILED %s", infoLog)
        }
        glDeleteShader(vshShader);
        glDeleteShader(fragShader);
        return program;
    }

private:

    static int initShader(const char *fileName, int type) {
        int shader = glCreateShader(type);
        std::string code;
        MyAssetManager::getInstance()->readAssetFile(fileName, code);
        const char* c_sCode = code.c_str();
        glShaderSource(shader, 1, &c_sCode, 0);
        glCompileShader(shader);
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            char const *typeStr = "GL_VERTEX_SHADER";
            if (type == GL_FRAGMENT_SHADER) {
                typeStr = "GL_FRAGMENT_SHADER";
            }
            LOGE(TAG, "ERROR:: %s SHADER::initShader::COMPILATION_FAILED %s", typeStr, infoLog)
        }
        return shader;
    }

};


#endif //FMPEG_GLTOOLS_H
