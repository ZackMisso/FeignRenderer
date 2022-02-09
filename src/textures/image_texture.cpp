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
    : filename(filename), scale(scale) {}

ImageTexture::~ImageTexture()
{
    // image.clear();
}

// this method will always be callsed by fr_texture
void ImageTexture::preProcess()
{
    image = Imagef(filename);
}

Color3f ImageTexture::evaluate(const Point2f &point)
{
    Point2f sample;

    sample[0] = bound(point(0) * image.width(), 0.f, image.width() - 1);
    sample[1] = bound(point(1) * image.height(), 0.f, image.height() - 1);

    // it is segfaulting and I am trying to figure out why
    LOG("pre");

    LOG(std::to_string(point(0)));
    LOG(std::to_string(point(1)));

    LOG("samp");

    LOG(std::to_string(sample[0]));
    LOG(std::to_string(sample[1]));

    Color3f im_samp = Color3f(image(sample(0), sample(1), 0),
                              image(sample(0), sample(1), 1),
                              image(sample(0), sample(1), 2));

    LOG("post");

    return im_samp * scale;
}

FEIGN_END()
