//
// Created by lipengfei6 on 2023/3/30.
//

#ifndef FMPEG_GUIDE_LINE_H
#define FMPEG_GUIDE_LINE_H
#include "GLSampleBase.h"
#include <iostream>
#include <vector>

class guide_line : public GLSampleBase{
private:
    std::vector<float> vec;
    unsigned int VAO;
public:
    virtual void init();

    virtual void draw(int width, int height);

    virtual void destroy();

    void OnSurfaceChanged(int width, int height) override;
};


#endif //FMPEG_GUIDE_LINE_H
