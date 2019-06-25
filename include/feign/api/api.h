#pragma once

// eventually, you will be able to create renders by only having to include api.h
// and calling the correct api calls to initialize a scene.

// the format of the api is slightly different than the format of the rest of the
// project. All method names are written in lower case.

// this api is the main interface between this renderer and whatever application
// or gui attempting to use it. This api is made to be used to set up a world of
// scenes and sets up the parameters and executes renders. If a feature is added
// to this renderer, then it should also be made available here.

#include <feign/common.h>

class FeignRenderer
{
private:
    static WorldNode* world;
    static std::vector<Transform> transform_stack;
    static Transform current_transform;
    static Node* current_node;
    static Scene* current_scene;

public:
    // begin
    static void begin_world();
    static void begin_scene();
    static void begin_obj(const std::string& filename);

    // end
    static void end_world();
    static void end_scene();
    static void end_obj();

    // bsdfs
    static void bsdf_diffuse(Color3f albedo = Color3f(0.5));

    // cameras
    static void camera_ortho(Vec3f ori,
                             Vec3f tar,
                             Vec3f up);

    static void camera_perspective(Vec3f ori,
                                   Vec3f tar,
                                   Vec3f up,
                                   Float aperatureRadius,
                                   Float focalDistance,
                                   Float fov,
                                   Float near,
                                   Float far);

    // integrators
    static void integrator_light_unidir();
    static void integrator_normal();
    static void integrator_path_bidir();
    static void integrator_path_unidir();
    static void integrator_whitted();

    // samplers
    static void sampler_independent(uint32_t sample_cnt, uint32_t seed);
    static void sampler_halton(uint32_t sample_cnt, uint32_t seed);
    static void sampler_latin(uint32_t sample_cnt, uint32_t seed);

    // emitters
    static void emitter_point(Color3f I = Color3f(1.0),
                              Point3f pos = Point3f(0.0));

    // transforms
    static void transform_scale(Vec3f scale);

    static void transform_rot(Float degree, Vec3f axis);

    static void transform_trans(Float x,
                                Float y,
                                Float z);

    static void transform_lookat(Vec3f eye,
                                 Vec3f at,
                                 Vec3f up);

    static void transform_matrix(Matrix4f matrix);

    // TODO: add methods to run renders or batch renders
}
