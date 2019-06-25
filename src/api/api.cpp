#include <feign/api/api.h>

FeignRenderer::WorldNode* world = nullptr;
FeignRenderer::std::vector<Transform> transform_stack = std::vector<Transform>();
FeignRenderer::Node* current_node = nullptr;
FeignRenderer::Scene* current_scene = nullptr;
FeignRenderer::Transform current_transform = Transform();

void FeignRenderer::begin_world()
{
    // Note: this method will never delete the old world
    if (world) world = nullptr;
    world = new WorldNode();
    current_node = world;
    current_transform = Transform();
    transform_stack.clear();
}

void FeignRenderer::begin_scene()
{
    assert(!current_scene);

    current_transform.push_back(current_transform);
    Scene* scene = new Scene(current_node);
    current_node->addChild(scene);
    current_node = scene;
    current_scene = scene;
}

void FeignRenderer::begin_obj(const std::string& filename)
{
    current_transform.push_back(current_transform);
    ObjMesh* mesh = new ObjMesh(current_node, filename);
    current_node->addChild(mesh);
    current_node = mesh;

    // TODO
}

void FeignRenderer::end_world()
{
    assert(world);
    
    // TODO: need to fix all pre process calls
    world->preProcess();

    current_node = nullptr;
    current_scene = nullptr;
    world = nullptr;
}

void FeignRenderer::end_scene()
{
    current_transform = transform_stack[transform_stack.size() - 1];
    transform_stack.pop_back();
    current_node = current_node->getParent();
    current_scene = nullptr;
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

    if (shape->getMaterial()->getBSDf())
    {
        delete shape->getMaterial()->getBSDF();
    }

    shape->getMaterial()->setBSDF(new Diffuse(current_node, albedo));
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
                                       Float far)
{
    Perspective* perspective = new Perspective(current_node,
                                               aperatureRadius,
                                               focalDistance,
                                               fov,
                                               near,
                                               far);

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
}

void FeignRenderer::emitter_point(Color3f I, Point3f pos)
{
    Point3f actual_pos = current_transform * pos;

    PointEmitter* emitter = new PointEmitter(current_node, I, pos);

    current_scene->getEmitters().push_back(emitter);
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
