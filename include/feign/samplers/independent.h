#pragma once

#include <feign/samplers/sampler.h>

class Independent : public Sampler
{
public:
    Independent();
    Independent(Node* parent);

    virtual string getName() const;

    // TODO
};
