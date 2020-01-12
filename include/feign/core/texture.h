/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>

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
        Params(std::string filename) : filename(filename) { }

        std::string filename;
    };

    ImageTexture(std::string filename);

    virtual Color3f evaluate(const Point2f& point);
    virtual void preProcess();

    Imagef image;
    std::string filename;
private:
    bool initialized;
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

    Texture* texture;
};
/////////////////////////////////////////////////
