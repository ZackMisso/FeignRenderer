#include <feign/integrators/normal.h>
#include <feign/scene.h>

NormalIntegrator::NormalIntegrator() : Integrator() { }

NormalIntegrator::NormalIntegrator(Node* parent) : Integrator(parent) { }

void NormalIntegrator::preProcess()
{
    preProcessChildren();
}

Color3f NormalIntegrator::Li(const Scene* scene,
                             Sampler* sampler,
                             const Ray3f& ray) const
{
    // cout << "Whoo" << endl;
    Intersection its;

    // cout << "intersect" << endl;
    if (!scene->intersect(ray, its))
    {
        // cout << "huh" << endl;
        return Color3f(0.f);
    }

    // cout << "Post Intersect" << endl;

    Normal3f n = ~(its.s_frame.n);
    return Color3f(n(0), n(1), n(2));
}

string NormalIntegrator::getName() const
{
    return Integrator::getName() + "normal";
}
