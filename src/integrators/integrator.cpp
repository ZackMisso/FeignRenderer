/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/recon_filter.h>
#include <feign/core/scene.h>
#include <feign/stats/clocker.h>
#include <feign/misc/render_pool.h>

FEIGN_BEGIN()

void Integrator::preProcess(const Scene *scene, Sampler *sampler)
{
    if (!filter)
        LOG("filter node never created");
    if (!filter->filter)
        LOG("filter object never created");
    filter->filter->preProcess();
}

// TODO: multithreaded implementation still needs to get clocking support implemented
void Integrator::render_fast(const Scene *scene,
                             const Camera *camera,
                             Sampler *sampler,
                             Imagef &image) const
{
    // TODO: make threads and tile size a configurable parameter
    RenderPool *pool = new RenderPool(NTHREADS, 64);
    pool->initialize_pool(image.width(), image.height());
    pool->evaluate_pool(scene, this, camera, sampler, image);
    delete pool;
}

void Integrator::render(const Scene *scene,
                        const Camera *camera,
                        Sampler *sampler,
                        Imagef &image) const
{
    image.clear();
    Imagef filter_weights = Imagef(image.width(), image.height());
    filter_weights.clear();

    for (int k = 0; k < sampler->getSampleCnt(); ++k)
    {
        for (int i = 0; i < camera->getFilmSize()[1]; ++i)
        {
            for (int j = 0; j < camera->getFilmSize()[0]; ++j)
            {
                CLOCKER_START_ONE(ClockerType::CAMERA_RAY)

                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                Ray3f ray;
                Color3f radiance = camera->sampleRay(ray, pixelSample, apertureSample);

                CLOCKER_START_STOP_ONE(ClockerType::INTEGRATOR,
                                       ClockerType::CAMERA_RAY)

                radiance *= Li(scene, sampler, ray);

                if (radiance.isNan())
                {
                    LOG("nan estimate at: " + STR(i) + " " + STR(j));
                }

                if (radiance.isInf())
                {
                    LOG("inf estimate at: " + STR(i) + " " + STR(j));
                }

                CLOCKER_START_STOP_ONE(ClockerType::FILTER,
                                       ClockerType::INTEGRATOR)

                BBox2f filter_bounds = BBox2f(pixelSample - filter->filter->getSize(),
                                              pixelSample + filter->filter->getSize());

                filter_bounds.clip(Point2f(0.0, 0.0),
                                   Point2f(camera->getFilmSize()[0] - 1, camera->getFilmSize()[1] - 1));

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

                        filter_weights(fj, fi, 0) += weight;
                    }
                }

                CLOCKER_STOP_ONE(ClockerType::FILTER)
            }
        }
    }

    CLOCKER_START_ONE(ClockerType::FILTER)

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

    CLOCKER_STOP_ONE(ClockerType::FILTER)
}

FEIGN_END()
