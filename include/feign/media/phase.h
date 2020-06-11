/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>

class PhaseFunction : public Node
{
public:
    PhaseFunction();
    virtual ~PhaseFunction();

    virtual std::string getName() const;

    // TODO
};

/////////////////////////////////////////////////
// PhaseFunction Node structure
/////////////////////////////////////////////////
struct PhaseFunctionNode : public Node
{
public:
    PhaseFunctionNode() : phase(nullptr) { }
    PhaseFunctionNode(std::string name) : Node(name), phase(nullptr) { }
    PhaseFunctionNode(PhaseFunction* phase) : phase(phase) { }

    ~PhaseFunctionNode() { delete phase; }

    PhaseFunction* phase;
};
/////////////////////////////////////////////////
