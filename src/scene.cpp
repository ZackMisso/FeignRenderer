/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

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
    std::cout << "deleting scene" << std::endl;
    std::cout << "before delete root" << std::endl;
    if (!root) std::cout << "root does not exist" << std::endl;
    delete root;
    std::cout << "post delete root" << std::endl;
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

void Scene::renderScene() const
{
    if (!integrator)
    {
        throw new MissingPrimitiveException("no specified integrator");
    }
    if (!camera)
    {
        throw new MissingPrimitiveException("no specified camera");
    }
    if (!sampler)
    {
        throw new MissingPrimitiveException("no specified sampler");
    }

    std::cout << "rendering film: " << camera->getFilmSize()[0] << ", " << camera->getFilmSize()[1] << std::endl;

    // TODO: this will need to be changed for parallelization
    Imagef image = Imagef(camera->getFilmSize()[0],
                          camera->getFilmSize()[1],
                          3);

    integrator->render(this,
                       camera,
                       sampler,
                       image);

    std::cout << "writing scene: " << sceneName << std::endl;

    image.write(sceneName + ".png"); // .png writer has some issues
                                     // for some scenes
    image.write(sceneName + ".exr");

    std::cout << "Rendering Complete" << std::endl;
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
