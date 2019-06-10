#include <feign/scene.h>
#include <feign/accel/naive.h>
#include <feign/accel/bvh.h>

Scene::Scene() : Node()
{
    sceneObjects = std::vector<Node*>();
    root = nullptr;
    acceleration = nullptr;
    scene = nullptr;
}

Scene::Scene(Node* parent) : Node(parent)
{
    sceneObjects = std::vector<Node*>();
    root = nullptr;
    acceleration = nullptr;
    scene = nullptr;
}

Scene::~Scene()
{
    delete root;
    sceneObjects.clear();
    rtcReleaseScene(scene);
    scene = nullptr;
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
        std::cout << "No Specified Acceleration" << std::endl;
        acceleration = new NaiveAccel();
        // acceleration = new BVH();

        // scene = rtcDeviceNewScene(EmbreeUtil::getDevice(), RTC_SCENE_STATIC | RTC_SCENE_INCOHERENT, RTC_INTERSECT1);
        // userGeomId = rtcNewUserGeometry(scene, shapes.size());

        // TODO: implement the scene
    }

    device = rtcNewDevice(rtcore.c_str());
    scene = rtcNewScene(device);

    // error_handler(nullptr,rtcGetDeviceError(device));
    /* set error handler */
    rtcSetDeviceErrorFunction(device, embree_error_handler, nullptr);

    // acceleration->clear();

    for (int i = 0; i < shapes.size(); ++i)
    {
        std::cout << "SHAPE IS BEING ADDED" << std::endl;
        acceleration->addShape(shapes[i]);
    }
    // acceleration->setMeshes(shapes);
    //
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
