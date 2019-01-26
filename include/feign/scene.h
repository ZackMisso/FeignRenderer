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
    std::string getSceneName() const;
    Node* getRoot() const;
    std::vector<Node*> getSceneObjects() const;
    std::vector<Emitter*> getEmitters() const;
    Texture* getEnvMap() const;
    Integrator* getIntegrator() const;
    Media* getEnvMedium() const;
    Sampler* getSampler() const;
    Camera* getCamera() const;

    virtual NodeType getNodeType() const;
    virtual std::string getName() const;

protected:
    std::string sceneName;
    Node* root; // is this needed???
    Accel* acceleration;
    std::vector<Node*> sceneObjects; // keeping this for debugging purposes
    std::vector<Emitter*> emitters;
    Texture* envMap;
    Integrator* integrator;
    Media* envMedium;
    Sampler* sampler;
    Camera* camera;
};
