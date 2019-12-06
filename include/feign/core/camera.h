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
class Camera
{
public:
    Camera() { }
    virtual ~Camera() { }

    // takes in a film and apperture sample
    // calculates a generated ray according to the samples
    // returns an importance to weight the generated ray
    virtual Color3f sampleRay(Ray3f& ray,
                              const Point2f& filmSamp,
                              const Point2f& appSamp) const = 0;

    Vec2i getFilmSize() const { return filmSize; }

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
    Perspective(Float aperatureRadius,
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

    virtual void preProcess();

    virtual Color3f sampleRay(Ray3f& ray,
                              const Point2f& filmSamp,
                              const Point2f& appSamp) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Camera Node structure
/////////////////////////////////////////////////
struct CameraNode : public Node
{
public:
    CameraNode() : camera(nullptr) { }
    CameraNode(Camera* camera) : camera(camera) { }

    Camera* camera;
};
/////////////////////////////////////////////////
