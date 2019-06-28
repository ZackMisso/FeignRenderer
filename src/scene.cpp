#include <feign/scene.h>
#include <feign/accel/naive.h>
#include <feign/accel/embree.h>

Scene::Scene() : Node()
{
    sceneObjects = std::vector<Node*>();
    root = nullptr;
    acceleration = nullptr;
    integrator = nullptr;
    envMedium = nullptr;
    sampler = nullptr;
    camera = nullptr;
}

Scene::Scene(Node* parent) : Node(parent)
{
    sceneObjects = std::vector<Node*>();
    root = nullptr;
    acceleration = nullptr;
    integrator = nullptr;
    envMedium = nullptr;
    sampler = nullptr;
    camera = nullptr;
}

Scene::~Scene()
{
    delete root;
    sceneObjects.clear();
}

// TODO: this preprocess assumes no depth in the scene graph
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

    std::cout << "before accel" << std::endl;
    if (!acceleration)
    {
        std::cout << "in accel" << std::endl;
        acceleration = new EmbreeAccel();
        // acceleration = new NaiveAccel();
        children.push_back(acceleration);
        acceleration->preProcess();
    }
    std::cout << "after accel" << std::endl;

    for (int i = 0; i < shapes.size(); ++i)
    {
        acceleration->addShape(shapes[i]);
    }

    acceleration->build();
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
