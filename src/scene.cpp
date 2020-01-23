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
    // if (global_params.sdf_only)
    // {
    //     LOG("working");
    // }

    if (!ray_accel)
    {
        if (global_params.sdf_only)
        {
            // assert(false);
            ray_accel = new SDFAccel();
        }
        else
        {
            // assert(false);
            ray_accel = new EmbreeAccel();
        }

        ray_accel->preProcess();
    }

    for (int i = 0; i < shapes.size(); ++i)
    {
        shapes[i]->transform.print();
        if (global_params.sdf_only)
        {
            ray_accel->addSDFShape((SDFShape*)shapes[i]);
        }
        else
        {
            ray_accel->addShape(shapes[i]);
        }
    }

    // assert(false);
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

const MaterialShader* Scene::getShapeMaterialShader(const Intersection& its) const
{
    int id = its.intersected_mesh->getInstID();

    return (*(*objects[id]).material_shader)();
}

// TODO: this should also work for media closures...
void Scene::eval_all_emitters(MaterialClosure& closure) const
{
    closure.shadow_rays = std::vector<EmitterEval>(emitters.size());

    for (int i = 0; i < emitters.size(); ++i)
    {
        EmitterQuery eqr(closure.its->p);
        Float emitter_pdf = 0.f;
        Color3f Li = emitters[i]->sample_li(eqr,
                                            closure.sampler->next2D(),
                                            &emitter_pdf);

        Ray3f shadow_ray = Ray3f(closure.its->p,
                                 eqr.wi,
                                 Epsilon,
                                 sqrt(eqr.sqr_dist));

        Intersection tmp;

        if (!intersect(shadow_ray, tmp) ||
             global_params.ignore_shadow_checks)
        {
            Float cos_term = closure.its->s_frame.n % eqr.wi;
            if (cos_term < -Epsilon) cos_term = -cos_term;

            closure.shadow_rays[i].valid = true;
            closure.shadow_rays[i].shadow_ray = closure.its->toLocal(eqr.wi);
            closure.shadow_rays[i].throughput = Li * cos_term;
            // Note: bsdf_values are fully accumulated later
        }
    }
}

void Scene::eval_one_emitter(MaterialClosure& closure) const
{
    closure.shadow_rays = std::vector<EmitterEval>(1);

    Float choice_pdf = 1.0 / Float(emitters.size());
    // LOG("choice pdf:", choice_pdf);

    int emitter = closure.sampler->next1D() * emitters.size();

    EmitterQuery eqr(closure.its->p);
    Float emitter_pdf = 0.f;
    Color3f Li = emitters[emitter]->sample_li(eqr,
                                              closure.sampler->next2D(),
                                              &emitter_pdf);

    Ray3f shadow_ray = Ray3f(closure.its->p,
                             eqr.wi,
                             Epsilon,
                             sqrt(eqr.sqr_dist));

    Intersection tmp;

    if (!intersect(shadow_ray, tmp) ||
         global_params.ignore_shadow_checks)
    {
        Float cos_term = closure.its->s_frame.n % eqr.wi;
        if (cos_term < -Epsilon) cos_term = -cos_term;

        closure.shadow_rays[0].valid = true;
        closure.shadow_rays[0].shadow_ray = closure.its->toLocal(eqr.wi);
        closure.shadow_rays[0].throughput = Li * cos_term * choice_pdf / emitter_pdf;
        // Note: bsdf_values are fully accumulated later
    }
}
