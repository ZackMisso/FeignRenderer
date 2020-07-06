/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>
#include <feign/math/warp.h>

FEIGN_BEGIN()

MeshEmitter::MeshEmitter(Color3f intensity)
    : intensity(intensity)
{
    sa_pdf = nullptr;
    mesh = nullptr;
}

MeshEmitter::~MeshEmitter()
{
    delete sa_pdf;
}

void MeshEmitter::preProcess()
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

Color3f MeshEmitter::sample_li(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const
{
    Point2f uv_samp = sample;
    int primitive_index = sa_pdf->sample_reuse(uv_samp[0], *pdf);

    Point2f uv = WarpSpace::sqrToUniTri(uv_samp);
    Float sa = mesh->mesh->surface_area(primitive_index);
    *pdf = (1.0 / sa) * (*pdf);

    Intersection its;
    its.uv = uv;
    its.f = primitive_index;
    mesh->mesh->completeIntersectionInfo(its);

    rec.wi = its.p - rec.p;
    rec.sqr_dist = rec.wi.sqrNorm();
    rec.wi = rec.wi.normalized();
    rec.sh_n = its.s_frame.n;

    if (-rec.wi % rec.sh_n <= Epsilon)
    {
        *pdf = 0.f;
        return Color3f(0.f);
    }

    *pdf = ((*pdf) * rec.sqr_dist) / std::abs(-rec.wi % rec.sh_n);

    return intensity;
}

Color3f MeshEmitter::sample_pos(EmitterQuery& rec,
                                const Point2f& sample,
                                Float* pdf) const
{
    // NOTE: this should never be called. This type of sampling is only used for
    //       point lights and spot lights
    throw new NotImplementedException("mesh emitter sample pos");

    return Color3f(0.f);
}

Color3f MeshEmitter::evaluate(EmitterQuery& rec) const
{
    if (-rec.wi[2] < Epsilon) return Color3f(0.f);

    return intensity;
}

void MeshEmitter::setMeshNode(MeshNode* node)
{
    mesh = node;
}

Point3f MeshEmitter::getCenter() const
{
    return mesh->mesh->centroid();
}

FEIGN_END()
