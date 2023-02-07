/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/node.h>
#include <feign/core/accel_ray.h>
#include <feign/core/accel_light.h>
#include <feign/core/integrator.h>
#include <feign/core/emitter.h>
#include <feign/core/sampler.h>
#include <feign/core/camera.h>
#include <feign/media/media.h>
#include <feign/misc/intersection.h>

FEIGN_BEGIN()

// required forward declarations for the Nodes
class IntegratorNode;
class SamplerNode;
class MediaNode;
class RayAccelNode;
class CameraNode;

// TODO: scene should not be an observable data structure externally
class Scene
{
public:
    Scene(std::string name,
          IntegratorNode *integrator,
          SamplerNode *sampler,
          CameraNode *camera,
          MediaNode *media);

    ~Scene();

    virtual void preProcess(const GlobalParams &globals);

    void renderScene() const;

    bool intersect_full(const Ray3f &ray, Intersection &its) const;
    bool intersect_non_null(const Ray3f &ray, Intersection &its) const;
    bool intersect_transmittance(const Ray3f &ray,
                                 const Media *initial_media,
                                 Intersection &its,
                                 Sampler *sampler,
                                 Color3f &beta,
                                 bool last_event_surface = true,
                                 bool next_event_surface = true) const;

    void addEmitter(Emitter *emitter);
    void addMedium(Media *media);

    const MaterialShader *getShapeMaterialShader(const Intersection &its) const;

    void eval_all_emitters(MaterialClosure &closure, bool in_media = false) const;
    void eval_one_emitter(MaterialClosure &closur, bool in_media = false) const;
    void eval_multi_emitters(MaterialClosure &closur,
                             int num_to_eval,
                             bool in_media = false) const;
    void accumulate_emission(MaterialClosure &closure) const;

    Color3f env_emission(const Ray3f &ray) const;

    // choosing an emitter given a specific intersection location
    Emitter *choose_emitter(MaterialClosure &closure, Float *pdf) const;
    // choosing a uniformly random emitter
    Emitter *choose_emitter(Sampler *sampler, Float *pdf) const;

public:
    std::string name;
    BBox3f sceneBounds;

    // required structures
    IntegratorNode *integrator_node;
    SamplerNode *sampler_node;
    LightAccel *light_selection;
    CameraNode *camera_node;
    RayAccel *ray_accel;
    Imagef *target; // the target to render the image to, if not
                    // offline rendering

    // pre-processed structures
    std::vector<Emitter *> env_emitters;
    std::vector<Emitter *> emitters;
    std::vector<Media *> mediums;
    std::vector<Shape *> shapes;
    std::vector<ObjectNode *> objects;

    // optional structures
    MediaNode *env_medium_node;
};

// TODO: the nodes should be a layer of abstraction over the actual
// implementation

/////////////////////////////////////////////////
// Scene Node structure
/////////////////////////////////////////////////
struct SceneNode : public Node
{
public:
    SceneNode() : scene(nullptr) {}
    SceneNode(std::string name) : Node(name), scene(nullptr) {}
    SceneNode(Scene *scene) : scene(scene) {}
    SceneNode(std::string name, Scene *scene)
        : Node(name), scene(scene) {}

    ~SceneNode() { delete scene; }

    Scene *scene;
};
/////////////////////////////////////////////////

FEIGN_END()
