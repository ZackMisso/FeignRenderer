#include <feign/bsdfs/diffuse.h>

Diffuse::Diffuse() : BSDF()
{
    // TODO
}

Diffuse::Diffuse(Node* parent) : BSDF(parent)
{
    // TODO
}

string Diffuse::getName() const
{
    return BSDF::getName() + "diffuse";
}
