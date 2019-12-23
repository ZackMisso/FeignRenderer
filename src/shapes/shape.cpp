/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

#include <feign/shapes/shape.h>

Shape::Shape()
{
    material = new Material();
    transform = Transform();
}

Shape::~Shape()
{
    std::cout << "deleting shape" << std::endl;
    delete material;
}
