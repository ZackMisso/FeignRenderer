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
