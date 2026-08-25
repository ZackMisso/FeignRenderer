/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/ray.h>
#include <feign/math/vector.h>
#include <feign/math/transform.h>

FEIGN_BEGIN()

/////////////////////////////////////////////////
// Camera
/////////////////////////////////////////////////
class Camera
{
public:
    Camera() {}
    virtual ~Camera() {}

    // takes in a film and apperture sample
    // calculates a generated ray according to the samples
    // returns an importance to weight the generated ray
    virtual Color3f sample_ray(Ray3f &ray,
                               const Point2f &film_samp,
                               const Point2f &app_samp) const = 0;

    virtual void pre_process() {}

    Vec2i get_film_size() const { return film_size; }
    void set_film_size(Vec2i param) { film_size = param; }

    void set_camera_to_world(const Transform &param) { camera_to_world = param; }

protected:
    Transform camera_to_world;
    Vec2i film_size;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Perspective Camera
/////////////////////////////////////////////////
class Perspective : public Camera
{
public:
    struct Params
    {
        Params(Vector3f origin,
               Vector3f target,
               Vector3f up,
               Float fov,
               Float near,
               Float far,
               Float focal_dist,
               Float app_radius,
               Vec2i image_res)
            : origin(origin),
              target(target),
              up(up),
              fov(fov),
              near(near),
              far(far),
              focal_dist(focal_dist),
              app_radius(app_radius),
              image_res(image_res) {}

        Vector3f origin;
        Vector3f target;
        Vector3f up;
        Float fov;
        Float near;
        Float far;
        Float focal_dist;
        Float app_radius;
        Vec2i image_res;
    };

    Perspective(Float aperature_radius,
                Float focal_distance,
                Float fov,
                Float near,
                Float far,
                uint32_t width,
                uint32_t height);

    ~Perspective() {}

    virtual void pre_process();

    virtual Color3f sample_ray(Ray3f &ray,
                               const Point2f &film_samp,
                               const Point2f &app_samp) const;

    void print() const;

    void set_transform(const Transform &transform)
    {
        sample_to_camera = transform;
    }

protected:
    Transform sample_to_camera;
    Float aperature_radius;
    Float focal_distance;
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
    ~Orthographic() {}

    virtual void pre_process();

    virtual Color3f sample_ray(Ray3f &ray,
                               const Point2f &film_samp,
                               const Point2f &app_samp) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Camera Node structure
/////////////////////////////////////////////////
struct CameraNode : public Node
{
public:
    CameraNode() : camera(nullptr) {}
    CameraNode(std::string name) : Node(name), camera(nullptr) {}
    CameraNode(Camera *camera) : camera(camera) {}

    ~CameraNode() { delete camera; }

    Camera *camera;
};
/////////////////////////////////////////////////

FEIGN_END()
