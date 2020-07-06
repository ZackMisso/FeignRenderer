/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/media/media.h>

FEIGN_BEGIN()

struct MediumBoundry
{
    MediumBoundry(MediaNode* inside, MediaNode* outside)
        : inside(inside), outside(outside) { }

    ~MediumBoundry()
    {
        inside = nullptr;
        outside = nullptr;
    }

    MediumBoundry(const MediumBoundry& boundry)
    {
        inside = boundry.inside;
        outside = boundry.outside;
    }

    MediaNode* inside;
    MediaNode* outside;
};

FEIGN_END()
