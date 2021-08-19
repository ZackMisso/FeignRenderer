/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

FEIGN_BEGIN()

DirectionalMeshEmitter::DirectionalMeshEmitter(Vector3f light_dir,
                                               Color3f intensity)
    : light_dir(light_dir), intensity(intensity)
{
    light_dir = light_dir.normalized();
    sa_pdf = nullptr;
    mesh = nullptr;
}

DirectionalMeshEmitter::~DirectionalMeshEmitter()
{
    delete sa_pdf;
}

void DirectionalMeshEmitter::preProcess()
{
    if (!mesh)
    {
        throw new FeignRendererException("mesh node not initialized mesh emitter");
    }

    // calculate the pdf from the mesh
    sa_pdf = new DiscretePDF1D(mesh->mesh->primitiveCount());

    for (int i = 0; i < mesh->mesh->primitiveCount(); ++i)
    {
        sa_pdf->cdf[i + 1] = sa_pdf->cdf[i] + mesh->mesh->surface_area(i);
    }

    sa_pdf->normalize();
}

Color3f DirectionalMeshEmitter::sample_nee(EmitterQuery &rec,
                                           const Point2f &sample,
                                           Float *pdf) const
{
    // actual visibility is checked as apart of the actual integrator, so sampling
    // this emitter is functionally the same as sampling a directional emitter.
    // however, the post process visibility check will first check if the directional
    // emitter can be hit from the specific direction from the eye vertex, and
    // if so will set the correct sqr_dist. Otherwise it will set the intensity to
    // zero.

    rec.wi = -light_dir;
    // LOG("dir: ");
    // LOG(light_dir);
    if (pdf)
        *pdf = 1.0;

    return intensity;
}

Color3f DirectionalMeshEmitter::sample_medium(EmitterQuery &rec,
                                              const Point2f &sample,
                                              Float *pdf) const
{
    throw new NotImplementedException("emitter directional");
}

Color3f DirectionalMeshEmitter::sample_ray(EmitterQuery &rec,
                                           const Point2f &dir_sample,
                                           const Point2f &point_sample,
                                           Float *pdf) const
{
    throw new NotImplementedException("emitter directional");
}

Color3f DirectionalMeshEmitter::evaluate(EmitterQuery &rec) const
{
    // if wi is roughly == light dir return the radiance
    if (-rec.wi[2] < Epsilon)
        return Color3f(0.f);
    return intensity;

    if ((light_dir - rec.wi).sqrNorm() < Epsilon)
    {
        return intensity;
    }

    return COLOR_BLACK;
}

void DirectionalMeshEmitter::setMeshNode(MeshNode *node)
{
    mesh = node;
}

Point3f DirectionalMeshEmitter::getCenter() const
{
    return mesh->mesh->centroid();
}

FEIGN_END()
