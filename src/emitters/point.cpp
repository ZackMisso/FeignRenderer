#include <feign/emitters/point.h>

PointEmitter::PointEmitter() : Emitter() { }

PointEmitter::PointEmitter(Node* parent) : Emitter(parent) { }

PointEmitter::PointEmitter(Node* parent,
                           Color3f I,
                           Point3f pos)
    : Emitter(parent), I(I), pos(pos)
{
    // does nothing
}

void PointEmitter::preProcess() { preProcessChildren(); }

Color3f PointEmitter::sample_li(EmitterQuery& rec,
                                const Point2f& sample,
                                Float* pdf) const
{
    rec.wi = (pos - rec.p).normalized();

    if (pdf) *pdf = 1.0;

    return I / (pos - rec.p).sqrNorm();
}

Color3f PointEmitter::sample_pos(EmitterQuery& rec,
                                 const Point2f& sample,
                                 Float* pdf) const
{
    throw new NotImplementedException("point sample pos");

    return Color3f(0.f);
}

std::string PointEmitter::getName() const
{
    return Emitter::getName() + "point";
}
