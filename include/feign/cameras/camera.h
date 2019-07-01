#pragma once

#include <feign/node.h>
#include <feign/math/ray.h>
#include <feign/math/transform.h>

class Camera : public Node
{
public:
    Camera();
    Camera(Node* parent);
    virtual ~Camera();

    // takes in a film and apperture sample
    // calculates a generated ray according to the samples
    // returns an importance to weight the generated ray
    virtual Color3f sampleRay(Ray3f& ray,
                              const Point2f& filmSamp,
                              const Point2f& appSamp) const = 0;

    virtual std::string getName() const;
    virtual NodeType getNodeType() const;

    Vec2i getFilmSize() const;

    void setCameraToWorld(const Transform& param) { cameraToWorld = param; }

protected:
    Transform cameraToWorld;
    Vec2i filmSize;
};
