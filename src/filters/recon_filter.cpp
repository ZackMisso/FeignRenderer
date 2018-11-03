#include <feign/filters/recon_filter.h>

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

string ReconstructionFilter::getName() const
{
    return "reconstruction filter: ";
}
