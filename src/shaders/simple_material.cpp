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

SimpleMaterialShader::SimpleMaterialShader(MaterialNode* material)
    : material(material) { }

void SimpleMaterialShader::sample(MaterialClosure& closure) const
{
    (*material)()->sample(closure);
}

// TODO: why is this called evaluate if it does not actually evaluate the material...
void SimpleMaterialShader::evaluate(MaterialClosure& closure) const
{
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    closure.is_specular = (*material)()->isDelta();

    if (closure.last_spec)
    {
        closure.scene->accumulate_emission(closure);
    }

    if (closure.is_specular)
    {
        #if CLOCKING
            Clocker::endClock("shader eval");
        #endif

        closure.last_spec = closure.is_specular;

        return;
    }

    if (closure.sample_all_emitters)
    {
        // if ((*material)->accepts_shadows)
        // TODO: make this specified by the user
        if (closure.material_accepts_shadows)
        {
            // assert(false);
            closure.scene->eval_all_emitters(closure);
        }
    }
    else
    {
        // if ((*material)->accepts_shadows)
        // TODO: fix this hack with above
        if (closure.material_accepts_shadows)
        {
            // assert(false);
            closure.scene->eval_one_emitter(closure);
        }
    }

    closure.last_spec = closure.is_specular;
    closure.material_accepts_shadows = true;

    #if CLOCKING
        Clocker::endClock("shader eval");
    #endif
}

void SimpleMaterialShader::evaluate_mat_only(MaterialClosure& closure) const
{
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    (*material)()->evaluate(closure);

    #if CLOCKING
        Clocker::endClock("shader eval");
    #endif
}

FEIGN_END()
