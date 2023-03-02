//
// Created by LPF on 2023/3/2.
//

#ifndef FMPEG_TRIANGLESAMPLE_H
#define FMPEG_TRIANGLESAMPLE_H


class TriangleSample {
private:

    unsigned int shaderProgram;

    unsigned int VAO;

public:
    TriangleSample();

    virtual ~TriangleSample();

    void init();

    void draw(int width, int height);

};


#endif //FMPEG_TRIANGLESAMPLE_H
