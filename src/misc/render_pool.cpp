#include <feign/misc/render_pool.h>
#include <feign/core/integrator.h>
#include <feign/core/scene.h>
#include <feign/math/bbox.h>
#include <unistd.h>

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

void RenderTile::evaluate(RenderTile* tile,
                          const Scene* scene,
                          const Integrator* integrator,
                          const Camera* camera,
                          Sampler* sampler)
{
    int index = 0;

    for (int i = tile->min_y; i < tile->max_y; ++i)
    {
        for (int j = tile->min_x; j < tile->max_x; ++j)
        {
            for (int k = 0; k < sampler->getSampleCnt(); ++k)
            {
                // std::cout << sampler->getSampleCnt() << std::endl;
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
                                   Point2f(tile->max_x, tile->max_y));

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

                        tile->add_radiance(rad, fi, fj);
                        tile->add_weight(weight, fi, fj);
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

RenderPool::RenderPool(int num_threads, int tile_width)
    : tile_width(tile_width)
{
    threads = std::vector<std::thread>(num_threads);
    tiles = std::vector<RenderTile*>();
    active_tiles = std::vector<RenderTile*>(num_threads);
    // free_threads = new bool[num_threads];
    //
    // for (int i = 0; i < num_threads; ++i)
    //     free_threads[i] = true;
}

RenderPool::~RenderPool()
{
    // below maybe needed for running interactive stuff
    // for (int i = 0; i < threads.size(); ++i)
    // {
    //     if (!freeThreads[i])
    //     {
    //         threads[i].join();
    //     }
    // }

    // delete[] free_threads;

    for (int i = 0; i < tiles.size(); ++i)
    {
        delete tiles[i];
    }
}

void RenderPool::initialize_pool(int im_w, int im_h, int dim)
{
    width = im_w;
    height = im_h;

    for (int i = 0; i < im_h; i += tile_width)
    {
        for (int j = 0; j < im_w; j += tile_width)
        {
            tiles.push_back(new RenderTile(j, i,
                                           std::min(j+tile_width, im_w),
                                           std::max(i+tile_width, im_h)));
        }
    }
}

void RenderPool::evaluate_pool(const Scene* scene,
                               const Integrator* integrator,
                               const Camera* camera,
                               Sampler* sampler)
{
    int completed_tiles = 0;

    std::vector<RenderTile*> tiles_to_do = std::vector<RenderTile*>();

    for (int i = 0; i < tiles.size(); ++i)
    {
        tiles_to_do.push_back(tiles[i]);
    }

    for (int i = 0; i < threads.size(); ++i)
    {
        RenderTile* tile = tiles_to_do[tiles_to_do.size()-1];
        tiles_to_do.pop_back();
        // free_threads[i] = false;
        active_tiles[i] = tile;
        Sampler* tile_sampler = sampler->copy(sampler->next1D() * 100000000);
        threads[i] = std::thread(std::bind(&RenderTile::evaluate,
                                           tile,
                                           scene,
                                           integrator,
                                           camera,
                                           tile_sampler));
    }

    // std::cout << "HERE" << std::endl;

    while (tiles_to_do.size())
    {
        // std::cout << "HERE" << std::endl;
        sleep(1);
        // std::cout << "ack" << std::endl;

        std::cout << "tiles remaining: " << tiles_to_do.size() << std::endl;

        for (int i = 0; i < active_tiles.size(); ++i)
        {
            if (active_tiles[i]->done)
            {
                threads[i].join();
                RenderTile* tile = tiles_to_do[tiles_to_do.size()-1];
                tiles_to_do.pop_back();
                active_tiles[i] = tile;
                Sampler* tile_sampler = sampler->copy(sampler->next1D() * 100000000);
                threads[i] = std::thread(std::bind(&RenderTile::evaluate,
                                                   tile,
                                                   scene,
                                                   integrator,
                                                   camera,
                                                   tile_sampler));
            }
        }
    }

    for (int i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }
}

// int RenderPool::get_next_free_index() const
// {
//     for (int i = 0; i < threads.size(); ++i)
//     {
//         if (free_threads[i]) return i;
//     }
//
//     return -1;
// }

// TODO: for now this will work as if it is offline, but in the future this should
//       also have an implementation for interactive sessions
void RenderPool::accumulate_result(Imagef& image)
{
    for (int i = 0; i < tiles.size(); ++i)
    {
        tiles[i]->finalize(image);
    }
}
