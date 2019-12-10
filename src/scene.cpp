/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

#include <feign/core/scene.h>
#include <feign/core/accel.h>

// Scene::Scene()
// {
//     // sceneObjects = std::vector<Node*>();
//     shapes = std::vector<Shape*>();
//     // root = nullptr;
//     ray_accel = nullptr;
//     integrator = nullptr;
//     env_medium = nullptr;
//     sampler = nullptr;
//     camera = nullptr;
// }

Scene::Scene(std::string name,
             IntegratorNode* integrator,
             SamplerNode* sampler,
             CameraNode* camera,
             MediumNode* media)
    : name(name),
      integrator(integrator),
      sampler(sampler),
      camera(camera),
      env_media(media)
{
    shapes = std::vector<Shape*>();
    ray_accel = nullptr;
}

Scene::~Scene()
{
    // TODO delete correctly
    // std::cout << "deleting scene" << std::endl;
    // std::cout << "before delete root" << std::endl;
    // if (!root) std::cout << "root does not exist" << std::endl;
    // delete root;
    // std::cout << "post delete root" << std::endl;
    // sceneObjects.clear();
}

void Scene::preProcess()
{
    // preProcessChildren();

    // std::vector<Shape*> shapes = std::vector<Shape*>();

    // for (int i = 0; i < children.size(); ++i)
    // {
    //     if (children[i]->getNodeType() == NT_Camera)
    //     {
    //         camera = (Camera*)children[i];
    //     }
    //     if (children[i]->getNodeType() == NT_Sampler)
    //     {
    //         sampler = (Sampler*)children[i];
    //     }
    //     if (children[i]->getNodeType() == NT_Integrator)
    //     {
    //         integrator = (Integrator*)children[i];
    //     }
    //     if (children[i]->getNodeType() == NT_Emitter)
    //     {
    //         emitters.push_back((Emitter*)children[i]);
    //     }
    //     if (children[i]->getNodeType() == NT_Accel)
    //     {
    //         ray_accel = (Accel*)children[i];
    //     }
    //     if (children[i]->getNodeType() == NT_Mesh)
    //     {
    //         shapes.push_back((Shape*)children[i]);
    //     }
    // }

    if (!ray_accel)
    {
        ray_accel = new EmbreeAccel();
        // ray_accel = new NaiveAccel();
        // children.push_back(ray_accel);
        ray_accel->preProcess();
    }
    // std::cout << "after accel" << std::endl;

    for (int i = 0; i < shapes.size(); ++i)
    {
        ray_accel->addShape(shapes[i]);
    }

    ray_accel->build();
}

void Scene::renderScene() const
{
    if (!integrator)
    {
        throw new FeignRendererException("scene: no specified integrator");
    }
    if (!camera)
    {
        throw new FeignRendererException("scene: no specified camera");
    }
    if (!sampler)
    {
        throw new FeignRendererException("scene: no specified sampler");
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

    std::cout << "writing scene: " << name << std::endl;

    image.write(name + ".png"); // .png writer has some issues
                                     // for some scenes
    image.write(name + ".exr");

    std::cout << "Rendering Complete" << std::endl;
}

bool Scene::intersect(const Ray3f& ray, Intersection& its) const
{
    return ray_accel->intersect(ray, its);
}

void Scene::addEmitter(Emitter* emitter)
{
    emitters.push_back(emitter);
}

// void Scene::addObject(Node* node)
// {
//     sceneObjects.push_back(node);
// }
