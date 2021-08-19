/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/accel_light.h>

FEIGN_BEGIN()

NaiveLightAccel::NaiveLightAccel() : LightAccel() {}

void NaiveLightAccel::clear() {}

void NaiveLightAccel::build(const BBox3f &scene_bounds,
                            const std::vector<Emitter *> &emitters)
{
    number_of_emitters = emitters.size();
    pmf = 1.f / float(emitters.size());
}

void NaiveLightAccel::sampleEmitter(Point3f pos,
                                    Sampler *sampler,
                                    int &index,
                                    Float &pdf)
{
    index = sampler->next1D() * number_of_emitters;
    pdf = pmf;
}

void NaiveLightAccel::sampleEmitters(Point3f pos,
                                     Sampler *sampler,
                                     std::vector<int> &indices,
                                     std::vector<Float> &pdf)
{
    for (int i = 0; i < indices.size(); ++i)
    {
        indices[i] = sampler->next1D() * number_of_emitters;
        pdf[i] = pmf;
    }
}

FEIGN_END()
