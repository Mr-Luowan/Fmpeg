//
// Created by LPF on 2023/3/2.
//

#ifndef FMPEG_DOUBLETRIANGLESAMPLE_H
#define FMPEG_DOUBLETRIANGLESAMPLE_H
#include "GLSampleBase.h"
/**
 * 使用VAO VBO 绘制两个相连的三角形
 */
class DoubleTriangleSample: public GLSampleBase{
private:
    unsigned int VAO;
public:
    DoubleTriangleSample();

    virtual void init();

    virtual void draw(int width, int height);

    virtual void destroy();
};


#endif //FMPEG_DOUBLETRIANGLESAMPLE_H
