#include <feign/emitters/point.h>

PointEmitter::PointEmitter() : Emitter()
{
    throw new NotImplementedException("point constructor");
}

PointEmitter::PointEmitter(Node* parent) : Emitter(parent)
{
    throw new NotImplementedException("point constructor");
}

std::string PointEmitter::getName() const
{
    return Emitter::getName() + "point";
}
