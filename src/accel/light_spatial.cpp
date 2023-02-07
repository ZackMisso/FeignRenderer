/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/accel_light.h>

FEIGN_BEGIN()

SpatialLightAccel::SpatialLightAccel(int width, int height, int depth)
    : LightAccel(), width(width), height(height), depth(depth)
{
    bounds = nullptr;
}

void SpatialLightAccel::clear()
{
    if (bounds)
        delete[] bounds;
}

void SpatialLightAccel::build(const BBox3f &scene_bounds,
                              const std::vector<Emitter *> &emitters)
{
    // The recipe for building this acceleration structure is to make the pdf
    // proportional to the inverse distance. While calculating the inverse
    // distance to every intersection point will be prohibitively expensive,
    // the idea is to calculate the inverse distance with respect to the center
    // of the bounding box that the intersection point falls in.
    //
    // For emitters which are not spatial, i.e. environemnt maps, the values pmfs
    // are autimatically set to the average of all emitters within each spatial
    // light bounds.

    int size = width * height * depth;

    // TODO: maybe make this configurable to light groups in the future
    light_area_bounds = scene_bounds;

    bounds = new SpatialLightBounds[size];

    for (int k = 0; k < depth; ++k)
    {
        Float min_z = scene_bounds.sample_z(Float(k) / Float(depth));
        Float max_z = scene_bounds.sample_z(Float(k + 1) / Float(depth));

        for (int i = 0; i < height; ++i)
        {
            Float min_y = scene_bounds.sample_y(Float(i) / Float(height));
            Float max_y = scene_bounds.sample_y(Float(i + 1) / Float(height));

            for (int j = 0; j < width; ++j)
            {
                Float min_x = scene_bounds.sample_x(Float(j) / Float(width));
                Float max_x = scene_bounds.sample_x(Float(j + 1) / Float(width));

                int index = ((k * height) + i) * width + j;

                // LOG("creating disc pdf");
                bounds[index].emitter_pdf = new DiscretePDF1D(emitters.size());
                // LOG("post creating disc pdf");

                bounds[index].bbox = BBox3f(Vec3f(min_x, min_y, min_z),
                                            Vec3f(max_x, max_y, max_z));

                Point3f center = bounds[index].bbox.center();

                for (int l = 0; l < emitters.size(); ++l)
                {
                    if (!emitters[l]->isSpatial())
                    {
                        throw new NotImplementedException(
                            "infinite emitters for spatial acceleration");
                    }

                    if (bounds[index].bbox.contains(emitters[l]->getCenter()))
                    {
                        bounds[index].emitter_pdf->set_pmf(l, 1.f);
                    }
                    else
                    {
                        Float norm = ((center - emitters[l]->getCenter()).sqrNorm());

                        bounds[index].emitter_pdf->set_pmf(l, std::min(1.f / norm, 0.01f));
                    }
                }

                bounds[index].emitter_pdf->convert_pdf_to_cdf();
            }
        }
    }
}

void SpatialLightAccel::sampleEmitter(Point3f pos,
                                      Sampler *sampler,
                                      int &index,
                                      Float &pdf)
{
    Point3f local = light_area_bounds.local_space(pos).min(0.f).max(0.f);

    int x = floor(local(0) * width);
    int y = floor(local(1) * height);
    int z = floor(local(2) * depth);

    index = bounds[((z * height) + y) * width + x].emitter_pdf->sample(
        sampler->next1D(),
        pdf);
}

void SpatialLightAccel::sampleEmitters(Point3f pos,
                                       Sampler *sampler,
                                       std::vector<int> &indices,
                                       std::vector<Float> &pdf)
{
    for (int i = 0; i < indices.size(); ++i)
    {
        Point3f local = light_area_bounds.local_space(pos).min(0.f).max(0.f);

        int x = floor(local(0) * width);
        int y = floor(local(1) * height);
        int z = floor(local(2) * depth);

        indices[i] = bounds[((z * height) + y) * width + x].emitter_pdf->sample(
            sampler->next1D(),
            pdf[i]);
    }
}

FEIGN_END()
