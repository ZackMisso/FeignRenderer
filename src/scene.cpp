/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/scene.h>

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
    light_selection = nullptr;
    target = nullptr;
}

Scene::~Scene()
{
    if (ray_accel) delete ray_accel;
    if (light_selection) delete light_selection;
    integrator_node = nullptr;
    sampler_node = nullptr;
    camera_node = nullptr;
    target = nullptr;
    env_medium_node = nullptr;
    shapes.clear();
    objects.clear();
    emitters.clear();
}

void Scene::preProcess(const GlobalParams& globals)
{
    sceneBounds = BBox3f(Vec3f(0.f), Vec3f(0.f));

    if (!ray_accel)
    {
        if (globals.sdf_only)
        {
            ray_accel = new SDFAccel();
        }
        else
        {
            ray_accel = new EmbreeAccel();
        }

        ray_accel->preProcess();
    }
    if (!light_selection)
    {
        light_selection = new NaiveLightAccel();
    }

    for (int i = 0; i < shapes.size(); ++i)
    {
        if (globals.sdf_only)
        {
            ray_accel->addSDFShape((SDFShape*)shapes[i]);
        }
        else
        {
            ray_accel->addShape(shapes[i]);
        }

        sceneBounds.expand(shapes[i]->boundingBox());
    }

    ray_accel->build();
    light_selection->build(sceneBounds, emitters);

    integrator_node->integrator->preProcess();
    camera_node->camera->preProcess();
    LOG("preprocessing medium");
    if (env_medium_node->media)
        env_medium_node->media->preProcess();
    LOG("post preprocess medium");

    // TODO: is this really the best place to handle this?
    for (int i = 0; i < objects.size(); ++i)
    {
        // check if the object's emitter is valid. If so,
        // make sure the emitter's mesh matches the object's
        if (objects[i]->emitter)
        {
            objects[i]->emitter->emitter->setMeshNode(objects[i]->mesh);
            objects[i]->emitter->emitter->preProcess();
        }
    }
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

    Imagef* image;

    if (target)
    {
        image = target;
        camera->setFilmSize(Vec2i(image->width(), image->height()));
    }
    else
    {
        image = new Imagef(camera->getFilmSize()[0],
                           camera->getFilmSize()[1],
                           3);
    }

    // TODO: this will need to be changed for parallelization
    integrator->render(this,
                       camera,
                       sampler,
                       *image);

    if (!target)
    {
        image->write(integrator->location + name + ".png"); // .png writer has some issues for some scenes
        image->write(integrator->location + name + ".exr");
    }
}

bool Scene::intersect(const Ray3f& ray, Intersection& its) const
{
    its.medium = nullptr;

    bool intersected = ray_accel->intersect(ray, its);
    if (env_medium_node && !its.medium)
    {
        // LOG("setting medium");
        its.medium = env_medium_node->media;
    }

    return intersected;
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

// TODO: make these eval methods call the same function
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
                                 sqrt(eqr.sqr_dist) - Epsilon);

        Intersection tmp;

        if (!intersect(shadow_ray, tmp) ||
             global_params.ignore_shadow_checks)
        {
            Float cos_term = closure.its->g_frame.n % eqr.wi;

            if (cos_term < -Epsilon) cos_term = -cos_term;

            closure.shadow_rays[i].valid = true;
            closure.shadow_rays[i].shadow_ray = closure.its->toLocal(eqr.wi);

            Float transmittance = 1.f;
            if (tmp.medium)
            {
                transmittance = tmp.medium->transmittance(shadow_ray,
                                                          closure.sampler);
            }

            if (emitter_pdf == 0.f)
            {
                closure.shadow_rays[i].throughput = Li * cos_term * transmittance;
            }
            else
            {
                closure.shadow_rays[0].throughput = Li * cos_term / (emitter_pdf) * transmittance;
            }

            // Note: bsdf_values are fully accumulated later
        }
    }
}

void Scene::eval_one_emitter(MaterialClosure& closure) const
{
    closure.shadow_rays = std::vector<EmitterEval>(1);

    // uniform sampling of light sources
    // TODO: later add infrastructure for different light sampling
    //       methods

    Float choice_pdf;
    int emitter;

    light_selection->sampleEmitter(closure.its->p,
                                   closure.sampler,
                                   emitter,
                                   choice_pdf);

    EmitterQuery eqr(closure.its->p);
    Float emitter_pdf = 0.f;
    Color3f Li = emitters[emitter]->sample_li(eqr,
                                              closure.sampler->next2D(),
                                              &emitter_pdf);

    Ray3f shadow_ray = Ray3f(closure.its->p,
                             eqr.wi,
                             Epsilon,
                             sqrt(eqr.sqr_dist) - Epsilon);

    Intersection tmp;

    if (!intersect(shadow_ray, tmp) ||
         global_params.ignore_shadow_checks)
    {
        Float cos_term = closure.its->s_frame.n % eqr.wi;

        if (cos_term < -Epsilon) cos_term = -cos_term;

        closure.shadow_rays[0].valid = true;
        closure.shadow_rays[0].shadow_ray = closure.its->toLocal(eqr.wi);

        Float transmittance = 1.f;

        if (tmp.medium)
        {
            transmittance = tmp.medium->transmittance(shadow_ray,
                                                      closure.sampler);
        }

        if (emitter_pdf == 0.f)
        {
            closure.shadow_rays[0].throughput = Color3f(0.f);
        }
        else
        {
            closure.shadow_rays[0].throughput = Li * cos_term / (choice_pdf * emitter_pdf) * transmittance;
        }

        // Note: bsdf_values are fully accumulated later
    }
}

void Scene::accumulate_emission(MaterialClosure& closure) const
{
    int id = closure.its->intersected_mesh->getInstID();
    EmitterNode* emitter = objects[id]->emitter;

    if (emitter)
    {
        EmitterQuery rec = EmitterQuery(closure.ray->origin);
        rec.wi = closure.its->toLocal(closure.ray->dir);
        rec.sh_n = closure.its->toLocal(closure.its->s_frame.n);
        closure.emission = emitter->emitter->evaluate(rec);
    }
    else
    {
        closure.emission = Color3f(0.f);
    }
}
