/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/sampler.h>
#include <feign/math/ray.h>

class MediaClosure;

class Media
{
public:
    Media();
    virtual ~Media();

    virtual Float sample(Ray3f ray,
                         Sampler* sampler,
                         MediaClosure& closure) const = 0;

    virtual Float transmittance(Ray3f ray,
                                Float min_t,
                                Float max_t) const = 0;

    virtual bool isGlobal() const { return false; }
};

class HomogeneousAbsorbingMedia : public Media
{
public:
    struct Params
    {
        Params(Float avg_density)
            : avg_density(avg_density) { }

        Float avg_density;
    };

    HomogeneousAbsorbingMedia(Float avg_density);

    virtual Float sample(Ray3f ray,
                         Sampler* sampler,
                         MediaClosure& closure) const;

    virtual Float transmittance(Ray3f ray,
                                Float min_t,
                                Float max_t) const;

    virtual bool isGlobal() const;

    Float avg_density;
};

/////////////////////////////////////////////////
// Material Node structure
/////////////////////////////////////////////////
struct MediaNode : public Node
{
public:
    MediaNode() : media(nullptr) { }
    MediaNode(std::string name) : Node(name), media(nullptr) { }
    MediaNode(Media* media) : media(media) { }

    ~MediaNode() { delete media; }

    Media* media;
};
/////////////////////////////////////////////////
