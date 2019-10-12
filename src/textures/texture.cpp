/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

#include <feign/textures/texture.h>

void Texture::preProcess()
{
    throw new NotImplementedException("texture preProcess");
}

std::string Texture::getName() const
{
    return "texture: ";
}
