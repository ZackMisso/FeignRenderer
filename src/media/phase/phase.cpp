/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/phase/phase.h>

PhaseFunction::PhaseFunction()
{
    // TODO
}

PhaseFunction::~PhaseFunction()
{
    std::cout << "deleting phase" << std::endl;
    // TODO
}

std::string PhaseFunction::getName() const
{
    return "phase: ";
}
