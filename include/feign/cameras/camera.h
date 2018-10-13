#pragma once

#include <feign/node.h>
#include <feign/math/ray.h>
#include <feign/math/transform.h>
#include <feign/filters/recon_filter.h>

class Camera : public Node
{
public:
    Camera();
    Camera(Node* parent);
    virtual ~Camera();

    // takes in a film and apperture sample
    // calculates a generated ray according to the samples
    // returns an importance to weight the generated ray
    virtual Color3f sampleRay(Ray3f ray,
                              Point2f filmSamp,
                              Point2f appSamp) const = 0;

    virtual string getName() const;
    virtual NodeType getNodeType() const;

protected:
    Transform cameraToWorld;
    ReconstructionFilter* filter;
    Vec2i filmSize;
};
