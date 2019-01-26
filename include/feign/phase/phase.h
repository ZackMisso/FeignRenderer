#pragma once

#include <feign/node.h>

class PhaseFunction : public Node
{
public:
    PhaseFunction();
    virtual ~PhaseFunction();

    virtual std::string getName() const;

    // TODO
};
