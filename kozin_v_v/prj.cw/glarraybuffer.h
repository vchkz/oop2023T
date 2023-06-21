#ifndef GLARRAYBUFFER_H
#define GLARRAYBUFFER_H

#include <glad/glad.h>


class GlArrayBuffer {

public:


private:
    GLuint _index;

public:

    GlArrayBuffer();


    void init();


    void bind();


    void render();


    void unbind();

};

#endif // GLARRAYBUFFER_H
