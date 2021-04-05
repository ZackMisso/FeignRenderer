/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()

class Texture
{
public:
    Texture();
    virtual ~Texture();

    virtual Color3f evaluate(const Point2f& point);
    virtual Color3f evaluate(const Point3f& point);
    virtual Imagef evaluate_image(int width, int height);

    virtual void preProcess();
};

class ImageTexture : public Texture
{
public:
    struct Params
    {
        Params(std::string filename,
               Vec3f scale = Vec3f(1.f))
            : filename(filename), scale(scale) { }

        std::string filename;
        Vec3f scale;
    };

    ImageTexture(std::string filename,
                 Vec3f scale = Vec3f(1.f));

    ~ImageTexture();

    virtual Color3f evaluate(const Point2f& point);
    virtual void preProcess();

    Imagef image;
private:
    Vec3f scale;
    std::string filename;
};

class SinTexture : public Texture
{
public:
    struct Params
    {
        Params(Vec3f amp,
               Vec3f phase,
               Vec3f freq,
               Vec3f y)
            : amp(amp), phase(phase), y(y), freq(freq) { }

        Vec3f amp;
        Vec3f freq;
        Vec3f phase;
        Vec3f y;
    };

    SinTexture(Vec3f amp,
               Vec3f phase,
               Vec3f freq,
               Vec3f y);

    virtual Color3f evaluate(const Point2f& point);
    virtual void preProcess();

    Vec3f amp;
    Vec3f freq;
    Vec3f phase;
    Vec3f y;
};

class NoiseTexture : public Texture
{
public:
    struct Params
    {
        Params(std::string filename) : filename(filename) { }

        std::string filename;
    };

    NoiseTexture(std::string filename);

    virtual Color3f evaluate(const Point2f& point);
    virtual void preProcess();

    std::string filename;
private:
    bool initialized;
};

/////////////////////////////////////////////////
// Texture Node structure
/////////////////////////////////////////////////
struct TextureNode : public Node
{
public:
    TextureNode() : texture(nullptr) { }
    TextureNode(std::string name) : Node(name), texture(nullptr) { }
    TextureNode(Texture* texture) : texture(texture) { }

    ~TextureNode() { delete texture; }

    Texture* texture;
};
/////////////////////////////////////////////////

FEIGN_END()
