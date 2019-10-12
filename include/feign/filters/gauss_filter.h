/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/filters/recon_filter.h>

class GaussFilter : public ReconstructionFilter
{
public:
    GaussFilter();
    GaussFilter(Vec2f size);
    GaussFilter(Vec2f size, Float std_dev);

    virtual void preProcess();

    virtual Float evaluate(const Point2f& p) const;

    virtual std::string getName() const;

protected:
    Vec2f expCoeff;
    Float alpha;
    Float std_dev;
};
