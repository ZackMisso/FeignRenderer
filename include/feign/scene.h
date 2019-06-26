#pragma once

#include <feign/common.h>
#include <feign/node.h>
#include <feign/accel/accel.h>
#include <feign/integrators/integrator.h>
#include <feign/emitters/emitter.h>
#include <feign/media/media.h>
#include <feign/samplers/sampler.h>
#include <feign/cameras/camera.h>
#include <feign/misc/intersection.h>

class Scene : public Node
{
public:
    Scene();
    Scene(Node* parent);
    ~Scene();

    virtual void preProcess(bool use_prims);

    bool intersect(const Ray3f& ray, Intersection& its) const;

    void addEmitter(Emitter* emitter);
    void addObject(Node* object);

    // getter methods
    const std::vector<Node*>& getSceneObjects() const { return sceneObjects; }
    const std::vector<Emitter*>& getEmitters() const { return emitters; }
    const std::string& getSceneName() const { return sceneName; }

    Node* getRoot() const { return root; }
    Integrator* getIntegrator() const { return integrator; }
    Media* getEnvMedium() const { return envMedium; }
    Sampler* getSampler() const { return sampler; }
    Camera* getCamera() const { return camera; }

    // setter methods
    void setSceneName(const std::string& param) { sceneName = param; }
    void setIntegrator(Integrator* param) { integrator = param; }
    void setEnvMedium(Media* param) { envMedium = param; }
    void setSampler(Sampler* param) { sampler = param; }
    void setCamera(Camera* param) { camera = param; }

    virtual NodeType getNodeType() const;
    virtual std::string getName() const;

protected:
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
