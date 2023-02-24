//
// Created by LPF on 2023/2/23.
//

#ifndef FMPEG_AUDIOENGINE_H
#define FMPEG_AUDIOENGINE_H

#include <stdio.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <string>
#include <assert.h>
#include "../utils/logUtils.h"


class AudioEngine {
public:
    SLObjectItf engineObj;
    SLEngineItf engine;
    SLObjectItf outputMixObj;

    AudioEngine() : engineObj(nullptr), engine(nullptr),outputMixObj(nullptr) {
        createEngine();
    }

    virtual ~AudioEngine() {
        release();
    }

private:
    void createEngine() {
        //音频播放，涉及到 OpenSlES
        //第一大步：创建引擎并获取引擎接口
        //1.1创建引擎对象 SLObjectItf engineObj
        //参数一：  引擎对象地址，用于传出对象，是一个输出参数
        //参数二：  配置参数数量，传1
        //参数三：  配置参数，为枚举数组
        //参数四：  支持的接口数量
        //参数五：  具体的要支持的接口，是枚举的数组
        //参数六：  具体的要支持的接口是开放的还是关闭的，也是一个数组，后三个参数长度是一致的
        SLresult result = slCreateEngine(&engineObj, 0, nullptr, 0, nullptr, nullptr);
        checkSLResult(result, "slCreateEngine");
        //1.2初始化引擎
        result = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
        checkSLResult(result, "(*engineObj)->Realize");
        // 1.3 获取引擎接口 SLEngineItf engineInterface
        result = (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine);
        checkSLResult(result, "(*engineObj)->GetInterface");

        //  第二大步 设置混音器
        // 2.1 创建混音器：SLObjectItf outputMixObject
        result = (*engine)->CreateOutputMix(engine, &outputMixObj, 0, 0, 0);
        checkSLResult(result, "(*engine)->CreateOutputMix");
        // 2.2 初始化 混音器
        result = (*outputMixObj)->Realize(outputMixObj, SL_BOOLEAN_FALSE);
        checkSLResult(result, "(*engine)->Realize");
    }
    virtual void release() {
        if (outputMixObj) {
            (*outputMixObj)->Destroy(outputMixObj);
            outputMixObj = nullptr;
        }
        if (engineObj) {
            (*engineObj)->Destroy(engineObj);
            engineObj = nullptr;
            engine = nullptr;
        }
    }

    inline void checkSLResult(int code, std::string msg) {
        if (code != SL_RESULT_SUCCESS) {
            LOGE("checkSLResult", "msg -> %s", msg.c_str())
            assert(SL_RESULT_SUCCESS == code);
        }
    }

};

#endif //FMPEG_AUDIOENGINE_H
