#pragma once

#include <feign/samplers/sampler.h>

class Independent : public Sampler
{
public:
    Independent();
    Independent(SceneNode* parent);

    // TODO    
};
