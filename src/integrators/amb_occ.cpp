#include <feign/integrators/amb_occ.h>
#include <feign/math/warp.h>
#include <feign/scene.h>

Ambient_Occlusion_Integrator::Ambient_Occlusion_Integrator()
{
    // does nothing
}

Ambient_Occlusion_Integrator::Ambient_Occlusion_Integrator(Node* parent)
{
    // does nothing
}

void Ambient_Occlusion_Integrator::preProcess()
{
    Integrator::preProcess();

    preProcessChildren();
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
    Vector3f sample_dir = WarpSpace::squareToUniformHemisphere(point);
    Float pdf = WarpSpace::squareToCosineHemispherePdf(sample_dir);

    Ray3f shadow_ray(its.p,
                     its.toWorld(sample_dir),
                     Epsilon,
                     std::numeric_limits<Float>::infinity());

    Float cosTerm = its.s_frame.n % its.toWorld(sample_dir);

    return Color3f(cosTerm * INV_PI / pdf);
}

std::string Ambient_Occlusion_Integrator::getName() const
{
    return Integrator::getName() + "amb_occ";
}
