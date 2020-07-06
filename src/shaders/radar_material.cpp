/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/shader.h>
#include <feign/core/scene.h>
#include <feign/stats/clocker.h>

FEIGN_BEGIN()

RadarMaterialShader::RadarMaterialShader(MaterialNode* radar_mat,
                                         MaterialNode* mesh_mat,
                                         std::vector<Point3f> start_points,
                                         std::vector<Float> end_dist,
                                         std::vector<Float> start_proxies,
                                         std::vector<Float> end_proxies,
                                         Float band_width,
                                         Float fall_off,
                                         Float proxy)
    : radar_mat(radar_mat),
      mesh_mat(mesh_mat),
      start_points(start_points),
      end_dist(end_dist),
      start_proxies(start_proxies),
      end_proxies(end_proxies),
      band_width(band_width),
      fall_off(fall_off),
      proxy(proxy)
{
    assert(start_points.size() == end_dist.size() &&
           start_points.size() == start_proxies.size() &&
           start_points.size() == end_proxies.size());
}

void RadarMaterialShader::sample(MaterialClosure& closure) const
{
    Color3f scale;
    Material* material = choose_mat(closure, scale);
    material->sample(closure);
    closure.albedo *= scale;
}

void RadarMaterialShader::evaluate(MaterialClosure& closure) const
{
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    Color3f scale;
    Material* material = choose_mat(closure, scale);

    closure.is_specular = material->isDelta();

    if (closure.is_specular)
    {
        return;
    }

    if (closure.sample_all_emitters)
    {
        closure.scene->eval_all_emitters(closure);
    }
    else
    {
        closure.scene->eval_one_emitter(closure);
    }

    #if CLOCKING
        Clocker::endClock("shader eval");
    #endif
}

void RadarMaterialShader::evaluate_mat_only(MaterialClosure& closure) const
{
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    Color3f scale;
    Material* material = choose_mat(closure, scale);
    material->evaluate(closure);
    closure.albedo *= scale;

    #if CLOCKING
        Clocker::endClock("shader eval");
    #endif
}

Material* RadarMaterialShader::choose_mat(MaterialClosure& closure,
                                          Color3f& scale) const
{
    std::vector<Float> distances = std::vector<Float>();
    scale = Color3f(0.f);

    bool found = false;

    for (int i = 0; i < start_proxies.size(); ++i)
    {
        if (end_proxies[i] >= proxy && start_proxies[i] <= proxy)
        {
            Float dist = (proxy - start_proxies[i]) /
                         (end_proxies[i] - start_proxies[i]) * end_dist[i];

            distances.push_back(dist);
        }
        else
        {
            // skip this one because it is not in the time range
            distances.push_back(-1.0);
        }
    }

    for (int i = 0; i < start_points.size(); ++i)
    {
        if (distances[i] >= 0.0)
        {
            Float dist = (start_points[i] - closure.its->p).norm();

            if (dist >= distances[i] - band_width &&
                dist <= distances[i] + band_width)
            {
                Float mag = 1.0 - (std::abs(dist - distances[i]) / (band_width));
                scale = color_max(scale, Color3f(mag));
                found = true;
            }
        }
    }

    if (found)
    {
        return radar_mat->material;
    }
    else
    {
        scale = Color3f(1.f);
        return mesh_mat->material;
    }
}

FEIGN_END()
