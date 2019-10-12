/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/materials/material.h>

Material::Material()
{
    bsdf = nullptr;
}

Material::~Material()
{
    std::cout << "deleting material" << std::endl;
    // if (bsdf) delete bsdf;
}
