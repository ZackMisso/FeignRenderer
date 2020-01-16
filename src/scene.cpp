/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/scene.h>
#include <feign/core/accel.h>

Scene::Scene(std::string name,
             IntegratorNode* integrator,
             SamplerNode* sampler,
             CameraNode* camera,
             MediaNode* media)
    : name(name),
      integrator_node(integrator),
      sampler_node(sampler),
      camera_node(camera),
      env_medium_node(media)
{
    shapes = std::vector<Shape*>();
    objects = std::vector<ObjectNode*>();
    ray_accel = nullptr;
}

Scene::~Scene()
{
    delete ray_accel;
    integrator_node = nullptr;
    sampler_node = nullptr;
    camera_node = nullptr;
    shapes.clear();
    objects.clear();
    emitters.clear();
}

void Scene::preProcess()
{
    if (!ray_accel)
    {
        if (global_params.sdf_only)
        {
            ray_accel = new SDFAccel();
        }
        else
        {
            ray_accel = new EmbreeAccel();
        }

        ray_accel->preProcess();
    }

    for (int i = 0; i < shapes.size(); ++i)
    {
        shapes[i]->transform.print();
        ray_accel->addShape(shapes[i]);
    }

    ray_accel->build();

    integrator_node->integrator->preProcess();
    camera_node->camera->preProcess();
}

void Scene::renderScene() const
{
    Integrator* integrator = integrator_node->integrator;
    Camera* camera = camera_node->camera;
    Sampler* sampler = sampler_node->sampler;

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

    // TODO: this will need to be changed for parallelization
    Imagef image = Imagef(camera->getFilmSize()[0],
                          camera->getFilmSize()[1],
                          3);

    integrator->render(this,
                       camera,
                       sampler,
                       image);

    image.write(integrator->location + name + ".png"); // .png writer has some issues for some scenes
    image.write(integrator->location + name + ".hdr"); // .hdr writer has some issues for some scenes
    image.write(integrator->location + name + ".exr");
}

bool Scene::intersect(const Ray3f& ray, Intersection& its) const
{
    return ray_accel->intersect(ray, its);
}

void Scene::addEmitter(Emitter* emitter)
{
    emitters.push_back(emitter);
}

const Material* Scene::getShapeMaterial(const Intersection& its) const
{
    int id = its.intersected_mesh->getInstID();

    return (*(objects[id]->material))();
}
