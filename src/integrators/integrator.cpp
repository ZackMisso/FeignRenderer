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

                radiance *= Li(scene, sampler, ray);

                // TODO: use reconstruction filters in the future
                image[0] += radiance(0) / float(sampler->getSampleCnt());
                image[1] += radiance(1) / float(sampler->getSampleCnt());
                image[2] += radiance(2) / float(sampler->getSampleCnt());
            }
        }
    }
}

string Integrator::getName() const
{
    return "integrator: ";
}

NodeType Integrator::getNodeType() const
{
    return NT_Integrator;
}
