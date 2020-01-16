/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>

Path_Unidirectional_Integrator::Path_Unidirectional_Integrator(FilterNode* filter,
                                                               std::string location,
                                                               long max_time,
                                                               long max_heuristic)
    : Integrator(filter, location, max_time, max_heuristic) { }

void Path_Unidirectional_Integrator::preProcess()
{
    Integrator::preProcess();
}

Color3f Path_Unidirectional_Integrator::Li(const Scene* scene,
                                           Sampler* sampler,
                                           const Ray3f& ray) const
{
    return Color3f(0.f);
    // // TODO: continue from here
    // // return Color3f(0.f);
    //
    // Intersection its;
    //
    // Vector3f d = ray.d;
    //
    // if (!scene->intersect(ray, its))
    // {
    //     // TODO: environment emitter
    //     return Color3f(0.0);
    // }
    //
    // Color3f emit(0.f);
    // Color3f inc(0.f);
    // Color3f diff(0.f);
    //
    // return Color3f(0.f);
    //
    // // if (its.mesh->isEmitter())
    // // {
    // //     // query the emitter
    // //     // emit = eval_emitter
    // // }
    //
    // // get the bsdf
    // // get the emitters
    //
    // // sample the bsdf
    //
    // // if the bsdf is not specular
    // // // next event estimation with shadow connection
    //
    // // Intersection its;
    // //
    // // Vector3f d = ray.dir;
    // //
    // // if (!scene->intersect(ray, its))
    // // {
    // //     return Color3f(0.f);
    // // }
    // //
    // // Color3f emit(0.f);
    // // Color3f inc(0.f);
    // // Color3f diff(0.f);
    // //
    // // if (its->intersected_mesh->isEmitter())
    // // {
    // //     throw new NotImplementedException("path tracer hitable emitters");
    // // }
    // //
    // // const std::vector<Emitter*> emitters = scene->emitters;
    // // const BSDF* bsdf = scene->getShapeBSDF(its.intersected_mesh);
    // //
    // // BSDFQuery bqr(its.toLocal(-dir),
    // //               its.uv,
    // //               its.p);
    // //
    // // Point2f sample = sampler->next2D();
    // // Color3f bsdf_sample = bsdf->sample(bqr, sample);
    // //
    // // float bsdf_pdf = 1.0;
    // //
    // // if (!bsdf->isDelta())
    // // {
    // //     bsdf_pdf = bsdf->pdf(bqr);
    // //     bsdf_sample = bsdf->eval(bqr);
    // // }
    // //
    // // // TODO: incorporate ray depth
    // // Ray3f next_ray(its.p,
    // //                eqr.wi,
    // //                Epsilon,
    // //                std::numeric_limits<float>::infinity());
    // // // next_ray.update();
    // //
    // // float cos_term = its.s_frame.n % eqr.wi;
    // //
    // // if (cos_term < -Epsilon) cos_term = -cos_term;
    // //
    // // if (bsdf_pdf == 0.f) return emit + diff;
    // //
    // // if (sampler->next1D() < 0.1)
    // // {
    // //     return emit + diff;
    // // }
    // //
    // // return emit + diff + bsdf_sample * cos_term * Li(scene, sampler, next_ray) / (0.9 * bsdf_pdf);
}
