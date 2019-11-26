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
// Emitter Query Object
/////////////////////////////////////////////////
struct EmitterQuery
{
    Vector3f p;
    Vector3f wi;
    Vector3f sh_n;
    Vector3f g_n;
    Point2f uv;
    Float sqr_dist;
    Float pdf;
    int prim_index;
    int em_index;

    EmitterQuery(const Vector3f& p) : p(p) { }
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Emitter
/////////////////////////////////////////////////
class Emitter : public Node
{
public:
    Emitter();
    virtual ~Emitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const = 0;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const = 0;

    virtual std::string getName() const;
    virtual NodeType getNodeType() const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Distant Emitter
/////////////////////////////////////////////////
class DistantEmitter : public Emitter
{
public:
    DistantEmitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual void preProcess();

    virtual std::string getName() const;

protected:
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Directional Emitter
/////////////////////////////////////////////////
class DirectionalEmitter : public Emitter
{
public:
    DirectionalEmitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual void preProcess();

    virtual std::string getName() const;

protected:
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Point Emitter
/////////////////////////////////////////////////
class PointEmitter : public Emitter
{
public:
    PointEmitter();
    PointEmitter(Color3f I,
                 Point3f pos);

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual void preProcess();

    virtual std::string getName() const;

protected:
    Color3f I;
    Point3f pos;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Environment Emitter
/////////////////////////////////////////////////
class EnvironmentEmitter : public Emitter
{
public:
    EnvironmentEmitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual void preProcess();

    virtual std::string getName() const;

protected:
};
/////////////////////////////////////////////////
