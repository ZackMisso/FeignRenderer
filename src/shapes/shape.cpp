/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

#include <feign/core/shape.h>

Shape::Shape() : geomShader(nullptr)
{
    instID = 0;
    transform = Transform();
}
