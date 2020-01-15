/**
 * Author:    Zackary Misso
 * Version:   0.1.1
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
#include <feign/core/shader.h>
#include <feign/core/texture.h>
#include <unordered_map>

// The end goal is to design all of this so it can be specified by a node graph
// but it also needs to retain speed

class FeignRenderer
{
private:
    FeignRenderer();
    ~FeignRenderer();

    static FeignRenderer* instance;

protected:
    static FeignRenderer* getInstance() { return instance; }

public:

    Transform current_transform;
    SceneNode* scene;

    std::unordered_map<std::string, BSDFNode*> bsdfs;
    std::unordered_map<std::string, CameraNode*> cameras;
    std::unordered_map<std::string, EmitterNode*> emitters;
    std::unordered_map<std::string, MediaNode*> medias;
    std::unordered_map<std::string, IntegratorNode*> integrators;
    std::unordered_map<std::string, SamplerNode*> samplers;
    std::unordered_map<std::string, FilterNode*> filters;
    std::unordered_map<std::string, MaterialNode*> materials;
    std::unordered_map<std::string, ObjectNode*> objects;
    std::unordered_map<std::string, MeshNode*> meshes;
    std::unordered_map<std::string, GeometryShaderNode*> geom_shaders;
    std::unordered_map<std::string, MaterialShaderNode*> material_shaders;
    std::unordered_map<std::string, TextureNode*> textures;

    BSDFNode*           find_bsdf(std::string name);
    IntegratorNode*     find_integrator(std::string name);
    CameraNode*         find_camera(std::string name);
    EmitterNode*        find_emitter(std::string name);
    MediaNode*          find_media(std::string name);
    SamplerNode*        find_sampler(std::string name);
    FilterNode*         find_filter(std::string name);
    MaterialNode*       find_material(std::string name);
    ObjectNode*         find_object(std::string name);
    MeshNode*           find_mesh(std::string name);
    GeometryShaderNode* find_geometry_shader(std::string name);
    MaterialShaderNode* find_material_shader(std::string name);
    TextureNode*        find_texture(std::string name);

    static void initialize();
    static void clean_up();

    static void fr_scene(std::string name,
                         std::string integrator_node,
                         std::string sampler_node,
                         std::string camera_node,
                         std::string medium_node);

    static void fr_integrator(std::string name,
                              std::string type,
                              std::string filter,
                              void* integrator_data);

    static void fr_sampler(std::string name,
                           std::string type,
                           void* sampler_data);

    static void fr_camera(std::string name,
                          std::string type,
                          void* camera_data);

    static void fr_object(std::string name,
                          std::string mesh,
                          std::string material,
                          int index = -1); // what is this index parameter again

    static void fr_mesh(std::string name,
                        std::string type,
                        void* mesh_data);

    // TODO: should this be separated into the different shaders?
    static void fr_shader(std::string name,
                          std::string type,
                          void* shader_data);

    // TODO: with the current set up ypu can not sample emissive objects which
    //       are not emitters for NEE..... but NEE is not implemented yet so
    //       this is a problem for the future
    static void fr_emitter(std::string name,
                           std::string type,
                           std::string mesh,
                           std::string material,
                           void* emitter_data);

    static void fr_material(std::string name,
                            std::string type,
                            void* material_data);

    static void fr_bsdf(std::string name,
                        std::string type,
                        void* bsdf_data);

    static void fr_texture(std::string name,
                           std::string type,
                           void* texture_data);

    static void fr_clear_transform();
    static void fr_scale(float sx, float sy, float sz);
    static void fr_translate(float tx, float ty, float tz);
    static void fr_rotate(float angle, float x, float y, float z);

    static void flush_renders();
};
