#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "texture.h"

#include "GL/glew.h"

namespace GL {

class Texture2D : public Texture {
public:
    Texture2D(unsigned char *data, int width, int height, GLenum type = GL_UNSIGNED_BYTE);

    virtual void bind() const override;
    virtual void unbind() const override;
};

}

#endif // TEXTURE2D_H