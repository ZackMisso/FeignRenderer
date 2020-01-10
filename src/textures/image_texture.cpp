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
    // TODO: convert to grayscale... maybe
    initialized = true;
}

Color3f ImageTexture::evaluate(const Point2f& point)
{
    assert(initialized);

    Point2f sample;

    sample[0] = bound(point(0), 0.f, 1.f);
    sample[1] = bound(point(1), 0.f, 1.f);

    sample[0] *= image.width();
    sample[1] *= image.height();

    return Color3f(image(sample(0), sample(1), 0),
                   image(sample(0), sample(1), 1),
                   image(sample(0), sample(1), 2));
}
