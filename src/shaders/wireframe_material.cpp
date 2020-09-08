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

WireframeMaterialShader::WireframeMaterialShader(MaterialNode* wireframe_mat,
                                                 MaterialNode* mesh_mat,
                                                 float threshold)
    : wireframe_mat(wireframe_mat),
      mesh_mat(mesh_mat),
      threshold(threshold) { }

void WireframeMaterialShader::sample(MaterialClosure& closure) const
{
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    Material* material = choose_mat(closure);
    material->sample(closure);

    #if CLOCKING
        Clocker::endClock("shader eval");
    #endif
}

// TODO: why is this called evaluate if it does not actually evaluate the material...
void WireframeMaterialShader::evaluate(MaterialClosure& closure) const
{
    // assert(false);
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    Material* material = choose_mat(closure);

    closure.is_specular = material->isDelta();

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

void WireframeMaterialShader::evaluate_mat_only(MaterialClosure& closure) const
{
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    Material* material = choose_mat(closure);
    material->evaluate(closure);

    #if CLOCKING
        Clocker::endClock("shader eval");
    #endif
}

Material* WireframeMaterialShader::choose_mat(MaterialClosure& closure) const
{
    Point3f bary = closure.its->bary;
    float min = bary[0];
    if (bary[1] < min) min = bary[1];
    if (bary[2] < min) min = bary[2];

    if (min < threshold)
        return wireframe_mat->material;

    return mesh_mat->material;
}

void WireframeMaterialShader::evaluate_for_photon(MaterialClosure& closure) const
{
    // TODO
    throw new NotImplementedException("wireframe material shader evaluate photon");
}

FEIGN_END()
