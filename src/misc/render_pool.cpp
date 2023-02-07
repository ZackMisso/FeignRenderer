/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/misc/render_pool.h>
#include <feign/core/integrator.h>
#include <feign/core/scene.h>
#include <feign/math/bbox.h>
#include <unistd.h>

FEIGN_BEGIN()

// TODO: implement adaptive sampling for video compositing
void RenderTile::evaluate(RenderTile *tile,
                          const Scene *scene,
                          const Integrator *integrator,
                          const Camera *camera,
                          Sampler *sampler,
                          Imagef *image,
                          std::vector<Float> *weights,
                          std::mutex *mutexes)
{
    int index = 0;

    for (int i = tile->min_y; i < tile->max_y; ++i)
    {
        // Float percent_done = Float(i - tile->min_y) / Float(tile->max_y - tile->min_y);
        // LOG("percent_done: " + std::to_string(percent_done));
        for (int j = tile->min_x; j < tile->max_x; ++j)
        {
            for (int k = 0; k < sampler->getSampleCnt(); ++k)
            {
                // if (tile->tile_index == 475) std::cout << "sample: " << k << std::endl;
                // if (tile->tile_index == 475) std::cout << "i: " << i << " j: " << j << std::endl;
                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                Ray3f ray;
                Color3f rad = camera->sampleRay(ray, pixelSample, apertureSample);

                // TODO: get multi-threaded clocking working
                // #if CLOCKING
                //     Clocker::startClock("integrator");
                // #endif

                // if (tile->tile_index == 475)
                // {
                //     std::cout << "calling rad: " << tile->tile_index << std::endl;
                //     rad *= integrator->Li(scene, sampler, ray, true);
                //     std::cout << "rad: " << rad[0] << " " << rad[1] << " " << rad[2] << std::endl;
                // }
                // else
                // {
                rad *= integrator->Li(scene, sampler, ray);
                // }
                // if (tile->tile_index == 475) std::cout << "rad: " << rad[0] << " " << rad[1] << " " << rad[2] << std::endl;

                // if (rad.isNan())
                // {
                //     LOG("nan estimate at: " + STR(i) + " " + STR(j));
                // }
                //
                // if (rad.isInf())
                // {
                //     LOG("inf estimate at: " + STR(i) + " " + STR(j));
                // }

                // TODO: get multi-threaded clocking working
                // #if CLOCKING
                //     Clocker::endClock("integrator");
                // #endif

                BBox2f filter_bounds = BBox2f(pixelSample - integrator->filter->filter->getSize(),
                                              pixelSample + integrator->filter->filter->getSize());

                filter_bounds.clip(Point2f(0, 0),
                                   Point2f(image->width() - 1, image->height() - 1));

                // TODO: get multi-threaded clocking working
                // #if CLOCKING
                //     Clocker::startClock("filter");
                // #endif

                // LOG(STR(std::floor(filter_bounds.min(1))));
                // LOG(STR(std::floor(filter_bounds.max(1))));
                // LOG(STR(std::floor(filter_bounds.min(0))));
                // LOG(STR(std::floor(filter_bounds.max(0))));

                // if ()

                for (int fi = std::floor(filter_bounds.min(1));
                     fi <= std::floor(filter_bounds.max(1)); ++fi)
                {
                    for (int fj = std::floor(filter_bounds.min(0));
                         fj <= std::floor(filter_bounds.max(0)); ++fj)
                    {
                        Float weight = integrator->filter->filter->evaluate(Point2f(fj + 0.5, fi + 0.5) -
                                                                            pixelSample);

                        // if (std::floor(filter_bounds.min(1) == 167) &&
                        //     std::floor(filter_bounds.min(0) == 101))
                        // {
                        //     LOG("pixek Sample: " + STR(pixelSample));
                        //     LOG("weight: " + STR(weight));
                        //     LOG("min y: " + STR(std::floor(filter_bounds.min(1))));
                        //     LOG("min x: " + STR(std::floor(filter_bounds.min(0))));
                        //     assert(false);
                        // }
                        // if (weight < Epsilon)
                        // {
                        //     LOG(STR(integrator->filter->filter->getSize()));
                        //     LOG("WEIGHT WHAT");
                        //     LOG(STR(fj));
                        //     LOG(STR(fi));
                        //     assert(false);
                        // }

                        // if (std::isinf(weight) || std::isnan(weight))
                        // {
                        //     LOG("WHAT");
                        // }

                        // LOG(STR(rad));

                        int index = fi * image->width() + fj;

                        mutexes[index].lock();

                        (*image)(fj, fi, 0) += rad(0) * weight;
                        (*image)(fj, fi, 1) += rad(1) * weight;
                        (*image)(fj, fi, 2) += rad(2) * weight;

                        (*weights)[index] += weight;

                        mutexes[index].unlock();
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

    delete sampler;
    tile->done = true;

    // std::cout << "tile done: " << tile->tile_index << std::endl;

    if (is_verbose)
        std::cout << "tile done: " << tile->tile_index << std::endl;
}

RenderPool::RenderPool(int num_threads, int tile_width)
    : tile_width(tile_width)
{
    threads = std::vector<std::thread>(num_threads);
    tiles = std::vector<RenderTile *>();
    active_tiles = std::vector<RenderTile *>(num_threads);

    for (int i = 0; i < num_threads; ++i)
        active_tiles[i] = nullptr;
}

RenderPool::~RenderPool()
{
    threads.clear();
    active_tiles.clear();

    for (int i = 0; i < tiles.size(); ++i)
    {
        if (tiles[i])
            delete tiles[i];
    }
}

void RenderPool::initialize_pool(int im_w, int im_h)
{
    width = im_w;
    height = im_h;

    int index = 0;

    for (int i = 0; i < im_h; i += tile_width)
    {
        for (int j = 0; j < im_w; j += tile_width)
        {
            tiles.push_back(new RenderTile(j, i,
                                           std::min(j + tile_width, im_w),
                                           std::min(i + tile_width, im_h),
                                           index++));
        }
    }
}

void RenderPool::evaluate_pool(const Scene *scene,
                               const Integrator *integrator,
                               const Camera *camera,
                               Sampler *sampler,
                               Imagef &image)
{
    int completed_tiles = 0;

    std::vector<RenderTile *> tiles_to_do = std::vector<RenderTile *>();

    std::mutex *mutexes = new std::mutex[image.width() * image.height()];
    std::vector<Float> weights = std::vector<Float>(image.width() * image.height());

    for (int i = 0; i < weights.size(); ++i)
    {
        weights[i] = 0.0;
    }

    for (int i = 0; i < tiles.size(); ++i)
    {
        tiles_to_do.push_back(tiles[i]);
    }

    for (int i = 0; i < threads.size(); ++i)
    {
        RenderTile *tile = tiles_to_do[tiles_to_do.size() - 1];
        tiles_to_do.pop_back();
        active_tiles[i] = tile;
        Sampler *tile_sampler = sampler->copy(sampler->next1D() * 100000000);
        threads[i] = std::thread(std::bind(&RenderTile::evaluate,
                                           tile,
                                           scene,
                                           integrator,
                                           camera,
                                           tile_sampler,
                                           &image,
                                           &weights,
                                           mutexes));
    }

    while (tiles_to_do.size())
    {
        for (int i = 0; i < active_tiles.size(); ++i)
        {
            if (active_tiles[i]->done)
            {
                threads[i].join();

                if (tiles_to_do.size())
                {
                    RenderTile *tile = tiles_to_do[tiles_to_do.size() - 1];
                    tiles_to_do.pop_back();
                    active_tiles[i] = tile;
                    Sampler *tile_sampler = sampler->copy(sampler->next1D() * 100000000);
                    threads[i] = std::thread(std::bind(&RenderTile::evaluate,
                                                       tile,
                                                       scene,
                                                       integrator,
                                                       camera,
                                                       tile_sampler,
                                                       &image,
                                                       &weights,
                                                       mutexes));
                }

                // std::cout << tiles_to_do.size() << " tiles left to render" << std::endl;
            }
        }
    }

    for (int i = 0; i < threads.size(); ++i)
    {
        // std::cout << "joining threads: " << i << std::endl;
        try
        {
            threads[i].join();
        }
        catch (const std::exception &ex)
        {
        }
        catch (...)
        {
            LOG("double thread join bug.... to fix later");
        }
    }

    delete[] mutexes;

    // std::cout << "whoop" << std::endl;

    // accumulate the results here
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int index = i * image.width() + j;

            if (weights[index] != 0.f)
            {
                image(j, i, 0) = image(j, i, 0) / weights[index];
                image(j, i, 1) = image(j, i, 1) / weights[index];
                image(j, i, 2) = image(j, i, 2) / weights[index];
            }
        }
    }

    LOG("all render tasks completed");
}

FEIGN_END()
