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

void Scene::preProcess()
{
    preProcessChildren();

    for (int i = 0; i < children.size(); ++i)
    {
        if (children[i]->getNodeType() == NT_Camera)
        {
            camera = (Camera*)children[i];
        }
        if (children[i]->getNodeType() == NT_Sampler)
        {
            sampler = (Sampler*)children[i];
        }
        if (children[i]->getNodeType() == NT_Integrator)
        {
            integrator = (Integrator*)children[i];
        }
        if (children[i]->getNodeType() == NT_Emitter)
        {
            emitters.push_back((Emitter*)children[i]);
        }
    }
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
Camera* Scene::getCamera() const { return camera; }
