/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/shader.h>
#include <feign/core/scene.h>

WireframeMaterialShader::WireframeMaterialShader(MaterialNode* wireframe_mat,
                                                 MaterialNode* mesh_mat,
                                                 float threshold)
    : wireframe_mat(wireframe_mat),
      mesh_mat(mesh_mat),
      threshold(threshold) { }

void WireframeMaterialShader::sample(MaterialClosure& closure) const
{
    Material* material = choose_mat(closure);
    material->sample(closure);
}

void WireframeMaterialShader::evaluate(MaterialClosure& closure) const
{
    Material* material = choose_mat(closure);

    closure.is_specular = material->isDelta();

    if (closure.is_specular)
    {
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
}

void WireframeMaterialShader::evaluate_mat_only(MaterialClosure& closure) const
{
    Material* material = choose_mat(closure);
    material->evaluate(closure);
}

Material* WireframeMaterialShader::choose_mat(MaterialClosure& closure) const
{
    Point3f bary = closure.its->bary;
    float min = bary[0];
    if (bary[1] < min) min = bary[1];
    if (bary[2] < min) min = bary[2];

    if (min < threshold)
        return wireframe_mat->material;

    return mesh_mat->material;
}
