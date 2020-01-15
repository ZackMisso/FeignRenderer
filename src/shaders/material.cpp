/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/shader.h>

MaterialClosure::MaterialClosure() { }

MaterialShader::~MaterialShader() { }

MaterialClosure MaterialShader::evaluate(const Intersection& its) const
{
    return MaterialClosure();
}
