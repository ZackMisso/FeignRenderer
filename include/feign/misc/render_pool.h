/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/camera.h>
#include <feign/core/sampler.h>
#include <thread>
#include <mutex>
#include <functional>

FEIGN_BEGIN()

class Integrator;
class Scene;

struct RenderTile
{
    RenderTile(int min_x, int min_y, int max_x, int max_y, int tile_index)
        : min_x(min_x),
          min_y(min_y),
          max_x(max_x),
          max_y(max_y),
          tile_index(tile_index),
          done(false)
    {
        int size = (max_x - min_x) * (max_y - min_y);
    }

    static void evaluate(RenderTile* tile,
                         const Scene* scene,
                         const Integrator* intergator,
                         const Camera* camera,
                         Sampler* sampler,
                         Imagef* image,
                         std::vector<Float>* weights,
                         std::mutex* mutexes);

    // this tile represents all pixels in the range [min, max)
    bool done;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    int tile_index;
};


// this struct handles all of the multi-threading necessary for rendering
// complex scenes. currently this only supports rendering one frame at a
// time but in the future this should be extended to do batch renders
struct RenderPool
{
public:
    RenderPool(int num_threads, int tile_width);
    ~RenderPool();

    void initialize_pool(int im_w, int im_h);
    void evaluate_pool(const Scene* scene,
                       const Integrator* integrator,
                       const Camera* camera,
                       Sampler* sampler,
                       Imagef& image);

protected:
    std::vector<std::thread> threads;
    std::vector<RenderTile*> tiles;
    std::vector<RenderTile*> active_tiles;
    int tile_width;
    int width;
    int height;
};

FEIGN_END()
