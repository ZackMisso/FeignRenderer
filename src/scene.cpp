/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/scene.h>

FEIGN_BEGIN()

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
    mediums = std::vector<Media*>();
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
    mediums.clear();
}

void Scene::preProcess(const GlobalParams& globals)
{
    sceneBounds = BBox3f(Vec3f(0.f), Vec3f(0.f));

    #if VERBOSE
        LOG("preprocessing ray accel");
    #endif

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

    #if VERBOSE
        LOG("preprocessing light selection");
    #endif

    if (!light_selection)
    {
        light_selection = new NaiveLightAccel();
    }

    #if VERBOSE
        LOG("adding shapes");
    #endif

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

    #if VERBOSE
        LOG("building accels");
    #endif

    ray_accel->build();
    light_selection->build(sceneBounds, emitters);

    #if VERBOSE
        LOG("preprocessing cameras");
    #endif

    camera_node->camera->preProcess();
    // integrator pre-processing is done pre-rendering

    #if VERBOSE
        LOG("preprocessing env media");
    #endif

    // TODO: should this double check be necessary?
    if (env_medium_node && env_medium_node->media)
        env_medium_node->media->preProcess();

    #if VERBOSE
        LOG("preprocessing object emitters");
    #endif

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

    #if VERBOSE
        LOG("preprocessing mediums");
    #endif

    for (int i = 0; i < mediums.size(); ++i)
    {
        mediums[i]->preProcess();
    }

    #if VERBOSE
        LOG("finished preprocessing scene");
    #endif
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
                           camera->getFilmSize()[1]);
    }

    // perform preprocessing if the integrator requires it. i.e. scatter photons
    integrator->preProcess(this, sampler);

    #if GOTTAGOFAST
        integrator->render_fast(this,
                                camera,
                                sampler,
                                *image);
    #else
        integrator->render(this,
                           camera,
                           sampler,
                           *image);
    #endif

    if (!target)
    {
        image->write(integrator->location + name + ".png"); // .png writer has some issues for some scenes
        image->write(integrator->location + name + ".exr");
    }
}

bool Scene::intersect_full(const Ray3f& ray, Intersection& its) const
{
    return ray_accel->intersect(ray, its);
}

// bounaded mediums are represented by null objects, which should be skipped
// during certain processes. Which is why this method exists
bool Scene::intersect_non_null(const Ray3f& ray, Intersection& its) const
{
    Ray3f tmp_ray = ray;

    Intersection tmp = its;

    while (ray_accel->intersect(tmp_ray, tmp))
    {
        if (tmp.intersected_mesh->is_null)
        {
            // update the near t on the ray and continue traversal
            tmp_ray.near = tmp.t + Epsilon;
        }
        else
        {
            its = tmp;
            return true;
        }
    }

    return false;
}

// the intersect method used for intersection calls where transmittance should
// also be computed. This is used for emitter sampling of light sources in the
// volpath integrator.
//
// TODO: this fails if you are already inside the medium
// TODO: this needs to be thoroughly tested in a bunch of different scenarios,
//       i'm fairly certain this will start breaking once multiple different,
//       volumes get added to the system
bool Scene::intersect_transmittance(const Ray3f& ray,
                                    const Media* initial_media,
                                    Intersection& its,
                                    Sampler* sampler,
                                    Color3f& beta,
                                    bool last_event_surface,
                                    bool next_event_surface) const
{
    // this method assumes beta is set before being called
    Ray3f tmp_ray = ray;

    std::vector<Ray3f> rays = std::vector<Ray3f>();
    std::vector<const Media*> mediums = std::vector<const Media*>();

    // need to do an initial
    if (initial_media)
    {
        bool intersected = ray_accel->intersect(tmp_ray, its);

        if (intersected)
        {
            if (its.intersected_mesh->is_null)
            {
                Ray3f medium_ray = tmp_ray;
                medium_ray.far = its.t;

                rays.push_back(medium_ray);
                mediums.push_back(initial_media);

                tmp_ray = Ray3f(ray.origin,
                                tmp_ray.dir,
                                its.t + Epsilon,
                                ray.far,
                                tmp_ray.depth);
            }
            else
            {
                return true;
            }
        }
        else
        {
            Ray3f medium_ray = tmp_ray;

            rays.push_back(medium_ray);
            mediums.push_back(initial_media);
        }
    }

    // the below logic assumes you are starting outside a null-bounded media and
    // that there will be two intersections.
    while (ray_accel->intersect(tmp_ray, its))
    {
        if (its.intersected_mesh->is_null)
        {
            const Media* media = its.intersected_mesh->boundry->inside->media;

            if (media)
            {
                // assert(false);
                // append the medium to the running list of media and rays, and
                // continue the intersection. If the intersection turns out to
                // never hit anything the various transmittance calls will be
                // evaluated as a post process.
                tmp_ray = Ray3f(ray.origin,
                                tmp_ray.dir,
                                its.t + Epsilon,
                                ray.far,
                                tmp_ray.depth);

                // do a second intersection call to get the far bounds of the
                // bounding medium.... TODO: this implementation will be bugged
                // when the two different mediums overlap.
                if (ray_accel->intersect(tmp_ray, its))
                {

                    Ray3f medium_ray = tmp_ray;
                    medium_ray.far = its.t;

                    // return false if an intersection actually occurs with
                    // something that is not apart of the medium.
                    if (!its.intersected_mesh->is_null)
                    {
                        return true;
                    }

                    rays.push_back(medium_ray);
                    mediums.push_back(media);

                    if (its.t >= tmp_ray.far) break;
                }
            }
            else
            {
                // update the near t on the ray and continue traversal
                tmp_ray.near = its.t + Epsilon;
            }
        }
        else
        {
            return true;
        }
    }

    // accumulate transmittance
    for (int i = 0; i < rays.size(); ++i)
    {
        if (mediums[i])
        {
            MediaClosure closure(mediums[i],
                                 rays[i].near,
                                 rays[i].far,
                                 last_event_surface,
                                 next_event_surface);

            beta *= mediums[i]->transmittance(rays[i],
                                              sampler,
                                              closure);
        }
    }

    return false;
}

void Scene::addEmitter(Emitter* emitter)
{
    emitters.push_back(emitter);
}

void Scene::addMedium(Media* media)
{
    mediums.push_back(media);
}

const MaterialShader* Scene::getShapeMaterialShader(const Intersection& its) const
{
    int id = its.intersected_mesh->getInstID();

    return (*(*objects[id]).material_shader)();
}

// TODO: make these eval methods call the same function
// TODO: this should also work for media closures...
// TODO: this currently has the bug where if a mesh light is hit it gets
//       double counted
void Scene::eval_all_emitters(MaterialClosure& closure, bool in_media) const
{
    closure.shadow_rays = std::vector<EmitterEval>(emitters.size());

    for (int i = 0; i < emitters.size(); ++i)
    {
        EmitterQuery eqr(closure.its->p);
        Float emitter_pdf = 0.f;
        Color3f Li = emitters[i]->sample_nee(eqr,
                                             closure.sampler->next2D(),
                                             &emitter_pdf);
        //LOG("base chech");
        if (emitters[i]->requiresInitialVisibilityCheck())
        {
            // LOG("hello");
            // create ray
            Ray3f ray = Ray3f(closure.its->p,
                              eqr.wi,
                              Epsilon,
                              std::numeric_limits<Float>::infinity());

            // test intersection against emitter mesh
            Intersection tmp;

            if (intersect_non_null(ray, tmp))
            {
                if (emitters[i]->getMeshNode()->mesh == tmp.intersected_mesh)
                {
                    eqr.sqr_dist = (tmp.p - closure.its->p).sqrNorm();
                    Li /= eqr.sqr_dist;
                }
                else
                {
                    Li = COLOR_BLACK;
                }
            }
            else
            {
                Li = COLOR_BLACK;
            }
        }

        Ray3f shadow_ray = Ray3f(closure.its->p,
                                 eqr.wi,
                                 Epsilon,
                                 sqrt(eqr.sqr_dist) - Epsilon);

        Intersection tmp;

        Color3f transmittance = Color3f(1.f);

        if (!intersect_transmittance(shadow_ray,
                                     closure.media,
                                     tmp,
                                     closure.sampler,
                                     transmittance,
                                     closure.last_event != VERTEX_MEDIUM,
                                     true) ||
            global_params.ignore_shadow_checks)
        {
            Float cos_term = closure.its->g_frame.n % eqr.wi;

            if (cos_term < -Epsilon) cos_term = -cos_term;

            if (!in_media) Li *= cos_term;

            closure.shadow_rays[i].valid = true;
            closure.shadow_rays[i].shadow_ray = closure.its->toLocal(eqr.wi);

            if (emitter_pdf == 0.f)
            {
                closure.shadow_rays[i].throughput = Color3f(0.f);
            }
            else
            {
                closure.shadow_rays[i].throughput = Li / (emitter_pdf) * transmittance;
            }

            // Note: bsdf_values are fully accumulated later
        }
    }
}

void Scene::eval_one_emitter(MaterialClosure& closure, bool in_media) const
{
    closure.shadow_rays = std::vector<EmitterEval>(1);

    Float choice_pdf = 0.f;
    Emitter* emitter = choose_emitter(closure, &choice_pdf);

    if (!emitter)
    {
        closure.shadow_rays[0].throughput = Color3f(0.f);
        return;
    }

    EmitterQuery eqr(closure.its->p);
    Float emitter_pdf = 0.f;
    Color3f Li = emitter->sample_nee(eqr,
                                     closure.sampler->next2D(),
                                     &emitter_pdf);

    //LOG("base chech");
    if (emitter->requiresInitialVisibilityCheck())
    {
        //LOG("hello");
        // create ray
        //LOG(eqr.wi);
        Ray3f ray = Ray3f(closure.its->p,
                          eqr.wi,
                          Epsilon,
                          std::numeric_limits<Float>::infinity());

        // test intersection against emitter mesh
        Intersection tmp;

        if (intersect_non_null(ray, tmp))
        {
            // LOG("emitter");
            // std::cout << emitter->getMeshNode()->mesh << std::endl;
            // LOG("intersected");
            // std::cout << tmp.intersected_mesh << std::endl;
            if (emitter->getMeshNode()->mesh == tmp.intersected_mesh)
            {
                eqr.sqr_dist = (tmp.p - closure.its->p).sqrNorm();
                Li /= eqr.sqr_dist;
                //LOG("sqr dist: " + std::to_string(eqr.sqr_dist));
            }
            else
            {
                Li = COLOR_BLACK;
            }
        }
        else
        {
            Li = COLOR_BLACK;
        }
    }

    Ray3f shadow_ray = Ray3f(closure.its->p,
                             eqr.wi,
                             Epsilon,
                             sqrt(eqr.sqr_dist) - Epsilon);

    Intersection tmp;

    Color3f transmittance = Color3f(1.f);

    if (!intersect_transmittance(shadow_ray,
                                 closure.media,
                                 tmp,
                                 closure.sampler,
                                 transmittance,
                                 closure.last_event != VERTEX_MEDIUM,
                                 true) ||
        global_params.ignore_shadow_checks)
    {
        if (!in_media)
        {
            Float cos_term = closure.its->s_frame.n % eqr.wi;

            if (cos_term < -Epsilon) cos_term = -cos_term;

            Li *= cos_term;
        }

        closure.shadow_rays[0].valid = true;
        closure.shadow_rays[0].shadow_ray = closure.its->toLocal(eqr.wi);

        if (emitter_pdf == 0.f)
        {
            closure.shadow_rays[0].throughput = Color3f(0.f);
        }
        else
        {
            closure.shadow_rays[0].throughput = Li / (choice_pdf * emitter_pdf) * transmittance;
        }

        // Note: bsdf_values are fully accumulated later
    }
}

void Scene::eval_multi_emitters(MaterialClosure& closure,
                                int to_sample,
                                bool in_media) const
{
    closure.shadow_rays = std::vector<EmitterEval>(to_sample);

    for (int i = 0; i < to_sample; ++i)
    {
        Float choice_pdf = 0.f;
        Emitter* emitter = choose_emitter(closure, &choice_pdf);

        if (!emitter)
        {
            closure.shadow_rays[i].throughput = Color3f(0.f);
            continue;
        }

        EmitterQuery eqr(closure.its->p);
        Float emitter_pdf = 0.f;
        Color3f Li = emitter->sample_nee(eqr,
                                         closure.sampler->next2D(),
                                         &emitter_pdf);

        if (emitter->requiresInitialVisibilityCheck())
        {
            //LOG("hello");
            // create ray
            Ray3f ray = Ray3f(closure.its->p,
                              eqr.wi,
                              Epsilon,
                              std::numeric_limits<Float>::infinity());

            // test intersection against emitter mesh
            Intersection tmp;

            if (intersect_non_null(ray, tmp))
            {
                if (emitter->getMeshNode()->mesh == tmp.intersected_mesh)
                {
                    eqr.sqr_dist = (tmp.p - closure.its->p).sqrNorm();
                    Li /= eqr.sqr_dist;
                    //LOG("sqr dist: " + std::to_string(eqr.sqr_dist));
                }
                else
                {
                    Li = COLOR_BLACK;
                }
            }
            else
            {
                Li = COLOR_BLACK;
            }
        }

        Ray3f shadow_ray = Ray3f(closure.its->p,
                                 eqr.wi,
                                 Epsilon,
                                 sqrt(eqr.sqr_dist) - Epsilon);

        Intersection tmp;

        Color3f transmittance = Color3f(1.f);

        if (!intersect_transmittance(shadow_ray,
                                     closure.media,
                                     tmp,
                                     closure.sampler,
                                     transmittance,
                                     closure.last_event != VERTEX_MEDIUM,
                                     true) ||
            global_params.ignore_shadow_checks)
        {
            if (!in_media)
            {
                Float cos_term = closure.its->s_frame.n % eqr.wi;

                if (cos_term < -Epsilon) cos_term = -cos_term;

                Li *= cos_term;
            }

            closure.shadow_rays[0].valid = true;
            closure.shadow_rays[0].shadow_ray = closure.its->toLocal(eqr.wi);

            if (emitter_pdf == 0.f)
            {
                closure.shadow_rays[i].throughput = Color3f(0.f);
            }
            else
            {
                closure.shadow_rays[i].throughput = Li / (choice_pdf * emitter_pdf) * transmittance;
                // closure.shadow_rays[i].throughput = Li;
            }

            // Note: bsdf_values are fully accumulated later
        }
    }
}

Emitter* Scene::choose_emitter(MaterialClosure& closure, Float* pdf) const
{
    Float choice_pdf;
    int emitter;

    light_selection->sampleEmitter(closure.its->p,
                                   closure.sampler,
                                   emitter,
                                   choice_pdf);

    // check to make sure there is no double counting
    // TODO: maybe make this its own function?
    if (emitters[emitter]->getMeshNode())
    {
        unsigned int emit_id = emitters[emitter]->getMeshNode()->mesh->getInstID();
        unsigned int mesh_id = closure.its->intersected_mesh->getInstID();
        *pdf = choice_pdf;

        if (emit_id == mesh_id) return nullptr;
    }

    *pdf = choice_pdf;
    return emitters[emitter];
}

// uniformly returns an emitter (used for light tracing / photon mapping)
Emitter* Scene::choose_emitter(Sampler* sampler, Float* pdf) const
{
    Float val = sampler->next1D();

    int emitter = int(val * emitters.size());
    *pdf = 1.0 / Float(emitters.size());

    return emitters[emitter];
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

FEIGN_END()
