/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/closure.h>
#include <feign/core/scene.h>

MaterialClosure::MaterialClosure()
{
    throw new FeignRendererException("empty constructor should not be used");
}

MaterialClosure::MaterialClosure(Sampler* sampler,
                                 Intersection* its,
                                 Ray3f* ray,
                                 const Scene* scene,
                                 bool sample_all_emitters,
                                 bool last_bounce_specular)
    : sampler(sampler),
      its(its),
      ray(ray),
      scene(scene),
      shadow_rays(std::vector<EmitterEval>()),
      albedo(Color3f(0.f)),
      nee(Color3f(0.f)),
      emission(Color3f(0.f)),
      wo(Vector3f(1.f)),
      pdf(1.f),
      eta(1.f),
      is_specular(false),
      last_spec(last_bounce_specular),
      sample_all_emitters(sample_all_emitters)
{
    wi = its->toLocal(-ray->dir);
}

MaterialClosure::MaterialClosure(Sampler* sampler,
                                 const Scene* scene,
                                 bool sample_all_emitters,
                                 bool last_bounce_specular)
    : sampler(sampler),
      scene(scene),
      shadow_rays(std::vector<EmitterEval>()),
      albedo(Color3f(0.f)),
      nee(Color3f(0.f)),
      emission(Color3f(0.f)),
      wo(Vector3f(1.f)),
      pdf(1.f),
      eta(1.f),
      is_specular(false),
      last_spec(last_bounce_specular),
      sample_all_emitters(sample_all_emitters)
{
}

void MaterialClosure::accumulate_shadow_rays(const MaterialShader* shader)
{
    Vector3f tmp_wo = wo;
    Color3f tmp_albedo = albedo;

    // LOG("shadow rays size:", int(shadow_rays.size()));

    for (int i = 0; i < shadow_rays.size(); ++i)
    {
        assert(shadow_rays.size() == 1);
        if (shadow_rays[i].valid)
        {
            // assert(false);
            wo = shadow_rays[i].shadow_ray.normalized();
            // evaluate the albedo for this shadow connection
            // LOG("albedo_before:", albedo);
            // LOG("throughput_before:", shadow_rays[i].throughput);
            shader->evaluate_mat_only(*this);
            // LOG("albedo:", albedo);
            // LOG("through:", shadow_rays[i].throughput);
            // LOG("albedo", albedo);
            shadow_rays[i].throughput *= albedo;
            // LOG("throughput_after:", shadow_rays[i].throughput);
            // LOG("albedo_after:", albedo);

            nee += shadow_rays[i].throughput;
            // nee = albedo;
            // nee = Color3f(0.5);
        }
    }

    shadow_rays.clear();

    wo = tmp_wo;
    albedo = tmp_albedo;
}
