/**
* Author:    Zackary Misso
* Version:   0.2.0
*
* Anyone has permission to use the following code as long as proper
* acknowledgement is provided to the original author(s).
**/

#include <feign/core/texture.h>

FEIGN_BEGIN()

ImageTexture::ImageTexture(std::string filename, Vec3f scale)
    : filename(filename), scale(scale) { }

ImageTexture::~ImageTexture()
{
    image.clear();
}

// this method will always be callsed by fr_texture
void ImageTexture::preProcess()
{
    image = Imagef(filename);
}

Color3f ImageTexture::evaluate(const Point2f& point)
{
    Point2f sample;

    sample[0] = bound(point(0), 0.f, 1.f);
    sample[1] = bound(point(1), 0.f, 1.f);

    sample[0] *= image.width();
    sample[1] *= image.height();

    return Color3f(image(sample(0), sample(1), 0),
                   image(sample(0), sample(1), 1),
                   image(sample(0), sample(1), 2)) * scale;
}

FEIGN_END()
