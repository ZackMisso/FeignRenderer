#include <feign/materials/material.h>

Material::Material()
{
    bsdf = nullptr;
}

Material::~Material()
{
    if (bsdf) delete bsdf;
}
