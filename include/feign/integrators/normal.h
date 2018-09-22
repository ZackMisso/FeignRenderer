#pragma once

#include <feign/integrators/integrator.h>

class NormalIntegrator : public Integrator
{
public:
    NormalIntegrator();
    NormalIntegrator(Node* parent);

    virtual void preProcess();

    virtual string getName() const;

    virtual void tempMethod();
};
