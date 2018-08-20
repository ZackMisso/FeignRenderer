#include <feign/samplers/independent.h>

Independent::Independent() : Sampler()
{
    // TODO
}

Independent::Independent(Node* parent) : Sampler(parent)
{
    // TODO
}

string Independent::getName() const
{
    return Sampler::getName() + "independent";
}
