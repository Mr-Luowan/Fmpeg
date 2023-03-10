//
// Created by LPF on 2023/3/3.
//

#ifndef FMPEG_PICTURESAMPLE_H
#define FMPEG_PICTURESAMPLE_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
//矩阵运算库
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "GLSampleBase.h"
#include "GLTools.h"
#include "MyAssetManager.h"
#include "stb_image.h"

class PictureSample : public GLSampleBase {
public:
    virtual void init();

    virtual void draw(int width, int height);

    virtual void destroy();

    void OnSurfaceChanged(int width, int height) override;

    void updateTransformMatrix(float angleX, float angleY, float scaleX, float scaleY) override;

private:
    int m_iAngleX;
    int m_iAngleY;
    float m_fScaleX = 1.0f;
    float m_fScaleY = 1.0f;
};


#endif //FMPEG_PICTURESAMPLE_H
