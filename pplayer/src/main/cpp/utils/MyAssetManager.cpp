//
// Created by LPF on 2023/3/3.
//
#include "MyAssetManager.h"
#include "stb_image.h"
#include <cstdlib>


MyAssetManager *MyAssetManager::navAssetManager_Instance = nullptr;

void MyAssetManager::setAssetManager(AAssetManager *manager) {
    this->assetManager = manager;
}

void MyAssetManager::readAssetFile(const char *fileName, std::string &content) {
    int attach;
    JNIEnv *env = EnvManager::getInstance()->getCurrentEnv(&attach);
    if (env != nullptr) {
        if (this->assetManager == nullptr) {
            LOGE(TAG, "assetManager is null")
            return;
        }
        AAsset *dataAsset = AAssetManager_open(assetManager, fileName,
                                               AASSET_MODE_UNKNOWN);//get file read AAsset
        size_t dataBufferSize = AAsset_getLength(dataAsset);
        char *data = new char[dataBufferSize + 1];
        int numBytesRead = AAsset_read(dataAsset, data,
                                       dataBufferSize);  //begin to read data once time
        if (numBytesRead <= 0) {
            LOGE(TAG, "读取assets失败")
            content = nullptr;
        } else {
            data[dataBufferSize] = '\0';
            content = data;
            LOGI(TAG, "读取assets成功 %s", content.c_str())
        }
        AAsset_close(dataAsset);
        free(data);
    } else {
        LOGE(TAG, "env 获取失败")
    }
}

unsigned char * MyAssetManager::readPicture(const char *fileName, long &fileSize) {
    int attach;
    JNIEnv *env = EnvManager::getInstance()->getCurrentEnv(&attach);
    if (env != nullptr) {
        if (this->assetManager == nullptr) {
            LOGE(TAG, "assetManager is null")
            return nullptr;
        }
        AAsset *assetFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_UNKNOWN);
        fileSize  = AAsset_getLength(assetFile);
        auto* fileData = new unsigned char[fileSize];
        AAsset_read(assetFile, fileData, fileSize );
        AAsset_close(assetFile);
        return fileData;
    } else {
        LOGE(TAG, "env 获取失败")
    }
    return nullptr;
}

AAssetManager *MyAssetManager::getAssetManager() const {
    return assetManager;
}
