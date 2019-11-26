/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

// eventually, you will be able to create renders by only having to include api.h
// and calling the correct api calls to initialize a scene.

// the format of the api is slightly different than the format of the rest of the
// project. All method names are written in lower case and separate words are
// separated by underscores.

// this api is the main interface between this renderer and whatever application
// or gui attempting to use it. This api is made to be used to set up a scene
// and sets up the parameters and executes renders. If a feature is added to
// this renderer, then it should also be made available here.

#include <feign/common.h>
#include <feign/core/scene.h>
#include <unordered_map>

// The end goal is to design all of this so it can be specified by a node graph
// but it also needs to retain speed

class FeignRenderer
{
private:
    static std::vector<Transform> transform_stack;
    static Transform current_transform;
    static Scene* scene;

    // TODO: what is the best representation? vector? tree?
    static std::vector<Node*> nodes;
    static std::unordered_map<std::string, BSDF*> bsdfs;
    static std::unordered_map<std::string, Camera*> cameras;
    static std::unordered_map<std::string, Emitter*> emitters;
    static std::unordered_map<std::string, Media*> medias;
    static std::unordered_map<std::string, Integrator*> integrators;
    static std::unordered_map<std::string, Sampler*> samplers;
    static std::unordered_map<std::string, ReconstructionFilter*> filters;
    static std::unordered_map<std::string, Material*> materials;

    static void clean_up();

public:
    // TODO: this new api is not good for users directly
    static void fr_scene(std::string name,
                         std::string integrator_node,
                         std::string sampler_node,
                         std::string camera_node,
                         std::string medium_node);

    static void fr_integrator(std::string name,
                              std::string type,
                              long max_time,
                              long max_heuristic);

    static void fr_sampler(std::string name,
                           std::string type,
                           int spp,
                           long seed,
                           long seed2);

    static void fr_camera(std::string name,
                          std::string type,
                          Vector3f origin,
                          Vector3f target,
                          Vector3f up,
                          float fov,
                          Vec2i image_res);

    static void fr_object(std::string name,
                          std::string mesh,
                          std::string material);

    static void fr_mesh(std::string name,
                        std::string type,
                        std::string filename);

    static void fr_emitter(std::string name,
                           std::string type,
                           std::string mesh,
                           std::string material,
                           Vector3f pos,
                           Color3f intensity);

    static void fr_material(std::string name,
                            std::string bsdf);

    static void fr_bsdf(std::string name,
                        std::string type,
                        Color3f albedo);

    static void fr_clear_transform();
    static void fr_scale(float sx, float sy, float sz);
    static void fr_translate(float tx, float ty, float tz);
    static void fr_rotate(float angle, float x, float y, float z);

    static void flush_renders();




    // Old api
    // // begin
    // static void begin_world();
    // static void create_scene(std::string name);
    // static void begin_node();
    // static void begin_obj(const std::string& filename,
    //                       bool flip_norms);
    //
    // // end
    // static void end_world();
    // static void end_node();
    // static void end_obj();
    //
    // // bsdfs
    // static void bsdf_diffuse(Color3f albedo = Color3f(0.5));
    //
    // // cameras
    // static void camera_ortho(Vec3f ori,
    //                          Vec3f tar,
    //                          Vec3f up);
    //
    // static void camera_perspective(Vec3f ori,
    //                                Vec3f tar,
    //                                Vec3f up,
    //                                Float aperatureRadius,
    //                                Float focalDistance,
    //                                Float fov,
    //                                Float near,
    //                                Float far,
    //                                uint32_t width,
    //                                uint32_t height);
    //
    // // integrators
    // static void integrator(std::string integrator_type);
    //
    // // samplers
    // static void sampler(std::string sampler_type,
    //                     uint32_t sample_cnt,
    //                     uint32_t seed);
    //
    // // emitters
    // static void emitter_point(Color3f I = Color3f(1.0),
    //                           Point3f pos = Point3f(0.0));
    //
    // // transforms
    // static void transform_scale(Vec3f scale);
    //
    // static void transform_rot(Float degree, Vec3f axis);
    //
    // static void transform_trans(Vec3f translate);
    //
    // static void transform_lookat(Vec3f eye,
    //                              Vec3f at,
    //                              Vec3f up);
    //
    // static void transform_matrix(Float a00, Float a01, Float a02, Float a03,
    //                              Float a10, Float a11, Float a12, Float a13,
    //                              Float a20, Float a21, Float a22, Float a23,
    //                              Float a30, Float a31, Float a32, Float a33);
    //
    // static void transform_matrix(Matrix4f matrix);
    //
    // static void transform_transform(Transform transform);
    //
    // // TODO: ehhh are these needed anymore???
    // // for external control
    // static void transform_push();
    // static void transform_pop();

    // TODO: add methods to run renders or batch renders
};
