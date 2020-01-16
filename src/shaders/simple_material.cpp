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

MaterialClosure SimpleMaterialShader::evaluate(const Intersection& its) const
{
    MaterialClosure closure = MaterialClosure();

    (*material)()->evaluate(closure);

    return closure;
}
