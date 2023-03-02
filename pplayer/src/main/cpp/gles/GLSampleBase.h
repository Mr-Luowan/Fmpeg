//
// Created by LPF on 2023/3/2.
//

#ifndef FMPEG_GLSAMPLEBASE_H
#define FMPEG_GLSAMPLEBASE_H

class GLSampleBase {

protected:
    unsigned int m_iProgramObj;
    int m_SurfaceWidth;
    int m_SurfaceHeight;

public:
    GLSampleBase() :
            m_iProgramObj(-1),
            m_SurfaceWidth(-1),
            m_SurfaceHeight(-1) {}

    virtual void init() = 0;

    virtual void draw(int width, int height) = 0;

    virtual void destroy() = 0;
};

#endif //FMPEG_GLSAMPLEBASE_H
