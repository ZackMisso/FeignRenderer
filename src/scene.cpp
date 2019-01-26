#include <feign/scene.h>
#include <feign/accel/naive.h>

Scene::Scene() : Node()
{
    sceneObjects = std::vector<Node*>();
    root = nullptr;
    acceleration = nullptr;
}

Scene::Scene(Node* parent) : Node(parent)
{
    sceneObjects = std::vector<Node*>();
    root = nullptr;
    acceleration = nullptr;
}

Scene::~Scene()
{
    delete root;
    sceneObjects.clear();
}

void Scene::preProcess()
{
    preProcessChildren();

    std::vector<Shape*> shapes = std::vector<Shape*>();

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
        if (children[i]->getNodeType() == NT_Accel)
        {
            acceleration = (Accel*)children[i];
        }
        if (children[i]->getNodeType() == NT_Mesh)
        {
            shapes.push_back((Shape*)children[i]);
        }
    }

    primitives->findString("sceneName", sceneName, "test.png");

    if (!acceleration)
    {
        acceleration = new NaiveAccel();
    }

    acceleration->setMeshes(shapes);
    acceleration->build();
}

bool Scene::intersect(const Ray3f& ray, Intersection& its) const
{
    return acceleration->intersect(ray, its);
}

std::string Scene::getName() const
{
    return "scene";
}

NodeType Scene::getNodeType() const
{
    return NT_Scene;
}

std::string Scene::getSceneName() const { return sceneName; }
Node* Scene::getRoot() const { return root; }
std::vector<Node*> Scene::getSceneObjects() const { return sceneObjects; }
std::vector<Emitter*> Scene::getEmitters() const { return emitters; }
Texture* Scene::getEnvMap() const { return envMap; }
Integrator* Scene::getIntegrator() const { return integrator; }
Media* Scene::getEnvMedium() const { return envMedium; }
Sampler* Scene::getSampler() const { return sampler; }
Camera* Scene::getCamera() const { return camera; }
