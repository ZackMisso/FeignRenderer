/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/material.h>
#include <feign/core/scene.h>

SimpleMaterial::SimpleMaterial(BSDFNode* bsdf)
    : bsdf(bsdf) { }

SimpleMaterial::~SimpleMaterial() { }

// BSDF* SimpleMaterial::getBSDF(const Intersection& its,
//                               Color3f& mat_scale) const
// {
//     mat_scale = Color3f(1.f);
//     return bsdf->bsdf;
// }

void SimpleMaterial::sample(MaterialClosure& closure) const
{
    (*bsdf)()->sample(closure);
}

void SimpleMaterial::evaluate_mat_only(MaterialClosure& closure) const
{
    (*bsdf)()->evaluate(closure);
}

void SimpleMaterial::evaluate(MaterialClosure& closure) const
{
    closure.is_specular = (*bsdf)()->isDelta();

    if (closure.is_specular)
    {
        // no light contribution for specular surfaces
        return;
    }

    if (closure.sample_all_emitters)
    {
        closure.scene->eval_all_emitters(closure);
    }
    else
    {
        closure.scene->eval_one_emitter(closure);
    }

    // // create the bsdf query
    // BSDFQuery bqr(closure.its->toLocal(-closure.ray->dir),
    //               closure.its->toLocal(closure.wo),
    //               closure.its->uv,
    //               closure.its->p);
    //
    // Color3f bsdf_val = (*bsdf)()->eval(bqr);
    //
    // float cos_term = closure.its->s_frame.n % closure.wo;
    // if (cos_term < -Epsilon) cos_term = -cos_term;
    //
    // closure.albedo = bsdf_val * cos_term;
}
