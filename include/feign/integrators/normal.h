#pragma once

#include <feign/integrators/integrator.h>

class NormalIntegrator : public Integrator
{
public:
    NormalIntegrator();
    NormalIntegrator(SceneNode* parent);

    virtual void tempMethod();
};
