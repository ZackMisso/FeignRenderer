/**
 * Author:    Zackary Misso
 * Version:   0.1.0
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

void MeshEmitter::pre_process()
{
    if (!mesh)
    {
        throw new FeignRendererException("mesh node not initialized mesh emitter");
    }

    // calculate the pdf from the mesh
    sa_pdf = new DiscretePDF1D(mesh->mesh->primitive_count());

    for (int i = 0; i < mesh->mesh->primitive_count(); ++i)
    {
        sa_pdf->cdf[i + 1] = sa_pdf->cdf[i] + mesh->mesh->surface_area(i);
    }

    sa_pdf->normalize();
}

Color3f MeshEmitter::sample_nee(EmitterQuery &rec,
                                const Point2f &sample,
                                Float *pdf) const
{
    Point2f uv_samp = sample;
    int primitive_index = sa_pdf->sample_reuse(uv_samp[0], *pdf);

    Point2f uv = WarpSpace::sqr_to_uni_tri(uv_samp);
    Float sa = mesh->mesh->surface_area(primitive_index);
    *pdf = (1.0 / sa) * (*pdf);

    Intersection its;
    its.uv = uv;
    its.f = primitive_index;
    mesh->mesh->complete_intersection_info(its);

    rec.wi = its.p - rec.p;
    rec.sqr_dist = rec.wi.sqr_norm();
    rec.wi = rec.wi.normalized();
    rec.sh_n = its.s_frame.n;

    if (-rec.wi % rec.sh_n <= EPSILON)
    {
        *pdf = 0.f;
        return Color3f(0.f);
    }

    *pdf = ((*pdf) * rec.sqr_dist) / std::abs(-rec.wi % rec.sh_n);

    return intensity;
}

Color3f MeshEmitter::sample_medium(EmitterQuery &rec,
                                   const Point2f &sample,
                                   Float *pdf) const
{
    // NOTE: this should never be called. This type of sampling is only used for
    //       point lights and spot lights
    // NOTE: zack from 4-5 years ago was dumb. implement this eventually.
    throw new NotImplementedException("mesh emitter sample pos");

    return Color3f(0.f);
}

Color3f MeshEmitter::sample_ray(EmitterQuery &rec,
                                const Point2f &dir_sample,
                                const Point2f &point_sample,
                                Float *pdf) const
{
    throw new NotImplementedException("mesh emitter sample ray");
}

Color3f MeshEmitter::evaluate(EmitterQuery &rec) const
{
    if (-rec.wi[2] < EPSILON)
        return Color3f(0.f);

    return intensity;
}

void MeshEmitter::set_mesh_node(MeshNode *node)
{
    mesh = node;
}

Point3f MeshEmitter::get_center() const
{
    return mesh->mesh->centroid();
}

FEIGN_END()
