/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

MeshEmitter::MeshEmitter(Color3f intensity)
    : intensity(intensity)
{
    // does nothing
}

void MeshEmitter::preProcess()
{
    throw new NotImplementedException("mesh emitter preprocess");
}

Color3f MeshEmitter::sample_li(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const
{
    throw new NotImplementedException("mesh emitter sample li");

    return Color3f(0.0);
}

Color3f MeshEmitter::sample_pos(EmitterQuery& rec,
                                const Point2f& sample,
                                Float* pdf) const
{
    throw new NotImplementedException("mesh emitter sample pos");

    return Color3f(0.f);
}

Color3f MeshEmitter::evaluate(EmitterQuery& rec) const
{
    throw new NotImplementedException("mesh emitter evaluate");

    return Color3f(0.f);
}

void MeshEmitter::setMeshNode(MeshNode* node)
{
    mesh = node;
}
