#pragma once

#include <feign/common.h>
#include <feign/core/camera.h>
#include <feign/core/sampler.h>
#include <thread>
#include <functional>

class Integrator;
class Scene;

struct RenderTile
{
    RenderTile(int min_x, int min_y, int max_x, int max_y)
        : min_x(min_x),
          min_y(min_y),
          max_x(max_x),
          max_y(max_y),
          done(false)
    {
        int size = (max_x - min_x) * (max_y - min_y);
        pixels = std::vector<Color3f>(size);
        weights = std::vector<Float>(size);
    }

    void add_radiance(Color3f rad, int i, int j);
    void add_weight(Float wei, int i, int j);

    static void evaluate(RenderTile* tile,
                         const Scene* scene,
                         const Integrator* intergator,
                         const Camera* camera,
                         Sampler* sampler);

    void finalize(Imagef& image);

    std::vector<Color3f> pixels;
    std::vector<Float> weights;

    // this tile represents all pixels in the range [min, max)
    bool done;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};


// this struct handles all of the multi-threading necessary for rendering
// complex scenes. currently this only supports rendering one frame at a
// time but in the future this should be extended to do batch renders
struct RenderPool
{
public:
    RenderPool(int num_threads, int tile_width);
    ~RenderPool();

    void initialize_pool(int im_w, int im_h, int dim);
    void evaluate_pool(const Scene* scene,
                       const Integrator* integrator,
                       const Camera* camera,
                       Sampler* sampler);
    void accumulate_result(Imagef& image);

protected:

    // int get_next_free_index() const;

    std::vector<std::thread> threads;
    std::vector<RenderTile*> tiles;
    std::vector<RenderTile*> active_tiles;
    // bool* free_threads;
    int tile_width;
    int width;
    int height;
};
