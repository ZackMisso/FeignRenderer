/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/shader.h>
#include <feign/core/scene.h>
#include <feign/stats/clocker.h>

SimpleMaterialShader::SimpleMaterialShader(MaterialNode* material)
    : material(material) { }

void SimpleMaterialShader::sample(MaterialClosure& closure) const
{
    (*material)()->sample(closure);
}

void SimpleMaterialShader::evaluate(MaterialClosure& closure) const
{
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    closure.is_specular = (*material)()->isDelta();

    // LOG("huh");
    if (closure.last_spec)
    {
        closure.scene->accumulate_emission(closure);
    }

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
        // get next event estimation
        closure.scene->eval_one_emitter(closure);
    }

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

// MaterialClosure SimpleMaterialShader::evaluate(const Intersection& its) const
// {
//     MaterialClosure closure = MaterialClosure();
//
//     (*material)()->evaluate(closure);
//
//     return closure;
// }
