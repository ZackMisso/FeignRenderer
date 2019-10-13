/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/ray.h>
#include <feign/math/transform.h>

/////////////////////////////////////////////////
// Camera
/////////////////////////////////////////////////
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
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Perspective Camera
/////////////////////////////////////////////////
class Perspective : public Camera
{
public:
    Perspective();
    Perspective(Node* parent);
    Perspective(Node* parent,
                Float aperatureRadius,
                Float focalDistance,
                Float fov,
                Float near,
                Float far,
                uint32_t width,
                uint32_t height);

    virtual void preProcess();

    virtual Color3f sampleRay(Ray3f& ray,
                              const Point2f& filmSamp,
                              const Point2f& appSamp) const;

    virtual std::string getName() const;

    void print() const;

protected:
    Transform sampleToCamera;
    Float aperatureRadius;
    Float focalDistance;
    Float fov;
    Float near;
    Float far;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Orthographic Camera
/////////////////////////////////////////////////
class Orthographic : public Camera
{
public:
    Orthographic();
    Orthographic(Node* parent);

    virtual void preProcess();

    virtual Color3f sampleRay(Ray3f& ray,
                              const Point2f& filmSamp,
                              const Point2f& appSamp) const;

    virtual std::string getName() const;
};
/////////////////////////////////////////////////
