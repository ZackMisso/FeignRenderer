#include <feign/integrators/integrator.h>
#include <feign/scene.h>

Integrator::Integrator() : Node() { }

Integrator::Integrator(Node* parent) : Node(parent) { }

Integrator::~Integrator() { }

// TODO: multithreading
void Integrator::render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const
{
    image.clear();

    for (int k = 0; k < sampler->getSampleCnt(); ++k)
    {
        // std::cout << "sample: " << k << std::endl;
        // std::cout << double(k) / double(sampler->getSampleCnt()) * 100.0 << " Percent Done" << std::endl;

        for (int i = 0; i < camera->getFilmSize()[1]; ++i)
        {
            // std::cout << double(k * camera->getFilmSize()[1] * camera->getFilmSize()[0] +
            //              i * camera->getFilmSize()[0]) /
            //              double(camera->getFilmSize()[0] * camera->getFilmSize()[1] * sampler->getSampleCnt())
            //              * 100.0 << " Percent Done" << std::endl;

            for (int j = 0; j < camera->getFilmSize()[0]; ++j)
            {
                // std::cout << double(k * camera->getFilmSize()[1] * camera->getFilmSize()[0] +
                //              i * camera->getFilmSize()[0] + j) /
                //              double(camera->getFilmSize()[0] * camera->getFilmSize()[1] * sampler->getSampleCnt())
                //              * 100.0 << " Percent Done" << std::endl;

                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                Ray3f ray;
                Color3f radiance = camera->sampleRay(ray, pixelSample, apertureSample);

                radiance *= Li(scene, sampler, ray);

                // TODO: use reconstruction filters in the future
                image(j, i, 0) += radiance(0); // / float(sampler->getSampleCnt());
                image(j, i, 1) += radiance(1); // / float(sampler->getSampleCnt());
                image(j, i, 2) += radiance(2); // / float(sampler->getSampleCnt());
            }
        }
    }

    std::cout << "sample count: " << sampler->getSampleCnt() << std::endl;
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = image[i] / Float(sampler->getSampleCnt());
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
