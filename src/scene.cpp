#include <feign/scene.h>

Scene::Scene() : Node()
{
    sceneObjects = vector<Node*>();
    root = nullptr;
}

Scene::Scene(Node* parent) : Node(parent)
{
    sceneObjects = vector<Node*>();
    root = nullptr;
}

Scene::~Scene()
{
    delete root;
    sceneObjects.clear();
}

string Scene::getName() const
{
    return "scene";
}

NodeType Scene::getNodeType() const
{
    return NT_Scene;
}

Node* Scene::getRoot() const { return root; }
vector<Node*> Scene::getSceneObjects() const { return sceneObjects; }
vector<Emitter*> Scene::getEmitters() const { return emitters; }
Texture* Scene::getEnvMap() const { return envMap; }
Integrator* Scene::getIntegrator() const { return integrator; }
Media* Scene::getEnvMedium() const { return envMedium; }
Sampler* Scene::getSampler() const { return sampler; }
