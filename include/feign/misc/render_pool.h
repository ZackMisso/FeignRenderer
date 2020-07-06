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
#include <functional>

FEIGN_BEGIN()

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
        // int size = (max_x - min_x + 2) * (max_y - min_y + 2);
        // std::cout << "SIZE: " << size << std::endl;
        pixels = new Color3f[size]();
        weights = new Float[size]();

        for (int i = 0; i < size; ++i)
        {
            pixels[i] = Color3f(0.f);
            weights[i] = 0.f;
        }
    }

    ~RenderTile()
    {
        // LOG("deleting tile ~ fox");
        // pixels = std::vector<Color3f>();
        // LOG(std::to_string(pixels.size()));
        // pixels.clear();
        delete[] pixels;
        // LOG("pixels deleted");
        delete[] weights;
        // LOG("deleted tile");
    }

    void add_radiance(Color3f rad, int i, int j);
    void add_weight(Float wei, int i, int j);

    static void evaluate(RenderTile* tile,
                         const Scene* scene,
                         const Integrator* intergator,
                         const Camera* camera,
                         Sampler* sampler);

    void finalize(Imagef& image);

    Color3f* pixels;
    Float* weights;

    // std::vector<Color3f> pixels;
    // std::vector<Float> weights;

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

    void initialize_pool(int im_w, int im_h);
    void evaluate_pool(const Scene* scene,
                       const Integrator* integrator,
                       const Camera* camera,
                       Sampler* sampler,
                       Imagef& image);
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

FEIGN_END()
