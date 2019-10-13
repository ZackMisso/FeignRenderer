/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/recon_filter.h>

ReconstructionFilter::ReconstructionFilter()
{
    // does nothing
}

ReconstructionFilter::ReconstructionFilter(Vec2f size) : size(size)
{
    // does nothing
}

NodeType ReconstructionFilter::getNodeType() const
{
    return NT_Recon_Filter;
}

std::string ReconstructionFilter::getName() const
{
    return "reconstruction filter: ";
}
