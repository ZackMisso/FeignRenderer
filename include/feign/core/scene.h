/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/node.h>
#include <feign/core/accel.h>
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
class AccelNode;
class CameraNode;

// TODO: scene should not be an observable data structure externally
class Scene
{
public:
    Scene();
    ~Scene();

    virtual void preProcess();

    void renderScene() const;

    bool intersect(const Ray3f& ray, Intersection& its) const;

    void addEmitter(Emitter* emitter);
    // void addObject(Node* object);

    // TODO: should these really be public...
public:
    std::string name;
    BBox3f sceneBounds;

    // required structures
    Integrator* integrator;
    Sampler* sampler;
    Camera* camera;
    Accel* ray_accel;

    // pre-processed structures
    std::vector<Emitter*> emitters;
    std::vector<Shape*> shapes;

    // optional structures
    Media* env_medium;
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
    SceneNode(Scene* scene) : scene(scene) { }

    SceneNode(std::string name,
              std::string integrator,
              std::string sampler,
              std::string camera,
              std::string medium) {}

    ~SceneNode() { }

    Scene* scene;

    IntegratorNode integrator_node;
    SamplerNode sampler_node;
    CameraNode camera_node;
    AccelNode accel_node;
    MediaNode media_node;
};
/////////////////////////////////////////////////
