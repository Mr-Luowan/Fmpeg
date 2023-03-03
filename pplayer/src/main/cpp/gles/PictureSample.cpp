//
// Created by LPF on 2023/3/3.
//

#include "PictureSample.h"
#include "MyAssetManager.h"

unsigned int texture;

void PictureSample::init() {
    m_iProgramObj = GLTools::initProgram("texture.vsh", "texture.fsh");
    float vertices[] = {
            //position  //color //texture
            0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };
    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    unsigned int VBO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    long fileLength;
    unsigned char * fileData = MyAssetManager::getInstance()->readPicture("flower.jpg", fileLength);
    unsigned char* image = stbi_load_from_memory(fileData, fileLength, &width, &height, &nrChannels, 0);
    delete[] fileData;
    LOGE(TAG, "图片宽高 %d %d" , width, height)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete[] image;
}

void PictureSample::draw(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(m_iProgramObj);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void PictureSample::destroy() {

}
