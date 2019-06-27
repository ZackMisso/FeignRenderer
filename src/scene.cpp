#include <feign/scene.h>
#include <feign/accel/naive.h>
#include <feign/accel/bvh.h>
#include <feign/accel/embree.h>

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
    // TODO - more?
}

// TODO: this preprocess assumes no depth in the scene graph
void Scene::preProcess(bool use_prims)
{
    preProcessChildren(use_prims);

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
        // std::cout << "No Specified Acceleration" << std::endl;
        acceleration = new EmbreeAccel();
        acceleration->preProcess(use_prims);
    }

    // std::cout << "adding shapes" << std::endl;

    for (int i = 0; i < shapes.size(); ++i)
    {
        // std::cout << "wooop" << std::endl;
        acceleration->addShape(shapes[i]);
    }

    // std::cout << "building" << std::endl;

    acceleration->build();

    // std::cout << "what" << std::endl;
}

bool Scene::intersect(const Ray3f& ray, Intersection& its) const
{
    return acceleration->intersect(ray, its);
}

void Scene::addEmitter(Emitter* emitter)
{
    emitters.push_back(emitter);
}

void Scene::addObject(Node* node)
{
    sceneObjects.push_back(node);
}

std::string Scene::getName() const
{
    return "scene";
}

NodeType Scene::getNodeType() const
{
    return NT_Scene;
}
