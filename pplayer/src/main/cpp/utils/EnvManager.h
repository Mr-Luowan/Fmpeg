//
// Created by LPF on 2023/3/3.
//

#ifndef FMPEG_ENVMANAGER_H
#define FMPEG_ENVMANAGER_H
#include <jni.h>
#include "NativeImage.h"

class EnvManager {
private:
    static EnvManager *s_Instance;
    JavaVM *globalJVM = nullptr;
    EnvManager() {};
    ~EnvManager() {};
public:
    static EnvManager *getInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new EnvManager();
        }
        return s_Instance;
    }

    JavaVM *getGlobalJvm() const {
        return globalJVM;
    }

    void setGlobalJvm(JavaVM *globalJvm) {
        globalJVM = globalJvm;
    }

    JNIEnv *getCurrentEnv(int *attach) {
        if (globalJVM == nullptr) return nullptr;
        *attach = 0;
        JNIEnv *jni_env = nullptr;
        int result = globalJVM->GetEnv((void **) &jni_env, JNI_VERSION_1_6);
        if (result == JNI_EDETACHED || jni_env == nullptr) {
            result = globalJVM->AttachCurrentThread(&jni_env, nullptr);
            if (result < 0) {
                jni_env = nullptr;
            } else {
                *attach = 1;
            }
        }
        return jni_env;
    }
};


#endif //FMPEG_ENVMANAGER_H
