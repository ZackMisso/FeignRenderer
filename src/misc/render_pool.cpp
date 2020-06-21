#include <feign/misc/render_pool.h>
#include <feign/core/integrator.h>
#include <feign/core/scene.h>
#include <feign/math/bbox.h>

void RenderTile::add_radiance(Color3f rad, int i, int j)
{
    int wid = max_x - min_x;
    int hei = max_y - min_y;

    int index = (i - min_y) * wid + (j - min_x);

    pixels[index] += rad;
}

void RenderTile::add_weight(Float wei, int i, int j)
{
    int wid = max_x - min_x;
    int hei = max_y - min_y;

    int index = (i - min_y) * wid + (j - min_x);

    weights[index] += wei;
}

void RenderTile::evaluate(const Scene* scene,
                          const Integrator* integrator,
                          const Camera* camera,
                          Sampler* sampler)
{
    int index = 0;

    for (int i = min_y; i < max_y; ++i)
    {
        for (int j = min_x; j < max_x; ++j)
        {
            for (int k = 0; k < sampler->getSampleCnt(); ++k)
            {
                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                Ray3f ray;
                Color3f rad = camera->sampleRay(ray, pixelSample, apertureSample);

                // TODO: get multi-threaded clocking working
                // #if CLOCKING
                //     Clocker::startClock("integrator");
                // #endif

                rad *= integrator->Li(scene, sampler, ray);

                // TODO: get multi-threaded clocking working
                // #if CLOCKING
                //     Clocker::endClock("integrator");
                // #endif

                BBox2f filter_bounds = BBox2f(pixelSample - integrator->filter->filter->getSize(),
                                              pixelSample + integrator->filter->filter->getSize());

                filter_bounds.clip(Point2f(0.0, 0.0),
                                   Point2f(camera->getFilmSize()[0]-1, camera->getFilmSize()[1]-1));

                // TODO: get multi-threaded clocking working
                // #if CLOCKING
                //     Clocker::startClock("filter");
                // #endif

                for (int fi = std::floor(filter_bounds.min(1));
                     fi <= std::floor(filter_bounds.max(1)); ++fi)
                {
                    for (int fj = std::floor(filter_bounds.min(0));
                         fj <= std::floor(filter_bounds.max(0)); ++fj)
                    {
                        Float weight = integrator->filter->filter->evaluate(Point2f(fj + 0.5, fi + 0.5) -
                                                                            pixelSample);

                        add_radiance(rad, fi, fj);
                        add_weight(weight, fi, fj);
                    }
                }

                // TODO: get multi-threaded clocking working
                // #if CLOCKING
                //     Clocker::endClock("filter");
                // #endif
            }

            ++index;
        }
    }
    // TODO
}

void RenderTile::finalize(Imagef& image)
{
    int wid = max_x - min_x;
    int hei = max_y - min_y;
    int index = 0;

    for (int i = min_y; i < max_y; ++i)
    {
        for (int j = min_x; j < max_x; ++j)
        {
            image(j, i, 0) = pixels[index](0) / weights[index];
            image(j, i, 1) = pixels[index](1) / weights[index];
            image(j, i, 2) = pixels[index](2) / weights[index];

            ++index;
        }
    }
}

RenderPool::RenderPool(int num_threads)
{
    // TODO
}

RenderPool::~RenderPool()
{
    // TODO
}

void RenderPool::initialize_pool(int im_w, int im_h, int dim)
{
    // TODO
}

void RenderPool::evaluate_pool(const Scene* scene,
                               const Integrator* integrator,
                               const Camera* camera,
                               Sampler* sampler)
{
    // TODO
}

Imagef RenderPool::accumulate_result()
{
    // TODO
}
