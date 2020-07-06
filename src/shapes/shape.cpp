/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/shape.h>

FEIGN_BEGIN()

Shape::Shape(const MediumBoundry* boundry,
             bool is_null)
    : geomShader(nullptr),
      is_null(is_null),
      boundry(boundry)
{
    instID = 0;
    transform = Transform();
}

FEIGN_END()
