/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/shader.h>

SimpleMaterialShader::SimpleMaterialShader(MaterialNode* material)
    : material(material) { }

void SimpleMaterialShader::sample(MaterialClosure& closure) const
{
    (*material)()->sample(closure);
}

void SimpleMaterialShader::evaluate(MaterialClosure& closure) const
{
    (*material)()->evaluate(closure);
}

void SimpleMaterialShader::evaluate_mat_only(MaterialClosure& closure) const
{
    (*material)()->evaluate_mat_only(closure);
}

// MaterialClosure SimpleMaterialShader::evaluate(const Intersection& its) const
// {
//     MaterialClosure closure = MaterialClosure();
//
//     (*material)()->evaluate(closure);
//
//     return closure;
// }
