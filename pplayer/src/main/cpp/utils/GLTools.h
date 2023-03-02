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

class GLTools {

public:

    static unsigned int createOESTexture() {
        unsigned int textureIDs[1] = {0};
        glGenTextures(1,textureIDs);
        return textureIDs[0];
    }
    static int initProgram(const char *vshSource, const char *fragSource) {
        int program = glCreateProgram();
        int vshShader = initShader(vshSource, GL_VERTEX_SHADER);
        int fragShader = initShader(fragSource, GL_FRAGMENT_SHADER);
        glAttachShader(program, vshShader);
        glAttachShader(program, fragShader);
        glLinkProgram(program);
        return program;
    }

private:
    static int initShader(const char *source, int type) {
        int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        return shader;
    }

};


#endif //FMPEG_GLTOOLS_H
