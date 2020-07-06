/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/closure.h>
#include <feign/core/scene.h>

FEIGN_BEGIN()

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
      is_specular(last_bounce_specular),
      last_spec(last_bounce_specular),
      sample_all_emitters(sample_all_emitters),
      material_accepts_shadows(true)
{
    wi = its->toLocal(-ray->dir);
    media = nullptr;
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
      is_specular(last_bounce_specular),
      last_spec(last_bounce_specular),
      sample_all_emitters(sample_all_emitters),
      material_accepts_shadows(true)
{
    media = nullptr;
}

void MaterialClosure::accumulate_shadow_rays(const MaterialShader* shader)
{
    Vector3f tmp_wo = wo;
    Color3f tmp_albedo = albedo;

    for (int i = 0; i < shadow_rays.size(); ++i)
    {
        if (shadow_rays[i].valid)
        {
            wo = shadow_rays[i].shadow_ray.normalized();
            shader->evaluate_mat_only(*this);
            shadow_rays[i].throughput *= albedo;
            nee += shadow_rays[i].throughput;
        }
    }

    shadow_rays.clear();

    wo = tmp_wo;
    albedo = tmp_albedo;
}

FEIGN_END()
