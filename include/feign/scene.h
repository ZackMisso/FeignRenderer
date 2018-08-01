#pragma once

#include <feign/common.h>
#include <feign/node.h>
#include <feign/integrators/integrator.h>
#include <feign/emitters/emitter.h>
#include <feign/media/media.h>
#include <feign/samplers/sampler.h>
#include <feign/textures/texture.h>

// TODO: make this a scene node
class Scene : public SceneNode
{
public:
    Scene();
    Scene(SceneNode* parent);
    ~Scene();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    SceneNode* getRoot() const;
    vector<SceneNode*> getSceneObjects() const;
    vector<Emitter*> getEmitters() const;
    Texture* getEnvMap() const;
    Integrator* getIntegrator() const;
    Media* getEnvMedium() const;
    Sampler* getSampler() const;

protected:
    SceneNode* root;
    vector<SceneNode*> sceneObjects; // keeping this for debugging purposes
    vector<Emitter*> emitters;
    Texture* envMap;
    Integrator* integrator;
    Media* envMedium;
    Sampler* sampler;
};
