#include <feign/scene.h>

Scene::Scene() : SceneNode()
{
    sceneObjects = vector<SceneNode*>();
    root = nullptr;
}

Scene::Scene(SceneNode* parent) : SceneNode(parent)
{
    sceneObjects = vector<SceneNode*>();
    root = nullptr;
}

Scene::~Scene()
{
    delete root;
    sceneObjects.clear();
}

NodeType Scene::getNodeType() const
{
    return NT_Scene;
}

bool Scene::isPrimitive() const
{
    return false;
}

SceneNode* Scene::getRoot() const { return root; }
vector<SceneNode*> Scene::getSceneObjects() const { return sceneObjects; }
vector<Emitter*> Scene::getEmitters() const { return emitters; }
Texture* Scene::getEnvMap() const { return envMap; }
Integrator* Scene::getIntegrator() const { return integrator; }
Media* Scene::getEnvMedium() const { return envMedium; }
Sampler* Scene::getSampler() const { return sampler; }
