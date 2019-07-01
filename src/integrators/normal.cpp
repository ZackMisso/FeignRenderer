#include <feign/integrators/normal.h>
#include <feign/scene.h>

NormalIntegrator::NormalIntegrator() : Integrator() { }

NormalIntegrator::NormalIntegrator(Node* parent) : Integrator(parent) { }

void NormalIntegrator::preProcess()
{
    Integrator::preProcess();
    
    preProcessChildren();
}

Color3f NormalIntegrator::Li(const Scene* scene,
                             Sampler* sampler,
                             const Ray3f& ray) const
{
    Intersection its;

    if (!scene->intersect(ray, its))
    {
        return Color3f(0.f);
    }

    Normal3f geom_n = ~(its.g_frame.n);
    Normal3f shad_n = ~(its.s_frame.n);

    // std::cout << "Geom: " << geom_n(0) << " " << geom_n(1) << " " << geom_n(2) << std::endl;
    // std::cout << "Shad: " << shad_n(0) << " " << shad_n(1) << " " << shad_n(2) << std::endl;
    // std::cout << std::endl;

    return Color3f(shad_n(0), shad_n(1), shad_n(2));
}

std::string NormalIntegrator::getName() const
{
    return Integrator::getName() + "normal";
}
