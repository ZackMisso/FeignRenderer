/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/recon_filter.h>
#include <feign/core/scene.h>

Integrator::Integrator() : Node()
{
    filter = nullptr;
}

Integrator::Integrator(Node* parent) : Node(parent)
{
    filter = nullptr;
}

Integrator::~Integrator() { std::cout << "deleting integrator" << std::endl; }

void Integrator::preProcess()
{
    if (!filter)
    {
        filter = new GaussFilter(Vec2f(2.0, 2.0), 0.5);
        // filter = new BoxFilter();
        addChild(filter);
    }

    preProcessChildren();
}

// TODO: multithreading
void Integrator::render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const
{
    image.clear();
    Imagef filter_weights = Imagef(image.width(), image.height(), 1);
    filter_weights.clear();

    for (int k = 0; k < sampler->getSampleCnt(); ++k)
    {
        std::cout << double(k) / double(sampler->getSampleCnt()) * 100.0 << " Percent Done" << std::endl;

        for (int i = 0; i < camera->getFilmSize()[1]; ++i)
        {
            for (int j = 0; j < camera->getFilmSize()[0]; ++j)
            {
                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                Ray3f ray;
                Color3f radiance = camera->sampleRay(ray, pixelSample, apertureSample);

                radiance *= Li(scene, sampler, ray);

                BBox2f filter_bounds = BBox2f(pixelSample - filter->getSize(),
                                              pixelSample + filter->getSize());

                filter_bounds.clip(Point2f(0.0, 0.0),
                                   Point2f(camera->getFilmSize()[0]-1, camera->getFilmSize()[1]-1));

                for (int fi = std::floor(filter_bounds.min(1));
                     fi <= std::floor(filter_bounds.max(1)); ++fi)
                {
                    for (int fj = std::floor(filter_bounds.min(0));
                         fj <= std::floor(filter_bounds.max(0)); ++fj)
                    {
                        Float weight = filter->evaluate(Point2f(fj + 0.5, fi + 0.5) -
                                                        pixelSample);

                        image(fj, fi, 0) += weight * radiance(0);
                        image(fj, fi, 1) += weight * radiance(1);
                        image(fj, fi, 2) += weight * radiance(2);

                        filter_weights(fj, fi, 0) += weight;
                    }
                }
            }
        }
    }

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            image(j, i, 0) = image(j, i, 0) / filter_weights(j, i, 0);
            image(j, i, 1) = image(j, i, 1) / filter_weights(j, i, 0);
            image(j, i, 2) = image(j, i, 2) / filter_weights(j, i, 0);
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
