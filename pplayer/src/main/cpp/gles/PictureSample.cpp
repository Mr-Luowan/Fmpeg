//
// Created by LPF on 2023/3/3.
//

#include "PictureSample.h"
#include "MyAssetManager.h"

unsigned int texture;
unsigned int texture2;
glm::mat4 trans;

void PictureSample::init() {
    m_iProgramObj = GLTools::initProgram("texture.vsh", "texture.fsh");
    float vertices[] = {
            //position  //texture
            0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    long fileLength;
    unsigned char *fileData = MyAssetManager::getInstance()->readPicture("img/container.jpg",
                                                                         fileLength);
    unsigned char *image = stbi_load_from_memory(fileData, fileLength, &width, &height, &nrChannels,
                                                 0);
    LOGE(TAG, "图片宽高 %d %d", width, height)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    delete[] fileData;

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width2 = 0;
    int height2 = 0;
    int nrChannels2 = 0;
    long fileLength2;
    unsigned char *fileData2 = MyAssetManager::getInstance()->readPicture("img/awesomeface.png",
                                                                          fileLength2);
    unsigned char *image2 = stbi_load_from_memory(fileData2, fileLength2, &width2, &height2,
                                                  &nrChannels2, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    LOGE(TAG, "图片宽高 %d %d", width2, height2)
    stbi_image_free(image2);
    delete[] fileData2;

    glUseProgram(m_iProgramObj);
    glUniform1i(glGetUniformLocation(m_iProgramObj, "texture1"), 0);
    glUniform1i(glGetUniformLocation(m_iProgramObj, "texture2"), 1);

    //glEnable(GL_BLEND); // 开启颜色混合
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置颜色混合模式
}
int i = 0;
void PictureSample::draw(int width, int height) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    trans = glm::mat4(1.0f);
    if (i > 360) {
        i = i;
    }
    i++;
    trans = glm::rotate(trans, glm::radians(i * 1.0F), glm::vec3(0.0, 0.0, 1.0));
//    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    //第三个参数询问我们是否希望对我们的矩阵进行转置(Transpose)


    glUseProgram(m_iProgramObj);
    glUniformMatrix4fv(glGetUniformLocation(m_iProgramObj, "transform"),1, false, glm::value_ptr(trans));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void PictureSample::destroy() {

}
