#pragma once

#include <feign/common.h>
#include <feign/node.h>
#include <feign/integrators/integrator.h>
#include <feign/emitters/emitter.h>
#include <feign/media/media.h>
#include <feign/samplers/sampler.h>
#include <feign/textures/texture.h>

class Scene : public Node
{
public:
    Scene();
    Scene(Node* parent);
    ~Scene();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    Node* getRoot() const;
    vector<Node*> getSceneObjects() const;
    vector<Emitter*> getEmitters() const;
    Texture* getEnvMap() const;
    Integrator* getIntegrator() const;
    Media* getEnvMedium() const;
    Sampler* getSampler() const;

protected:
    Node* root;
    vector<Node*> sceneObjects; // keeping this for debugging purposes
    vector<Emitter*> emitters;
    Texture* envMap;
    Integrator* integrator;
    Media* envMedium;
    Sampler* sampler;
};
