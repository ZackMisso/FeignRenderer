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

// TODO: scene should not be an observable data structure externally
class Scene : public Node
{
public:
    Scene();
    Scene(Node* parent);
    ~Scene();

    virtual void preProcess();

    void renderScene() const;

    bool intersect(const Ray3f& ray, Intersection& its) const;

    void addEmitter(Emitter* emitter);
    void addObject(Node* object);

    virtual NodeType getNodeType() const;
    virtual std::string getName() const;

    // TODO: should these really be public...
public:
    std::string sceneName;
    BBox3f sceneBounds;
    Node* root; // is this needed???
    Accel* acceleration;
    std::vector<Node*> sceneObjects; // keeping this for debugging purposes
    std::vector<Emitter*> emitters;
    Integrator* integrator;
    Media* envMedium;
    Sampler* sampler;
    Camera* camera;
};
