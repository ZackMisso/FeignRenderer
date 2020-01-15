/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/material.h>

RadarMaterial::RadarMaterial(BSDFNode* radar_bsdf,
                             BSDFNode* mesh_bsdf,
                             std::vector<Point3f> start_points,
                             std::vector<Float> end_dist,
                             std::vector<Float> start_times,
                             std::vector<Float> end_times,
                             float band_width,
                             float fall_off,
                             float proxy)
    : radar_bsdf(radar_bsdf),
      mesh_bsdf(mesh_bsdf),
      start_points(start_points),
      end_dist(end_dist),
      start_times(start_times),
      end_times(end_times),
      band_width(band_width),
      fall_off(fall_off),
      proxy(proxy)
{
    assert(start_points.size() == end_dist.size() &&
           start_points.size() == start_times.size() &&
           start_points.size() == end_times.size());

}

BSDF* RadarMaterial::getBSDF(const Intersection& its,
                             Color3f& mat_scale) const
{
    // LOG("getting bsdf");
    std::vector<Float> distances = std::vector<Float>();
    mat_scale = Color3f(0.f);

    bool found = false;

    for (int i = 0; i < start_times.size(); ++i)
    {
        if (end_times[i] >= proxy && start_times[i] <= proxy)
        {
            Float dist = (proxy - start_times[i]) /
                         (end_times[i] - start_times[i]) * end_dist[i];

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
            Float dist = (start_points[i] - its.p).norm();

            if (dist >= distances[i] - band_width &&
                dist <= distances[i] + band_width)
            {
                Float mag = 1.0 - (std::abs(dist - distances[i]) / (band_width));
                // LOG("mag:", mag);
                mat_scale = color_max(mat_scale, Color3f(mag));
                found = true;
            }
        }
    }

    // LOG("made it to the end");

    // TODO: this should really interpolate between two materials, but
    //       that will require the correct shader implementation of this.

    // throw new NotImplementedException("radar material get bsdf");
    if (found) return radar_bsdf->bsdf;
    mat_scale = Color3f(1.f);

    return mesh_bsdf->bsdf;
}
