#include <feign/integrators/integrator.h>
#include <feign/scene.h>

Integrator::Integrator() : Node() { }

Integrator::Integrator(Node* parent) : Node(parent) { }

Integrator::~Integrator() { }

void Integrator::render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const
{
    image.clear();

    // cout << "Woo" << endl;
    for (int k = 0; k < sampler->getSampleCnt(); ++k)
    {
        std::cout << double(k) / double(sampler->getSampleCnt()) * 100.0 << " Percent Done" << std::endl;
        // cout << "sampleXont" << endl;
        // cout << sampler->getSampleCnt() << endl;
        for (int i = 0; i < camera->getFilmSize()[1]; ++i)
        {
            for (int j = 0; j < camera->getFilmSize()[0]; ++j)
            {
                // cout << "Here" << endl;

                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                Ray3f ray;
                Color3f radiance = camera->sampleRay(ray, pixelSample, apertureSample);
                // cout << "ray.dir before: " << ray.dir[0] << " " << ray.dir[1] << " " << ray.dir[2] << endl;

                // cout << "Pre LI" << endl;
                radiance *= Li(scene, sampler, ray);
                // cout << "Post LI" << endl;

                // TODO: use reconstruction filters in the future
                image(j, i, 0) += radiance(0) / float(sampler->getSampleCnt());
                image(j, i, 1) += radiance(1) / float(sampler->getSampleCnt());
                image(j, i, 2) += radiance(2) / float(sampler->getSampleCnt());
            }
        }
    }
}

std::string Integrator::getName() const
{
    return "integrator: ";
}

NodeType Integrator::getNodeType() const
{
    return NT_Integrator;
}
