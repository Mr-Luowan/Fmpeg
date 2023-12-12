//
// Created by Mr_Lee on 2022/6/18.
//

#include <jni.h>
#include "NativeRender.h"


NativeRender::NativeRender(JNIEnv *env, jobject surface) : VideoRender(VIDEO_RENDER_AN_WINDOW) {
    //1. 利用 Java 层 SurfaceView 传下来的 Surface 对象，获取 ANativeWindow
    mNativeWindow = ANativeWindow_fromSurface(env, surface);
}

NativeRender::~NativeRender() {
    if (mNativeWindow) {
        ANativeWindow_release(mNativeWindow);
    }
}

void NativeRender::init(int videoWidth, int videoHeight, int *dstSize) {
    if (mNativeWindow == nullptr) {
        return;
    }
    int windowWidth = ANativeWindow_getWidth(mNativeWindow);
    int windowHeight = ANativeWindow_getHeight(mNativeWindow);
    if (windowWidth < windowHeight * videoWidth / videoHeight) {
        mDstWidth = windowWidth;
        mDstHeight = windowWidth * videoHeight / videoWidth;
    } else {
        mDstWidth = windowHeight * videoWidth / videoHeight;
        mDstHeight = windowHeight;
    }
    //2. 设置渲染区域和输入格式
    ANativeWindow_setBuffersGeometry(
            mNativeWindow,
            mDstWidth,
            mDstHeight,
            WINDOW_FORMAT_RGBA_8888
    );
    dstSize[0] = mDstWidth;
    dstSize[1] = mDstHeight;
}

void NativeRender::renderVideoFrame(NativeImage *pImage) {
    if (mNativeWindow == nullptr || pImage == nullptr) {
        return;
    }
    //3. 渲染  锁定当前 Window ，获取屏幕缓冲区 Buffer 的指针
    ANativeWindow_lock(mNativeWindow, &mNativeWindowBuffer, nullptr);
    uint8_t *dstBuffer = static_cast<uint8_t *>(mNativeWindowBuffer.bits);

    //RGBA
    int srcLineSize = pImage->width * 4;    //输入图的步长（一行像素有多少字节）
    int dstLineSize = mNativeWindowBuffer.stride * 4;  //RGBA 缓冲区步长
    for (int i = 0; i < mDstHeight; ++i) {
        //一行一行地拷贝图像数据
        memcpy(dstBuffer + i * dstLineSize, pImage->ppPlane[0] + i * srcLineSize, srcLineSize);
    }
    //解锁当前 Window ，渲染缓冲区数据
    ANativeWindow_unlockAndPost(mNativeWindow);
}

void NativeRender::unInit() {

}
