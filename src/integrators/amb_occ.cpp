/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>
#include <feign/math/warp.h>

Ambient_Occlusion_Integrator::Ambient_Occlusion_Integrator(long max_time, long max_heuristic)
    : Integrator(max_time, max_heuristic)
{
    // does nothing
}

void Ambient_Occlusion_Integrator::preProcess()
{
    Integrator::preProcess();

    // preProcessChildren();
}

Color3f Ambient_Occlusion_Integrator::Li(const Scene* scene,
                                           Sampler* sampler,
                                           const Ray3f& ray) const
{
    Intersection its;

    Vector3f dir = ray.dir;

    if (!scene->intersect(ray, its))
    {
        return Color3f(0.f);
    }

    Point2f point = sampler->next2D();
    Vector3f sample_dir = WarpSpace::squareToCosineHemisphere(point);
    Float pdf = WarpSpace::squareToCosineHemispherePdf(sample_dir);

    Ray3f shadow_ray(its.p,
                     its.toWorldGeom(sample_dir),
                     Epsilon,
                     std::numeric_limits<Float>::infinity());

    Intersection shadow_its;
    if (!scene->intersect(shadow_ray, shadow_its))
    {
        return Color3f(1.f);
    }

    // // Float cosTerm = its.s_frame.n % its.toWorld(sample_dir);
    //
    // if (sample_dir(2) == 0.0)
    // {
    //     std::cout << "zero z" << std::endl;
    // }
    //
    // // if (pdf == 0.0)
    // // {
    // //
    // // }
    //
    // if (pdf == 0.0)
    // {
    //     std::cout << "ZERO PDF" << std::endl;
    //     std::cout << "sampled dir: " << std::endl;
    //     sample_dir.info();
    // }

    return Color3f(0.0);
}
