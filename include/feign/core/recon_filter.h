/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>

/////////////////////////////////////////////////
// Reconstruction Filter
/////////////////////////////////////////////////
class ReconstructionFilter : public Node
{
public:
    ReconstructionFilter();
    ReconstructionFilter(Vec2f size);

    virtual Float evaluate(const Point2f& p) const = 0;

    virtual NodeType getNodeType() const;

    virtual std::string getName() const;

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

    virtual Float evaluate(const Point2f& p) const;

    virtual std::string getName() const;
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

    virtual Float evaluate(const Point2f& p) const;

    virtual std::string getName() const;

protected:
    Vec2f expCoeff;
    Float alpha;
    Float std_dev;
};
/////////////////////////////////////////////////
