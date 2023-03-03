//
// Created by LPF on 2023/3/3.
//

#ifndef FMPEG_PICTURESAMPLE_H
#define FMPEG_PICTURESAMPLE_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include "GLSampleBase.h"
#include "GLTools.h"
#include "MyAssetManager.h"
#include "stb_image.h"

class PictureSample : public GLSampleBase {
public:
    virtual void init();

    virtual void draw(int width, int height);

    virtual void destroy();
};


#endif //FMPEG_PICTURESAMPLE_H
