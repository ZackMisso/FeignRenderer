#include <feign/phase/phase.h>

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
