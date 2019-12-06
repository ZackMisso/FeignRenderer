/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

// core includes
#include <feign/core/api.h>
#include <feign/core/bsdf.h>
#include <feign/core/camera.h>
#include <feign/core/emitter.h>
#include <feign/core/recon_filter.h>
#include <feign/core/integrator.h>
#include <feign/core/sampler.h>
#include <feign/core/scene.h>
#include <feign/core/material.h>

// media includes
#include <feign/media/media.h>
#include <feign/media/phase/phase.h>

// shapes
#include <feign/shapes/objmesh.h>

// textures
#include <feign/textures/texture.h>

std::vector<Transform> FeignRenderer::transform_stack = std::vector<Transform>();
Transform FeignRenderer::current_transform = Transform();
SceneNode* FeignRenderer::scene = nullptr;

std::unordered_map<std::string, BSDFNode*>                 FeignRenderer::bsdfs = std::unordered_map<std::string, BSDFNode*>();
std::unordered_map<std::string, CameraNode*>               FeignRenderer::cameras = std::unordered_map<std::string, CameraNode*>();
std::unordered_map<std::string, EmitterNode*>              FeignRenderer::emitters = std::unordered_map<std::string, EmitterNode*>();
std::unordered_map<std::string, MediaNode*>                FeignRenderer::medias = std::unordered_map<std::string, MediaNode*>();
std::unordered_map<std::string, IntegratorNode*>           FeignRenderer::integrators = std::unordered_map<std::string, IntegratorNode*>();
std::unordered_map<std::string, SamplerNode*>              FeignRenderer::samplers = std::unordered_map<std::string, SamplerNode*>();
std::unordered_map<std::string, FilterNode*>               FeignRenderer::filters = std::unordered_map<std::string, FilterNode*>();
std::unordered_map<std::string, MaterialNode*>             FeignRenderer::materials = std::unordered_map<std::string, MaterialNode*>();
std::unordered_map<std::string, ObjectNode*>               FeignRenderer::meshes = std::unordered_map<std::string, ObjectNode*>();

void FeignRenderer::fr_scene(std::string name,
                             std::string integrator_node,
                             std::string sampler_node,
                             std::string camera_node,
                             std::string medium_node)
{

    if (scene)
    {
        throw new FeignRendererException("only one scene may be constructed");
    }

    scene = new Scene();

    throw new NotImplementedException("fr_scene");
}

void FeignRenderer::fr_integrator(std::string name,
                                  std::string type,
                                  long max_time,
                                  long max_heuristic)
{
    throw new NotImplementedException("fr_integrator");
}

void FeignRenderer::fr_sampler(std::string name,
                               std::string type,
                               int spp,
                               long seed,
                               long seed2)
{
    throw new NotImplementedException("fr_sampler");
}

void FeignRenderer::fr_camera(std::string name,
                              std::string type,
                              Vector3f origin,
                              Vector3f target,
                              Vector3f up,
                              float fov,
                              Vec2i image_res)
{
    throw new NotImplementedException("fr_camera");
}

void FeignRenderer::fr_object(std::string name,
                              std::string mesh,
                              std::string material)
{
    throw new NotImplementedException("fr_object");
}

void FeignRenderer::fr_mesh(std::string name,
                            std::string type,
                            std::string filename)
{
    throw new NotImplementedException("fr_mesh");
}

void FeignRenderer::fr_emitter(std::string name,
                               std::string type,
                               std::string mesh,
                               std::string material,
                               Vector3f pos,
                               Color3f intensity)
{
    throw new NotImplementedException("fr_emitter");
}

void FeignRenderer::fr_material(std::string name,
                                std::string bsdf)
{
    throw new NotImplementedException("fr_material");
}

void FeignRenderer::fr_bsdf(std::string name,
                            std::string type,
                            Color3f albedo)
{
    if (type == "diffuse")
    {
        throw new NotImplementedException("fr_bsdf");
        // Diffuse*
        // TODO
    }
    else
    {
        // unrecognized bsdf
        std::cout << "ERROR: BSDF : " << type << " : does not exist" << std::endl;
        assert(false);
    }
}

void FeignRenderer::fr_clear_transform()
{
    throw new NotImplementedException("fr_clear_transform");
    // TODO
}

void FeignRenderer::fr_scale(float sx, float sy, float sz)
{
    throw new NotImplementedException("fr_scale");
    // TODO
}

void FeignRenderer::fr_translate(float tx, float ty, float tz)
{
    throw new NotImplementedException("fr_translate");
    // TODO
}

void FeignRenderer::fr_rotate(float angle, float x, float y, float z)
{
    throw new NotImplementedException("fr_rotate");
    // TODO
}

// this is the big one
void FeignRenderer::flush_renders()
{
    throw new NotImplementedException("flush_renders");
    // TODO
}

void FeignRenderer::clean_up()
{
    throw new NotImplementedException("clean_up");
    // TODO
}


// TODO: rewrite all of this

// void FeignRenderer::begin_world()
// {
//     scene = nullptr;
//     current_transform = Transform();
//     nodes = std::vector<Node*>();
//     transform_stack.clear();
// }
//
// void FeignRenderer::begin_node()
// {
//     // what is this?
//     throw new NotImplementedException("api begin node");
// }
//
// void FeignRenderer::begin_obj(const std::string& filename,
//                               bool flip_norms)
// {
//     transform_stack.push_back(current_transform);
//
//     ObjMesh* mesh = new ObjMesh(current_node, filename);
//     current_node->addChild(mesh);
//     current_node = mesh;
//
//     mesh->parseFromFile(filename,
//                         current_transform,
//                         flip_norms);
// }
//
// void FeignRenderer::end_world()
// {
//     assert(scene);
//
//     scene->preProcess();
//
//     // TODO: render logic here
//
//     Scene* tmp = scene;
//
//     current_node = nullptr;
//     scene = nullptr;
//
//     // return tmp;
// }
//
// void FeignRenderer::end_node()
// {
//     throw new NotImplementedException("api end node");
// }
//
// void FeignRenderer::end_obj()
// {
//     current_transform = transform_stack[transform_stack.size() - 1];
//     transform_stack.pop_back();
//     current_node = current_node->getParent();
// }
//
// void FeignRenderer::bsdf_diffuse(Color3f albedo)
// {
//     assert(current_node->getNodeType() == NT_Mesh);
//
//     Shape* shape = (Shape*)current_node;
//
//     // TODO: get materials designed and working
//     if (!shape->getMaterial())
//     {
//         shape->setMaterial(new Material());
//     }
//
//     if (shape->getMaterial()->getBSDF())
//     {
//         delete shape->getMaterial()->getBSDF();
//     }
//
//     shape->getMaterial()->setBSDF(new Diffuse(current_node, albedo));
//
//     current_node->addChild(shape->getMaterial()->getBSDF());
// }
//
// void FeignRenderer::camera_ortho(Vec3f ori,
//                                  Vec3f tar,
//                                  Vec3f up)
// {
//     throw new NotImplementedException("api camera_ortho");
// }
//
// void FeignRenderer::camera_perspective(Vec3f ori,
//                                        Vec3f tar,
//                                        Vec3f up,
//                                        Float aperatureRadius,
//                                        Float focalDistance,
//                                        Float fov,
//                                        Float near,
//                                        Float far,
//                                        uint32_t width,
//                                        uint32_t height)
// {
//     Perspective* perspective = new Perspective(current_node,
//                                                aperatureRadius,
//                                                focalDistance,
//                                                fov,
//                                                near,
//                                                far,
//                                                width,
//                                                height);
//
//     transform_stack.push_back(current_transform);
//     transform_lookat(ori, tar, up);
//
//     perspective->setCameraToWorld(current_transform);
//
//     current_transform = transform_stack[transform_stack.size()-1];
//     transform_stack.pop_back();
//
//     if (scene->camera)
//     {
//         delete scene->camera;
//     }
//
//     scene->camera = perspective;
//     scene->addChild(perspective);
// }
//
// void FeignRenderer::integrator(std::string integrator_type)
// {
//     assert(scene);
//
//     Integrator* integrator;
//
//     if (integrator_type == "amb_occ")           integrator = new Ambient_Occlusion_Integrator(scene);
//     else if (integrator_type == "light_unidir") integrator = new Light_Unidirectional_Integrator(scene);
//     else if (integrator_type == "normal")       integrator = new NormalIntegrator(scene);
//     else if (integrator_type == "path_bidir")   integrator = new Path_Bidirectional_Integrator(scene);
//     else if (integrator_type == "path_unidir")  integrator = new Path_Unidirectional_Integrator(scene);
//     else if (integrator_type == "whitted")      integrator = new WhittedIntegrator(scene);
//     else assert(false);
//
//     if (scene->integrator) delete scene->integrator;
//
//     scene->integrator = integrator;
//     scene->addChild(integrator);
// }
//
// void FeignRenderer::sampler(std::string sampler_type,
//                             uint32_t sample_cnt,
//                             uint32_t seed)
// {
//     assert(scene);
//
//     Sampler* sampler;
//
//     if (sampler_type == "independent")
//     {
//         sampler = new Independent(scene,
//                                   seed,
//                                   sample_cnt);
//     }
//     else if (sampler_type == "halton")
//     {
//         sampler = new Halton(scene,
//                              seed,
//                              sample_cnt);
//     }
//     else if (sampler_type == "latin")
//     {
//         sampler = new Latin(scene,
//                             seed,
//                             sample_cnt);
//     }
//     else assert(false);
//
//     if (scene->sampler) delete scene->sampler;
//     scene->addChild(sampler);
// }
//
// void FeignRenderer::emitter_point(Color3f I, Point3f pos)
// {
//     Point3f actual_pos = current_transform * pos;
//
//     PointEmitter* emitter = new PointEmitter(current_node, I, pos);
//
//     scene->addEmitter(emitter);
//     current_node->addChild(emitter);
// }
//
// void FeignRenderer::transform_scale(Vec3f scale)
// {
//     Matrix4f matrix = Matrix4f::scale(scale);
//     Transform scale_transform = Transform(matrix);
//     current_transform *= scale_transform;
// }
//
// void FeignRenderer::transform_rot(Float angle, Vec3f axis)
// {
//     throw new NotImplementedException("api transform_rot");
// }
//
// void FeignRenderer::transform_trans(Vec3f translate)
// {
//     Matrix4f matrix = Matrix4f::translate(translate);
//     Transform translate_transform = Transform(matrix);
//     current_transform *= translate_transform;
// }
//
// void FeignRenderer::transform_lookat(Vec3f origin,
//                                      Vec3f target,
//                                      Vec3f up)
// {
//     Vec3f zaxis = (target - origin).normalized();
//     Vec3f xaxis = ((up.normalized()) ^ zaxis).normalized();
//     Vec3f yaxis = (zaxis ^ xaxis).normalized();
//
//     Matrix4f look_at_matrix = Matrix4f();
//
//     look_at_matrix.setCol(0, Vec4f(xaxis, 0.f));
//     look_at_matrix.setCol(1, Vec4f(yaxis, 0.f));
//     look_at_matrix.setCol(2, Vec4f(zaxis, 0.f));
//     look_at_matrix.setCol(3, Vec4f(origin, 1.f));
//
//     Transform look_at = Transform(look_at_matrix);
//     current_transform *= look_at;
// }
//
// void FeignRenderer::transform_matrix(Float a00, Float a01, Float a02, Float a03,
//                                      Float a10, Float a11, Float a12, Float a13,
//                                      Float a20, Float a21, Float a22, Float a23,
//                                      Float a30, Float a31, Float a32, Float a33)
// {
//     Matrix4f matrix(a00, a01, a02, a03,
//                     a10, a11, a12, a13,
//                     a20, a21, a22, a23,
//                     a30, a31, a32, a33);
//
//     Transform transform = Transform(matrix);
//
//     current_transform *= transform;
// }
//
// void FeignRenderer::transform_matrix(Matrix4f matrix)
// {
//     Transform transform = Transform(matrix);
//
//     current_transform *= transform;
// }
//
// void FeignRenderer::transform_transform(Transform transform)
// {
//     current_transform *= transform;
// }
//
// void FeignRenderer::transform_push()
// {
//     transform_stack.push_back(current_transform);
// }
//
// void FeignRenderer::transform_pop()
// {
//     current_transform = transform_stack[transform_stack.size()-1];
//     transform_stack.pop_back();
// }
