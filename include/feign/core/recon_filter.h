/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()

/////////////////////////////////////////////////
// Reconstruction Filter
/////////////////////////////////////////////////
class ReconstructionFilter
{
public:
    ReconstructionFilter() {}
    ReconstructionFilter(Vec2f size) : size(size) {}
    virtual ~ReconstructionFilter() {}

    virtual void preProcess() {}
    virtual Float evaluate(const Point2f &p) const = 0;

    Vec2f getSize() const { return size; }

protected:
    Vec2f size;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Box Filter
/////////////////////////////////////////////////
class BoxFilter : public ReconstructionFilter
{
public:
    BoxFilter();
    BoxFilter(Vec2f size);

    virtual void preProcess();

    virtual Float evaluate(const Point2f &p) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Gaussian Filter
/////////////////////////////////////////////////
class GaussFilter : public ReconstructionFilter
{
public:
    GaussFilter();
    GaussFilter(Vec2f size);
    GaussFilter(Vec2f size, Float std_dev);

    virtual void preProcess();

    virtual Float evaluate(const Point2f &p) const;

protected:
    Vec2f expCoeff;
    Float alpha;
    Float std_dev;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Filter Node structure
/////////////////////////////////////////////////
struct FilterNode : public Node
{
public:
    FilterNode() : filter(nullptr) {}
    FilterNode(std::string name) : Node(name), filter(nullptr) {}
    FilterNode(ReconstructionFilter *filter) : filter(filter) {}

    ~FilterNode() { delete filter; }

    ReconstructionFilter *filter;
};
/////////////////////////////////////////////////

FEIGN_END()
