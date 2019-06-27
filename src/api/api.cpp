#include <feign/api/api.h>

// bsdfs
#include <feign/bsdfs/diffuse.h>

// cameras
#include <feign/cameras/ortho.h>
#include <feign/cameras/perspective.h>

// emitters
#include <feign/emitters/point.h>

// filters
#include <feign/filters/box_filter.h>
#include <feign/filters/gauss_filter.h>

// integrators
#include <feign/integrators/light_unidir.h>
#include <feign/integrators/normal.h>
#include <feign/integrators/path_bidir.h>
#include <feign/integrators/path_unidir.h>
#include <feign/integrators/whitted.h>

// materials
#include <feign/materials/material.h>

// media
#include <feign/media/media.h>

// phases
#include <feign/phase/phase.h>

// samplers
#include <feign/samplers/halton.h>
#include <feign/samplers/independent.h>
#include <feign/samplers/latin.h>

// shapes
#include <feign/shapes/objmesh.h>

// textures
#include <feign/textures/texture.h>

// scenes
#include <feign/scene.h>

WorldNode* FeignRenderer::world = nullptr;
std::vector<Transform> FeignRenderer::transform_stack = std::vector<Transform>();
Node* FeignRenderer::current_node = nullptr;
Scene* FeignRenderer::current_scene = nullptr;
Transform FeignRenderer::current_transform = Transform();

void FeignRenderer::begin_world()
{
    // Note: this method will never delete the old world
    if (world) world = nullptr;
    world = new WorldNode();
    current_node = world;
    current_transform = Transform();
    transform_stack.clear();
}

void FeignRenderer::begin_scene(const std::string& name)
{
    assert(!current_scene);

    transform_stack.push_back(current_transform);
    Scene* scene = new Scene(current_node);
    scene->setSceneName(name);
    current_node->addChild(scene);
    current_node = scene;
    current_scene = scene;
}

void FeignRenderer::begin_node()
{
    throw new NotImplementedException("api begin node");
}

void FeignRenderer::begin_obj(const std::string& filename,
                              bool flip_norms)
{
    transform_stack.push_back(current_transform);

    ObjMesh* mesh = new ObjMesh(current_node, filename);
    current_node->addChild(mesh);
    current_node = mesh;

    mesh->parseFromFile(filename,
                        current_transform,
                        flip_norms);
}

WorldNode* FeignRenderer::end_world()
{
    assert(world);

    // TODO: need to fix all pre process calls
    world->preProcess(false);

    WorldNode* tmp = world;

    current_node = nullptr;
    current_scene = nullptr;
    world = nullptr;

    return tmp;
}

void FeignRenderer::end_scene()
{
    current_transform = transform_stack[transform_stack.size() - 1];
    transform_stack.pop_back();
    current_node = current_node->getParent();
    current_scene = nullptr;
}

void FeignRenderer::end_node()
{
    throw new NotImplementedException("api end node");
}

void FeignRenderer::end_obj()
{
    current_transform = transform_stack[transform_stack.size() - 1];
    transform_stack.pop_back();
    current_node = current_node->getParent();
}

void FeignRenderer::bsdf_diffuse(Color3f albedo)
{
    assert(current_node->getNodeType() == NT_Mesh);

    Shape* shape = (Shape*)current_node;

    // TODO: get materials designed and working
    if (!shape->getMaterial())
    {
        shape->setMaterial(new Material());
    }

    if (shape->getMaterial()->getBSDF())
    {
        delete shape->getMaterial()->getBSDF();
    }

    shape->getMaterial()->setBSDF(new Diffuse(current_node, albedo));

    current_node->addChild(shape->getMaterial()->getBSDF());
}

void FeignRenderer::camera_ortho(Vec3f ori,
                                 Vec3f tar,
                                 Vec3f up)
{
    throw new NotImplementedException("api camera_ortho");
}

void FeignRenderer::camera_perspective(Vec3f ori,
                                       Vec3f tar,
                                       Vec3f up,
                                       Float aperatureRadius,
                                       Float focalDistance,
                                       Float fov,
                                       Float near,
                                       Float far,
                                       uint32_t width,
                                       uint32_t height)
{
    Perspective* perspective = new Perspective(current_node,
                                               aperatureRadius,
                                               focalDistance,
                                               fov,
                                               near,
                                               far,
                                               width,
                                               height);

    transform_stack.push_back(current_transform);
    transform_lookat(ori, tar, up);

    perspective->setCameraToWorld(current_transform);

    current_transform = transform_stack[transform_stack.size()-1];
    transform_stack.pop_back();

    if (current_scene->getCamera())
    {
        delete current_scene->getCamera();
    }

    current_scene->setCamera(perspective);
    current_scene->addChild(perspective);
}

void FeignRenderer::integrator_light_unidir()
{
    assert(current_scene);

    Light_Unidirectional_Integrator* integrator = new Light_Unidirectional_Integrator(current_scene);

    if (current_scene->getIntegrator())
    {
        delete current_scene->getIntegrator();
    }

    current_scene->setIntegrator(integrator);
    current_scene->addChild(integrator);
}

void FeignRenderer::integrator_normal()
{
    assert(current_scene);

    NormalIntegrator* integrator = new NormalIntegrator(current_scene);

    if (current_scene->getIntegrator())
    {
        delete current_scene->getIntegrator();
    }

    current_scene->setIntegrator(integrator);
    current_scene->addChild(integrator);
}

void FeignRenderer::integrator_path_bidir()
{
    assert(current_scene);

    Path_Bidirectional_Integrator* integrator = new Path_Bidirectional_Integrator(current_scene);

    if (current_scene->getIntegrator())
    {
        delete current_scene->getIntegrator();
    }

    current_scene->setIntegrator(integrator);
    current_scene->addChild(integrator);
}

void FeignRenderer::integrator_path_unidir()
{
    assert(current_scene);

    Path_Unidirectional_Integrator* integrator = new Path_Unidirectional_Integrator(current_scene);

    if (current_scene->getIntegrator())
    {
        delete current_scene->getIntegrator();
    }

    current_scene->setIntegrator(integrator);
    current_scene->addChild(integrator);
}

void FeignRenderer::integrator_whitted()
{
    assert(current_scene);

    WhittedIntegrator* integrator = new WhittedIntegrator(current_scene);

    if (current_scene->getIntegrator())
    {
        delete current_scene->getIntegrator();
    }

    current_scene->setIntegrator(integrator);
    current_scene->addChild(integrator);
}

void FeignRenderer::sampler_independent(uint32_t sample_cnt, uint32_t seed)
{
    assert(current_scene);

    Independent* sampler = new Independent(current_scene,
                                           seed,
                                           sample_cnt);

    if (current_scene->getSampler())
    {
        delete current_scene->getSampler();
    }

    current_scene->setSampler(sampler);
    current_scene->addChild(sampler);
}

void FeignRenderer::sampler_halton(uint32_t sample_cnt, uint32_t seed)
{
    assert(current_scene);

    Halton* sampler = new Halton(current_scene,
                                 seed,
                                 sample_cnt);

    if (current_scene->getSampler())
    {
        delete current_scene->getSampler();
    }

    current_scene->setSampler(sampler);
    current_scene->addChild(sampler);
}

void FeignRenderer::sampler_latin(uint32_t sample_cnt, uint32_t seed)
{
    assert(current_scene);

    Latin* sampler = new Latin(current_scene,
                               seed,
                               sample_cnt);

    if (current_scene->getSampler())
    {
        delete current_scene->getSampler();
    }

    current_scene->setSampler(sampler);
    current_scene->addChild(sampler);
}

void FeignRenderer::emitter_point(Color3f I, Point3f pos)
{
    Point3f actual_pos = current_transform * pos;

    PointEmitter* emitter = new PointEmitter(current_node, I, pos);

    current_scene->addEmitter(emitter);
    current_node->addChild(emitter);
}

void FeignRenderer::transform_scale(Vec3f scale)
{
    Matrix4f matrix = Matrix4f::scale(scale);
    Transform scale_transform = Transform(matrix);
    current_transform *= scale_transform;
}

void FeignRenderer::transform_rot(Float angle, Vec3f axis)
{
    throw new NotImplementedException("api transform_rot");
}

void FeignRenderer::transform_trans(Vec3f translate)
{
    Matrix4f matrix = Matrix4f::translate(translate);
    Transform translate_transform = Transform(matrix);
    current_transform *= translate_transform;
}

void FeignRenderer::transform_lookat(Vec3f origin,
                                     Vec3f target,
                                     Vec3f up)
{
    Vec3f zaxis = (target - origin).normalized();
    Vec3f xaxis = ((up.normalized()) ^ zaxis).normalized();
    Vec3f yaxis = (zaxis ^ xaxis).normalized();

    Matrix4f look_at_matrix = Matrix4f();

    look_at_matrix.setCol(0, Vec4f(xaxis, 0.f));
    look_at_matrix.setCol(1, Vec4f(yaxis, 0.f));
    look_at_matrix.setCol(2, Vec4f(zaxis, 0.f));
    look_at_matrix.setCol(3, Vec4f(origin, 1.f));

    Transform look_at = Transform(look_at_matrix);
    current_transform *= look_at;
}

void FeignRenderer::transform_matrix(Float a00, Float a01, Float a02, Float a03,
                                     Float a10, Float a11, Float a12, Float a13,
                                     Float a20, Float a21, Float a22, Float a23,
                                     Float a30, Float a31, Float a32, Float a33)
{
    Matrix4f matrix(a00, a01, a02, a03,
                    a10, a11, a12, a13,
                    a20, a21, a22, a23,
                    a30, a31, a32, a33);

    Transform transform = Transform(matrix);

    current_transform *= transform;
}

void FeignRenderer::transform_matrix(Matrix4f matrix)
{
    Transform transform = Transform(matrix);

    current_transform *= transform;
}

void FeignRenderer::transform_transform(Transform transform)
{
    current_transform *= transform;
}

void FeignRenderer::transform_push()
{
    transform_stack.push_back(current_transform);
}

void FeignRenderer::transform_pop()
{
    current_transform = transform_stack[transform_stack.size()-1];
    transform_stack.pop_back();
}
