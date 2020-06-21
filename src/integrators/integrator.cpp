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

// Integrator::RenderTile(int min_x, int min_y, int max_x, int max_y)
//     : min_x(min_x),
//       min_y(min_y),
//       max_x(max_x),
//       max_y(max_y)
// {
//     int size = (max_x - min_x) * (max_y - min_y);
//     pixels = std::vector<Color3f>(size);
//     weights = std::vector<Float>(size);
// }

void Integrator::renderTile(const Scene* scene,
                            const Camera* camera,
                            Sampler* sampler,
                            RenderTile& tile) const
{
    int index = 0;

    for (int i = tile.min_y; i < tile.max_y; ++i)
    {
        for (int j = tile.min_x; j < tile.max_x; ++j)
        {
            for (int k = 0; k < sampler->getSampleCnt(); ++k)
            {
                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                Ray3f ray;
                Color3f rad = camera->sampleRay(ray, pixelSample, apertureSample);

                // #if CLOCKING
                //     Clocker::startClock("integrator");
                // #endif

                // LOG("pre radiance");

                rad *= Li(scene, sampler, ray);

                // #if CLOCKING
                //     Clocker::endClock("integrator");
                // #endif

                BBox2f filter_bounds = BBox2f(pixelSample - filter->filter->getSize(),
                                              pixelSample + filter->filter->getSize());

                filter_bounds.clip(Point2f(0.0, 0.0),
                                   Point2f(camera->getFilmSize()[0]-1, camera->getFilmSize()[1]-1));

                // #if CLOCKING
                //     Clocker::startClock("filter");
                // #endif

                for (int fi = std::floor(filter_bounds.min(1));
                     fi <= std::floor(filter_bounds.max(1)); ++fi)
                {
                    for (int fj = std::floor(filter_bounds.min(0));
                         fj <= std::floor(filter_bounds.max(0)); ++fj)
                    {
                        Float weight = filter->filter->evaluate(Point2f(fj + 0.5, fi + 0.5) -
                                                                pixelSample);

                        tile.add_radiance(rad, fi, fj);
                        tile.add_weight(weight, fi, fj);
                        // image(fj, fi, 0) += weight * radiance(0);
                        // image(fj, fi, 1) += weight * radiance(1);
                        // image(fj, fi, 2) += weight * radiance(2);
                        //
                        // filter_weights(fj, fi, 0) += weight;
                    }
                }

                // #if CLOCKING
                //     Clocker::endClock("filter");
                // #endif
            }

            ++index;
        }
    }
    // TODO
}

// // TODO: get clocking working for multi-threading
// void Integrator::RenderTile::render(const Scene* scene,
//                                     const Camera* camera,
//                                     const ReconstructionFilter* filter,
//                                     Sampler* sampler)
// {
//     int index = 0;
//
//     for (int i = min_y; i < max_y; ++i)
//     {
//         for (int j = min_x; j < max_x; ++j)
//         {
//             for (int k = 0; k < sampler->getSampleCnt(); ++k)
//             {
//                 Point2f pixelSample = Point2f(j, i) + sampler->next2D();
//                 Point2f apertureSample = sampler->next2D();
//
//                 Ray3f ray;
//                 Color3f rad = camera->sampleRay(ray, pixelSample, apertureSample);
//
//                 // #if CLOCKING
//                 //     Clocker::startClock("integrator");
//                 // #endif
//
//                 // LOG("pre radiance");
//
//                 rad *= Li(scene, sampler, ray);
//
//                 // #if CLOCKING
//                 //     Clocker::endClock("integrator");
//                 // #endif
//
//                 BBox2f filter_bounds = BBox2f(pixelSample - filter->filter->getSize(),
//                                               pixelSample + filter->filter->getSize());
//
//                 filter_bounds.clip(Point2f(0.0, 0.0),
//                                    Point2f(camera->getFilmSize()[0]-1, camera->getFilmSize()[1]-1));
//
//                 // #if CLOCKING
//                 //     Clocker::startClock("filter");
//                 // #endif
//
//                 for (int fi = std::floor(filter_bounds.min(1));
//                      fi <= std::floor(filter_bounds.max(1)); ++fi)
//                 {
//                     for (int fj = std::floor(filter_bounds.min(0));
//                          fj <= std::floor(filter_bounds.max(0)); ++fj)
//                     {
//                         Float weight = filter->evaluate(Point2f(fj + 0.5, fi + 0.5) -
//                                                         pixelSample);
//
//                         add_radiance(rad, fi, fj);
//                         add_weight(weight, fi, fj);
//                         // image(fj, fi, 0) += weight * radiance(0);
//                         // image(fj, fi, 1) += weight * radiance(1);
//                         // image(fj, fi, 2) += weight * radiance(2);
//                         //
//                         // filter_weights(fj, fi, 0) += weight;
//                     }
//                 }
//
//                 // #if CLOCKING
//                 //     Clocker::endClock("filter");
//                 // #endif
//             }
//
//             ++index;
//         }
//     }
//     // TODO
// }

// void Integrator::RenderTile::add_radiance(Color3f rad, int i, int j)
// {
//     int wid = max_x - min_x;
//     int hei = max_y - min_y;
//
//     int index = (i - min_y) * wid + (j - min_x);
//
//     pixels[index] += rad;
// }
//
// void Integrator::RenderTile::add_weight(Float wei, int i, int j)
// {
//     int wid = max_x - min_x;
//     int hei = max_y - min_y;
//
//     int index = (i - min_y) * wid + (j - min_x);
//
//     weights[index] += wei;
// }
//
// void Integrator::RenderTile::finalize(Imagef& image)
// {
//     int wid = max_x - min_x;
//     int hei = max_y - min_y;
//     int index = 0;
//
//     for (int i = min_y; i < max_y; ++i)
//     {
//         for (int j = min_x; j < max_x; ++j)
//         {
//             image(j, i, 0) = pixels[index](0) / weights[index];
//             image(j, i, 1) = pixels[index](1) / weights[index];
//             image(j, i, 2) = pixels[index](2) / weights[index];
//
//             ++index;
//         }
//     }
// }

// TODO: multithreading
void Integrator::render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const
{
    // RenderPool* pool = new RenderPool(1);


    // TODO: before testing multithreading - I need to get automatic unit
    //       testing working to verify nothing breaks

    // image.clear();
    //
    // int tile_width = 32;
    //
    // std::vector<RenderTile> tiles =
    //     std::vector<RenderTile>();
    //
    // int wid = camera->getFilmSize()[0];
    // int hei = camera->getFilmSize()[1];
    //
    // for (int i = 0; i < hei; i += tile_width)
    // {
    //     for (int j = 0; j < wid; j += tile_width)
    //     {
    //         tiles.push_back(RenderTile(j,
    //                                    i,
    //                                    std::min(j+tile_width, wid),
    //                                    std::max(i+tile_width, hei)));
    //     }
    // }

    // TODO: create and start jobs

    image.clear();
    Imagef filter_weights = Imagef(image.width(), image.height(), 1);
    filter_weights.clear();

    for (int k = 0; k < sampler->getSampleCnt(); ++k)
    {
        if (verbose) LOG("sample: " + std::to_string(k));

        // LOG("rendering");

        // for (int i = 141; i < 146; ++i)
        // {
        //     for (int j = 112; j < 118; ++j)

        for (int i = 0; i < camera->getFilmSize()[1]; ++i)
        {
            for (int j = 0; j < camera->getFilmSize()[0]; ++j)
            {
                Point2f pixelSample = Point2f(j, i) + sampler->next2D();
                Point2f apertureSample = sampler->next2D();

                Ray3f ray;
                Color3f radiance = camera->sampleRay(ray, pixelSample, apertureSample);

                #if CLOCKING
                    Clocker::startClock("integrator");
                #endif

                // LOG("pre radiance");

                radiance *= Li(scene, sampler, ray);

                #if CLOCKING
                    Clocker::endClock("integrator");
                #endif

                BBox2f filter_bounds = BBox2f(pixelSample - filter->filter->getSize(),
                                              pixelSample + filter->filter->getSize());

                filter_bounds.clip(Point2f(0.0, 0.0),
                                   Point2f(camera->getFilmSize()[0]-1, camera->getFilmSize()[1]-1));

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

// render method before implementing multithreading
// void Integrator::render(const Scene* scene,
//                         const Camera* camera,
//                         Sampler* sampler,
//                         Imagef& image) const
// {
//     image.clear();
//     Imagef filter_weights = Imagef(image.width(), image.height(), 1);
//     filter_weights.clear();
//
//     for (int k = 0; k < sampler->getSampleCnt(); ++k)
//     {
//         if (verbose) LOG("sample: " + std::to_string(k));
//
//         // LOG("rendering");
//
//         // for (int i = 141; i < 146; ++i)
//         // {
//         //     for (int j = 112; j < 118; ++j)
//
//         for (int i = 0; i < camera->getFilmSize()[1]; ++i)
//         {
//             for (int j = 0; j < camera->getFilmSize()[0]; ++j)
//             {
//                 Point2f pixelSample = Point2f(j, i) + sampler->next2D();
//                 Point2f apertureSample = sampler->next2D();
//
//                 Ray3f ray;
//                 Color3f radiance = camera->sampleRay(ray, pixelSample, apertureSample);
//
//                 #if CLOCKING
//                     Clocker::startClock("integrator");
//                 #endif
//
//                 // LOG("pre radiance");
//
//                 radiance *= Li(scene, sampler, ray);
//
//                 #if CLOCKING
//                     Clocker::endClock("integrator");
//                 #endif
//
//                 BBox2f filter_bounds = BBox2f(pixelSample - filter->filter->getSize(),
//                                               pixelSample + filter->filter->getSize());
//
//                 filter_bounds.clip(Point2f(0.0, 0.0),
//                                    Point2f(camera->getFilmSize()[0]-1, camera->getFilmSize()[1]-1));
//
//                 #if CLOCKING
//                     Clocker::startClock("filter");
//                 #endif
//
//                 for (int fi = std::floor(filter_bounds.min(1));
//                      fi <= std::floor(filter_bounds.max(1)); ++fi)
//                 {
//                     for (int fj = std::floor(filter_bounds.min(0));
//                          fj <= std::floor(filter_bounds.max(0)); ++fj)
//                     {
//                         Float weight = filter->filter->evaluate(Point2f(fj + 0.5, fi + 0.5) -
//                                                                 pixelSample);
//
//                         image(fj, fi, 0) += weight * radiance(0);
//                         image(fj, fi, 1) += weight * radiance(1);
//                         image(fj, fi, 2) += weight * radiance(2);
//
//                         filter_weights(fj, fi, 0) += weight;
//                     }
//                 }
//
//                 #if CLOCKING
//                     Clocker::endClock("filter");
//                 #endif
//             }
//         }
//     }
//
//     #if CLOCKING
//         Clocker::startClock("filter");
//     #endif
//
//
//     for (int i = 0; i < image.height(); ++i)
//     {
//         for (int j = 0; j < image.width(); ++j)
//         {
//             if (filter_weights(j, i, 0) == 0.f)
//             {
//                 image(j, i, 0) = 0.f;
//                 image(j, i, 1) = 0.f;
//                 image(j, i, 2) = 0.f;
//             }
//             else
//             {
//                 image(j, i, 0) = image(j, i, 0) / filter_weights(j, i, 0);
//                 image(j, i, 1) = image(j, i, 1) / filter_weights(j, i, 0);
//                 image(j, i, 2) = image(j, i, 2) / filter_weights(j, i, 0);
//             }
//         }
//     }
//
//     #if CLOCKING
//         Clocker::endClock("filter");
//     #endif
// }
