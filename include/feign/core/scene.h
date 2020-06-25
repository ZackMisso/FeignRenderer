/**
 * Author:    Zackary Misso
 * Version:   0.1.1
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
          IntegratorNode* integrator,
          SamplerNode* sampler,
          CameraNode* camera,
          MediaNode* media);

    ~Scene();

    virtual void preProcess(const GlobalParams& globals);

    void renderScene() const;

    bool intersect_full(const Ray3f& ray, Intersection& its) const;
    bool intersect_non_null(const Ray3f& ray, Intersection& its) const;
    bool intersect_transmittance(const Ray3f& ray,
                                 Intersection& its,
                                 Sampler* sampler,
                                 Color3f& beta) const;

    void addEmitter(Emitter* emitter);

    const MaterialShader* getShapeMaterialShader(const Intersection& its) const;
    const Media* getShapeMedium(const Intersection& its) const;

    void eval_all_emitters(MaterialClosure& closure) const;
    void eval_one_emitter(MaterialClosure& closure) const;
    void accumulate_emission(MaterialClosure& closure) const;

    // TODO: should these really be public...
public:
    std::string name;
    BBox3f sceneBounds;

    // required structures
    IntegratorNode* integrator_node;
    SamplerNode* sampler_node;
    LightAccel* light_selection;
    CameraNode* camera_node;
    RayAccel* ray_accel;
    Imagef* target; // the target to render the image to, if not
                    // offline rendering

    // pre-processed structures
    std::vector<Emitter*> emitters;
    std::vector<Shape*> shapes;
    std::vector<ObjectNode*> objects;

    // optional structures
    MediaNode* env_medium_node;
};

// TODO: the nodes should be a layer of abstraction over the actual
// implementation

/////////////////////////////////////////////////
// Scene Node structure
/////////////////////////////////////////////////
struct SceneNode : public Node
{
public:
    SceneNode() : scene(nullptr) { }
    SceneNode(std::string name) : Node(name), scene(nullptr) { }
    SceneNode(Scene* scene) : scene(scene) { }
    SceneNode(std::string name, Scene* scene)
        : Node(name), scene(scene) { }

    ~SceneNode() { delete scene; }

    Scene* scene;
};
/////////////////////////////////////////////////
