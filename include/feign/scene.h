#pragma once

#include <feign/common.h>
#include <feign/node.h>
#include <feign/accel/accel.h>
#include <feign/integrators/integrator.h>
#include <feign/emitters/emitter.h>
#include <feign/media/media.h>
#include <feign/samplers/sampler.h>
#include <feign/textures/texture.h>
#include <feign/cameras/camera.h>
#include <feign/misc/intersection.h>

class Scene : public Node
{
public:
    Scene();
    Scene(Node* parent);
    ~Scene();

    virtual void preProcess();

    bool intersect(const Ray3f& ray, Intersection& its) const;

    // getter methods
    string getSceneName() const;
    Node* getRoot() const;
    vector<Node*> getSceneObjects() const;
    vector<Emitter*> getEmitters() const;
    Texture* getEnvMap() const;
    Integrator* getIntegrator() const;
    Media* getEnvMedium() const;
    Sampler* getSampler() const;
    Camera* getCamera() const;

    virtual NodeType getNodeType() const;
    virtual string getName() const;

protected:
    string sceneName;
    Node* root; // is this needed???
    Accel* acceleration;
    vector<Node*> sceneObjects; // keeping this for debugging purposes
    vector<Emitter*> emitters;
    Texture* envMap;
    Integrator* integrator;
    Media* envMedium;
    Sampler* sampler;
    Camera* camera;
};
