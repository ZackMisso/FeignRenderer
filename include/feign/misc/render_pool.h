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
          max_y(max_y)
    {
        int size = (max_x - min_x) * (max_y - min_y);
        pixels = std::vector<Color3f>(size);
        weights = std::vector<Float>(size);
    }

    void add_radiance(Color3f rad, int i, int j);
    void add_weight(Float wei, int i, int j);
    void evaluate(const Scene* scene,
                  const Integrator* intergator,
                  const Camera* camera,
                  Sampler* sampler);
    void finalize(Imagef& image);

    std::vector<Color3f> pixels;
    std::vector<Float> weights;
    // this is not inclusive
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
    RenderPool(int num_threads);
    ~RenderPool();

    void initialize_pool(int im_w, int im_h, int dim);
    void evaluate_pool(const Scene* scene,
                       const Integrator* integrator,
                       const Camera* camera,
                       Sampler* sampler);
    Imagef accumulate_result();

protected:
    std::vector<std::thread> threads;
    std::vector<RenderTile*> tiles;
    std::vector<RenderTile*> activeTiles;
    bool* freeThreads;
};
