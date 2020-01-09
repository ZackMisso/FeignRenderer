/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/texture.h>

ImageTexture::ImageTexture(std::string filename)
    : filename(filename), initialized(false) { }

void ImageTexture::preProcess()
{
    image = Imagef(filename);
    // TODO: convert to grayscale
    initialized = true;
}

Color3f ImageTexture::evaluate(const Point2f& point)
{
    assert(initialized);

    return Color3f(image(point[0], point[1], 0),
                   image(point[0], point[1], 1),
                   image(point[0], point[1], 2));
}
