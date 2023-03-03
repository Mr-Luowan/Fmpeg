//
// Created by LPF on 2023/3/3.
//

#ifndef FMPEG_MYASSETMANAGER_H
#define FMPEG_MYASSETMANAGER_H

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <string>
#include "logUtils.h"
#include "EnvManager.h"

class MyAssetManager {
private:
    static MyAssetManager *navAssetManager_Instance;

    AAssetManager *assetManager;

    MyAssetManager() {};

    ~MyAssetManager() {};

public:
    static MyAssetManager *getInstance() {
        if (navAssetManager_Instance == nullptr) {
            navAssetManager_Instance = new MyAssetManager();
        }
        return navAssetManager_Instance;
    };

    void setAssetManager(AAssetManager *manager);

    void readAssetFile(const char* fileName, std::string &content);
};



#endif //FMPEG_MYASSETMANAGER_H
