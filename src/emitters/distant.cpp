#include <feign/emitters/distant.h>

DistantEmitter::DistantEmitter()
{
    throw new NotImplementedException("emitter distant");
}

DistantEmitter::DistantEmitter(Node* parent)
{
    throw new NotImplementedException("emitter distant");
}

Color3f DistantEmitter::sample_li(EmitterQuery& rec,
                                  const Point2f& sample,
                                  Float* pdf) const
{
    throw new NotImplementedException("emitter distant");
}

Color3f DistantEmitter::sample_pos(EmitterQuery& rec,
                                   const Point2f& sample,
                                   Float* pdf) const
{
    throw new NotImplementedException("emitter distant");
}

void DistantEmitter::preProcess()
{
    throw new NotImplementedException("emitter distant");
}

std::string DistantEmitter::getName() const
{
    throw new NotImplementedException("emitter distant");
}
