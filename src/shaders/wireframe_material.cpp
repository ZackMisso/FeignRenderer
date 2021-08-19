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

WireframeMaterialShader::WireframeMaterialShader(BSDFNode *wireframe_bsdf,
                                                 BSDFNode *mesh_bsdf,
                                                 float threshold)
    : wireframe_bsdf(wireframe_bsdf),
      mesh_bsdf(mesh_bsdf),
      threshold(threshold) {}

void WireframeMaterialShader::sample(MaterialClosure &closure) const
{
#if CLOCKING
    Clocker::startClock("shader eval");
#endif

    BSDF *bsdf = choose_bsdf(closure);
    bsdf->sample(closure);

#if CLOCKING
    Clocker::endClock("shader eval");
#endif
}

// TODO: why is this called evaluate if it does not actually evaluate the material...
void WireframeMaterialShader::evaluate(MaterialClosure &closure) const
{
#if CLOCKING
    Clocker::startClock("shader eval");
#endif

    BSDF *bsdf = choose_bsdf(closure);

    closure.is_specular = bsdf->isDelta();

    if (closure.last_spec)
    {
        closure.scene->accumulate_emission(closure);
    }

    if (closure.is_specular)
    {
        closure.last_spec = closure.is_specular;

        return;
    }

    if (closure.sample_all_emitters)
    {
        if (closure.material_accepts_shadows)
        {
            closure.scene->eval_all_emitters(closure);
        }
    }
    else
    {
        if (closure.material_accepts_shadows)
        {
            closure.scene->eval_one_emitter(closure);
        }
    }

#if CLOCKING
    Clocker::endClock("shader eval");
#endif
}

void WireframeMaterialShader::evaluate_mat_only(MaterialClosure &closure) const
{
#if CLOCKING
    Clocker::startClock("shader eval");
#endif

    BSDF *bsdf = choose_bsdf(closure);
    bsdf->evaluate(closure);

#if CLOCKING
    Clocker::endClock("shader eval");
#endif
}

BSDF *WireframeMaterialShader::choose_bsdf(MaterialClosure &closure) const
{
    Point3f bary = closure.its->bary;
    float min = bary[0];
    if (bary[1] < min)
        min = bary[1];
    if (bary[2] < min)
        min = bary[2];

    if (min < threshold)
        return wireframe_bsdf->bsdf;

    return mesh_bsdf->bsdf;
}

void WireframeMaterialShader::evaluate_for_photon(MaterialClosure &closure) const
{
    throw new NotImplementedException("wireframe material shader evaluate photon");
}

FEIGN_END()
