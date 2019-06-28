#include <feign/textures/texture.h>

void Texture::preProcess()
{
    throw new NotImplementedException("texture preProcess");
}

std::string Texture::getName() const
{
    return "texture: ";
}
