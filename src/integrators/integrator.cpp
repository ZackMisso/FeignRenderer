/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/recon_filter.h>
#include <feign/core/scene.h>
#include <feign/stats/clocker.h>

void Integrator::preProcess()
{
    if (!filter) LOG("filter node never created");
    if (!filter->filter) LOG("filter object never created");
    filter->filter->preProcess();
}

// TODO: multithreading
void Integrator::render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const
{
    LOG("starting render loop");

    image.clear();
    Imagef filter_weights = Imagef(image.width(), image.height(), 1);
    filter_weights.clear();

    for (int k = 0; k < sampler->getSampleCnt(); ++k)
    {
        LOG("sample: " + std::to_string(k));

        // std::cout << double(k) / double(sampler->getSampleCnt()) * 100.0 << " Percent Done" << std::endl;

        for (int i = 0; i < camera->getFilmSize()[1]; ++i)
        {
            for (int j = 0; j < camera->getFilmSize()[0]; ++j)
            {
                // for (int k = 0; k < sampler->getSampleCnt(); ++k)
                // {
                    // LOG("sample: " + std::to_string(k));
        // for (int i = 126; i < 127; ++i)
        // {
        //     for (int j = 128; j < 129; ++j)
        //     {
                // LOG("taking pixel and apeture samples");
                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                // LOG("sampling ray");
                Ray3f ray;
                Color3f radiance = camera->sampleRay(ray, pixelSample, apertureSample);

                // LOG("ray origin");
                // LOG(ray.origin);
                // LOG("dir");
                // LOG(ray.dir);

                // LOG("calculating radiance");
                #if CLOCKING
                    Clocker::startClock("integrator");
                #endif

                radiance *= Li(scene, sampler, ray);

                #if CLOCKING
                    Clocker::endClock("integrator");
                #endif

                // if (std::isnan(radiance[0]))
                // {
                //     LOG("Radiance is nan");
                // }

                // LOG("getting filter bounds");
                BBox2f filter_bounds = BBox2f(pixelSample - filter->filter->getSize(),
                                              pixelSample + filter->filter->getSize());

                filter_bounds.clip(Point2f(0.0, 0.0),
                                   Point2f(camera->getFilmSize()[0]-1, camera->getFilmSize()[1]-1));

                // LOG("splatting filter");
                #if CLOCKING
                    Clocker::startClock("filter");
                #endif

                for (int fi = std::floor(filter_bounds.min(1));
                     fi <= std::floor(filter_bounds.max(1)); ++fi)
                {
                    for (int fj = std::floor(filter_bounds.min(0));
                         fj <= std::floor(filter_bounds.max(0)); ++fj)
                    {
                        Float weight = filter->filter->evaluate(Point2f(fj + 0.5, fi + 0.5) -
                                                                pixelSample);



                        image(fj, fi, 0) += weight * radiance(0);
                        image(fj, fi, 1) += weight * radiance(1);
                        image(fj, fi, 2) += weight * radiance(2);

                        // if (std::isnan(image(fj, fi, 0)))
                        // {
                        //     // GaussFilter* debug = (GaussFilter*)filter;
                        //     // LOG("filter alpha: " std::to_string(filter->alpha));
                        //     LOG("nan setting");
                        //     LOG("weight: " + std::to_string(weight));
                        //     LOG("radiance: " + std::to_string(radiance(0)));
                        // }

                        filter_weights(fj, fi, 0) += weight;
                    }
                }

                #if CLOCKING
                    Clocker::endClock("filter");
                #endif
            }
        }
    }

    #if CLOCKING
        Clocker::startClock("filter");
    #endif


    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            if (filter_weights(j, i, 0) == 0.f)
            {
                image(j, i, 0) = 0.f;
                image(j, i, 1) = 0.f;
                image(j, i, 2) = 0.f;
            }
            else
            {
                image(j, i, 0) = image(j, i, 0) / filter_weights(j, i, 0);
                image(j, i, 1) = image(j, i, 1) / filter_weights(j, i, 0);
                image(j, i, 2) = image(j, i, 2) / filter_weights(j, i, 0);
            }
        }
    }

    #if CLOCKING
        Clocker::endClock("filter");
    #endif
}
