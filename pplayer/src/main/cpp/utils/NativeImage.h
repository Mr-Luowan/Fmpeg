//
// Created by Mr_Lee on 2022/6/19.
//

#ifndef FMPEG_NATIVEIMAGE_H
#define FMPEG_NATIVEIMAGE_H

#define IMAGE_FORMAT_RGBA           0x01
#define IMAGE_FORMAT_NV21           0x02
#define IMAGE_FORMAT_NV12           0x03
#define IMAGE_FORMAT_I420           0x04

#include <jni.h>

typedef struct _tag_NativeRectF
{
    float left;
    float top;
    float right;
    float bottom;
    _tag_NativeRectF()
    {
        left = top = right = bottom = 0.0f;
    }
} RectF;

typedef struct _tag_NativeImage
{
    int width;
    int height;
    int format;
    uint8_t *ppPlane[3];
    int pLineSize[3];
    _tag_NativeImage()
    {
        width = 0;
        height = 0;
        format = 0;
        ppPlane[0] = nullptr;
        ppPlane[1] = nullptr;
        ppPlane[2] = nullptr;
        pLineSize[0] = 0;
        pLineSize[1] = 0;
        pLineSize[2] = 0;
    }
} NativeImage;

#endif //FMPEG_NATIVEIMAGE_H
