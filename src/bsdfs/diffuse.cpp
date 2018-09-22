#include <feign/bsdfs/diffuse.h>

Diffuse::Diffuse() : BSDF()
{
    // TODO
}

Diffuse::Diffuse(Node* parent) : BSDF(parent)
{
    // TODO
}

void Diffuse::preProcess()
{
    throw new NotImplementedException("preprocess diffuse");

    preProcessChildren();
}

string Diffuse::getName() const
{
    return BSDF::getName() + "diffuse";
}
