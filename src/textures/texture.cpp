/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/texture.h>

Texture::Texture() { }

Texture::~Texture() { }

void Texture::preProcess() { }

Color3f Texture::evaluate(const Point2f& point)
{
    return Color3f(0.f);
}

Color3f Texture::evaluate(const Point3f& point)
{
    return Color3f(0.f);
}

Imagef Texture::evaluate_image(int width, int height)
{
    Imagef image = Imagef(width, height, 3);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            Float xpos = Float(j) / Float(width) + 0.5 / Float(width);
            Float ypos = Float(i) / Float(height) + 0.5 / Float(height);

            Color3f color = evaluate(Point2f(xpos, ypos));

            image(j, i, 0) = color[0];
            image(j, i, 1) = color[1];
            image(j, i, 2) = color[2];
        }
    }

    return image;
}
