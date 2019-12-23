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
std::unordered_map<std::string, ObjectNode*>               FeignRenderer::objects = std::unordered_map<std::string, ObjectNode*>();
std::unordered_map<std::string, MeshNode*>                 FeignRenderer::meshes = std::unordered_map<std::string, MeshNode*>();

// this is temporary delete this later
std::vector<ObjectNode*> temp_list = std::vector<ObjectNode*>();

BSDFNode* FeignRenderer::find_bsdf(std::string name)
{
    std::unordered_map<std::string, BSDFNode*>::const_iterator itr = FeignRenderer::bsdfs.find(name);

    if (itr == FeignRenderer::bsdfs.end())
    {
        BSDFNode* node = new BSDFNode(name);
        FeignRenderer::bsdfs.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

IntegratorNode* FeignRenderer::find_integrator(std::string name)
{
    std::unordered_map<std::string, IntegratorNode*>::const_iterator itr = FeignRenderer::integrators.find(name);

    if (itr == FeignRenderer::integrators.end())
    {
        IntegratorNode* node = new IntegratorNode(name);
        FeignRenderer::integrators.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

CameraNode* FeignRenderer::find_camera(std::string name)
{
    std::unordered_map<std::string, CameraNode*>::const_iterator itr = FeignRenderer::cameras.find(name);

    if (itr == FeignRenderer::cameras.end())
    {
        CameraNode* node = new CameraNode(name);
        FeignRenderer::cameras.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

EmitterNode* FeignRenderer::find_emitter(std::string name)
{
    std::unordered_map<std::string, EmitterNode*>::const_iterator itr = FeignRenderer::emitters.find(name);

    if (itr == FeignRenderer::emitters.end())
    {
        EmitterNode* node = new EmitterNode(name);
        FeignRenderer::emitters.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MediaNode* FeignRenderer::find_media(std::string name)
{
    std::unordered_map<std::string, MediaNode*>::const_iterator itr = FeignRenderer::medias.find(name);

    if (itr == FeignRenderer::medias.end())
    {
        MediaNode* node = new MediaNode(name);
        FeignRenderer::medias.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

SamplerNode* FeignRenderer::find_sampler(std::string name)
{
    std::unordered_map<std::string, SamplerNode*>::const_iterator itr = FeignRenderer::samplers.find(name);

    if (itr == FeignRenderer::samplers.end())
    {
        SamplerNode* node = new SamplerNode(name);
        FeignRenderer::samplers.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

FilterNode* FeignRenderer::find_filter(std::string name)
{
    std::unordered_map<std::string, FilterNode*>::const_iterator itr = FeignRenderer::filters.find(name);

    if (itr == FeignRenderer::filters.end())
    {
        FilterNode* node = new FilterNode(name);
        FeignRenderer::filters.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MaterialNode* FeignRenderer::find_material(std::string name)
{
    std::unordered_map<std::string, MaterialNode*>::const_iterator itr = FeignRenderer::materials.find(name);

    if (itr == FeignRenderer::materials.end())
    {
        MaterialNode* node = new MaterialNode(name);
        FeignRenderer::materials.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

ObjectNode* FeignRenderer::find_object(std::string name)
{
    std::unordered_map<std::string, ObjectNode*>::const_iterator itr = FeignRenderer::objects.find(name);

    if (itr == FeignRenderer::objects.end())
    {
        ObjectNode* node = new ObjectNode(name);
        FeignRenderer::objects.insert({name, node});
        // TODO: DELETE THIS
        temp_list.push_back(node);
        // blahblahblah
        return node;
    }
    else
    {
        return itr->second;
    }
}

MeshNode* FeignRenderer::find_mesh(std::string name)
{
    std::unordered_map<std::string, MeshNode*>::const_iterator itr = FeignRenderer::meshes.find(name);

    if (itr == FeignRenderer::meshes.end())
    {
        MeshNode* node = new MeshNode(name);
        FeignRenderer::meshes.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

void FeignRenderer::fr_scene(std::string name,
                             std::string integrator_node,
                             std::string sampler_node,
                             std::string camera_node,
                             std::string medium_node)
{
    if (scene)
    {
        throw new FeignRendererException("currently only one scene may be constructed");
    }

    IntegratorNode* integrator = find_integrator(integrator_node);
    SamplerNode* sampler = find_sampler(sampler_node);
    CameraNode* camera = find_camera(camera_node);
    MediaNode* media = nullptr;

    if (!medium_node.empty()) media = find_media(medium_node);

    scene = new SceneNode(name, new Scene(name,
                                          integrator,
                                          sampler,
                                          camera,
                                          media));
}

void FeignRenderer::fr_integrator(std::string name,
                                  std::string type,
                                  long max_time,
                                  long max_heuristic)
{
    IntegratorNode* integrator = find_integrator(name);

    if (integrator->integrator)
    {
        throw new FeignRendererException("integrator already defined");
    }

    if (type == "normal")
    {
        integrator->integrator = new NormalIntegrator(max_time, max_heuristic);
    }
    else if (type == "whitted")
    {
        integrator->integrator = new WhittedIntegrator(max_time, max_heuristic);
    }
    else
    {
        throw new NotImplementedException("unsupported integrator: " + type);
    }
}

void FeignRenderer::fr_sampler(std::string name,
                               std::string type,
                               int spp,
                               long seed,
                               long seed2)
{
    SamplerNode* sampler = find_sampler(name);

    if (sampler->sampler)
    {
        throw new FeignRendererException("sampler already defined");
    }

    if (type == "independent")
    {
        sampler->sampler = new Independent(seed, spp);
    }
    else if (type == "latin")
    {
        throw new NotImplementedException("latin sampler still unsupported");
    }
    else
    {
        throw new NotImplementedException("unsupported sampler: " + type);
    }
}

void FeignRenderer::fr_camera(std::string name,
                              std::string type,
                              Vector3f origin,
                              Vector3f target,
                              Vector3f up,
                              Float fov,
                              Float near,
                              Float far,
                              Float focal_dist,
                              Float app_radius,
                              Vec2i image_res)
{
    CameraNode* camera = find_camera(name);

    if (camera->camera)
    {
        throw new FeignRendererException("camera already defined");
    }

    if (type == "perspective")
    {
        Perspective* perspective = new Perspective(app_radius,
                                                   focal_dist,
                                                   fov,
                                                   near,
                                                   far,
                                                   image_res[0],
                                                   image_res[1]);

        Vec3f zaxis = (target - origin).normalized();
        Vec3f xaxis = ((up.normalized()) ^ zaxis).normalized();
        Vec3f yaxis = (zaxis ^ xaxis).normalized();

        Matrix4f look_at_matrix = Matrix4f();

        look_at_matrix.setCol(0, Vec4f(xaxis, 0.f));
        look_at_matrix.setCol(1, Vec4f(yaxis, 0.f));
        look_at_matrix.setCol(2, Vec4f(zaxis, 0.f));
        look_at_matrix.setCol(3, Vec4f(origin, 1.f));

        perspective->setTransform(current_transform * Transform(look_at_matrix));

        camera->camera = perspective;
    }
    else if (type == "orthographic")
    {
        throw new NotImplementedException("orthographic not supported yet" + type);
    }
    else
    {
        throw new NotImplementedException("unsupported camera type: " + type);
    }
}

void FeignRenderer::fr_object(std::string name,
                              std::string mesh,
                              std::string material,
                              int index)
{
    if (index >= 0) name += "_" + std::to_string(index);

    ObjectNode* object = find_object(name);

    // objects are allowed to have the same name and instead be refered to
    // by instance
    if (object->mesh)
    {
        fr_object(name,
                  mesh,
                  material,
                  index+1);

        return;
    }

    object->transform = current_transform;
    object->mesh = find_mesh(mesh);
    object->material = find_material(material);
}

void FeignRenderer::fr_mesh(std::string name,
                            std::string type,
                            std::string filename)
{
    LOG("MESHES: " + std::to_string(meshes.size()));
    MeshNode* mesh = find_mesh(name);
    LOG("MESHES 2: " + std::to_string(meshes.size()));

    if (!mesh)
    {
        LOG("WHAT NOW");
    }

    LOG("creating_mesh: " + name);

    if (mesh->mesh)
    {
        throw new FeignRendererException("mesh already defined");
    }

    if (type == "triangle_mesh")
    {
        mesh->mesh = new ObjMesh(filename);
    }
    else
    {
        throw new NotImplementedException("mesh type not recognized: " + type);
    }
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
    MaterialNode* material = find_material(name);

    if (material->material)
    {
        throw new FeignRendererException("material already defined");
    }

    material->material = new Material();
    material->material->bsdf = find_bsdf(bsdf);
}

void FeignRenderer::fr_bsdf(std::string name,
                            std::string type,
                            Color3f albedo)
{
    BSDFNode* bsdf = find_bsdf(name);

    if (bsdf->bsdf)
    {
        throw new FeignRendererException("bsdf already defined");
    }

    if (type == "diffuse")
    {
        throw new NotImplementedException("fr_bsdf diffuse");
        // Diffuse*
        // TODO
        // Diffuse
    }
    else if (type == "null")
    {
        bsdf->bsdf = nullptr;
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
    current_transform = Transform();
}

void FeignRenderer::fr_scale(float sx, float sy, float sz)
{
    Matrix4f matrix = Matrix4f::scale(Vec3f(sx, sy, sz));
    Transform scale_transform = Transform(matrix);
    current_transform *= scale_transform;
}

void FeignRenderer::fr_translate(float tx, float ty, float tz)
{
    Matrix4f matrix = Matrix4f::translate(Vec3f(tx, ty, tz));
    Transform translate_transform = Transform(matrix);
    current_transform *= translate_transform;
}

void FeignRenderer::fr_rotate(float angle, float x, float y, float z)
{
    Matrix4f matrix = Matrix4f::rotate(angle, Vec3f(x, y, z));
    Transform rotate_transform = Transform(matrix);
    current_transform *= rotate_transform;
}

// this is the big one
void FeignRenderer::flush_renders()
{
    Scene* scene_obj = scene->scene;

    assert(scene_obj);

    // first preprocess all meshes
    LOG("pre mesh preprocessing");

    // TODO: how do you iterate through an unordered map???
    for (int i = 0; i < temp_list.size(); ++i)
    {
        Shape* mesh = temp_list[i]->mesh->mesh;

        if (!mesh)
        {
            LOG(std::to_string(temp_list.size()));
            throw new FeignRendererException("mesh is not initialized");
        }

        mesh->transform = temp_list[i]->transform * mesh->transform;
        mesh->preProcess();
        scene_obj->shapes.push_back(mesh);
    }

    LOG("post mesh preprocessing");

    // preprocess the scene
    scene_obj->preProcess();

    // render current scene
    LOG("rendering scene");
    scene_obj->renderScene();

    // clean up used memory after the renders
    clean_up();

    // throw new NotImplementedException("flush_renders");
    // TODO
}

void FeignRenderer::clean_up()
{
    throw new NotImplementedException("clean_up");
    // TODO
}

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
