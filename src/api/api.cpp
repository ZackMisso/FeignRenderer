/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

// core includes
#include <feign/core/api.h>
#include <feign/core/accel_light.h>
#include <feign/core/accel_ray.h>
#include <feign/core/bsdf.h>
#include <feign/core/camera.h>
#include <feign/core/emitter.h>
#include <feign/core/recon_filter.h>
#include <feign/core/integrator.h>
#include <feign/core/sampler.h>
#include <feign/core/scene.h>
#include <feign/core/texture.h>

// media includes
#include <feign/media/media.h>
#include <feign/media/phase.h>
#include <feign/media/density_func.h>
#include <feign/media/sampling.h>
#include <feign/media/trans_est.h>
#include <feign/media/trans_func.h>

// shapes
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>

// stats
#include <feign/stats/clocker.h>

// TODO: make all of this logic more generic for more easily adding new classes
// TODO: rethink this node structure. storing everything as nodes instead of their
//       actual types is a little weird. Is there a cleaner way of doing this while
//       still allowing for compatibility with the eventual scene editor idea?

FEIGN_BEGIN()

FeignRenderer *FeignRenderer::instance = nullptr;

FeignRenderer::FeignRenderer(Imagef *target)
    : target(target)
{
    bsdfs = std::unordered_map<std::string, BSDFNode *>();
    cameras = std::unordered_map<std::string, CameraNode *>();
    emitters = std::unordered_map<std::string, EmitterNode *>();
    medias = std::unordered_map<std::string, MediaNode *>();
    integrators = std::unordered_map<std::string, IntegratorNode *>();
    samplers = std::unordered_map<std::string, SamplerNode *>();
    filters = std::unordered_map<std::string, FilterNode *>();
    objects = std::unordered_map<std::string, ObjectNode *>();
    meshes = std::unordered_map<std::string, MeshNode *>();
    geom_shaders = std::unordered_map<std::string, GeometryShaderNode *>();
    material_shaders = std::unordered_map<std::string, MaterialShaderNode *>();
    textures = std::unordered_map<std::string, TextureNode *>();
    density_funcs = std::unordered_map<std::string, DensityFunctionNode *>();
    phase_funcs = std::unordered_map<std::string, PhaseFunctionNode *>();
    medium_samplings = std::unordered_map<std::string, MediumSamplingNode *>();
    trans_ests = std::unordered_map<std::string, TransmittanceEstimatorNode *>();

    bsdfs.clear();
    cameras.clear();
    emitters.clear();
    medias.clear();
    integrators.clear();
    samplers.clear();
    filters.clear();
    objects.clear();
    meshes.clear();
    geom_shaders.clear();
    material_shaders.clear();
    textures.clear();
    density_funcs.clear();
    phase_funcs.clear();
    medium_samplings.clear();
    trans_ests.clear();

    current_transform = Transform();
    scene = nullptr;
}

FeignRenderer::~FeignRenderer()
{
#if CLOCKING
    Clocker::print_results();
    Clocker::deinitialize();
#endif

    for (auto it : bsdfs)
        delete it.second;
    for (auto it : cameras)
        delete it.second;
    for (auto it : emitters)
        delete it.second;
    for (auto it : medias)
        delete it.second;
    for (auto it : integrators)
        delete it.second;
    for (auto it : samplers)
        delete it.second;
    for (auto it : filters)
        delete it.second;
    for (auto it : objects)
        delete it.second;
    for (auto it : meshes)
        delete it.second;
    for (auto it : geom_shaders)
        delete it.second;
    for (auto it : material_shaders)
        delete it.second;
    for (auto it : textures)
        delete it.second;
    for (auto it : density_funcs)
        delete it.second;
    for (auto it : phase_funcs)
        delete it.second;
    for (auto it : medium_samplings)
        delete it.second;
    for (auto it : trans_ests)
        delete it.second;

    delete scene;
    scene = nullptr;
    target = nullptr;

    bsdfs.clear();
    cameras.clear();
    emitters.clear();
    medias.clear();
    integrators.clear();
    samplers.clear();
    filters.clear();
    objects.clear();
    meshes.clear();
    geom_shaders.clear();
    textures.clear();
    material_shaders.clear();
    density_funcs.clear();
    phase_funcs.clear();
    medium_samplings.clear();
    trans_ests.clear();
}

void FeignRenderer::initialize(Imagef *image)
{
    FeignRenderer::instance = new FeignRenderer(image);

#if CLOCKING
    Clocker::initialize();
#endif
}

BSDFNode *FeignRenderer::find_bsdf(std::string name)
{
    std::unordered_map<std::string, BSDFNode *>::const_iterator itr = bsdfs.find(name);

    if (itr == bsdfs.end())
    {
        BSDFNode *node = new BSDFNode(name);
        bsdfs.insert({name, node});

        if (name == "default")
        {
            node->bsdf = new NullBSDF();
        }

        return node;
    }
    else
    {
        return itr->second;
    }
}

IntegratorNode *FeignRenderer::find_integrator(std::string name)
{
    std::unordered_map<std::string, IntegratorNode *>::const_iterator itr = integrators.find(name);

    if (itr == integrators.end())
    {
        IntegratorNode *node = new IntegratorNode(name);
        integrators.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

CameraNode *FeignRenderer::find_camera(std::string name)
{
    std::unordered_map<std::string, CameraNode *>::const_iterator itr = cameras.find(name);

    if (itr == cameras.end())
    {
        CameraNode *node = new CameraNode(name);
        cameras.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

EmitterNode *FeignRenderer::find_emitter(std::string name)
{
    std::unordered_map<std::string, EmitterNode *>::const_iterator itr = emitters.find(name);

    if (name == "null")
    {
        return nullptr;
    }

    if (itr == emitters.end())
    {
        EmitterNode *node = new EmitterNode(name);
        emitters.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MediaNode *FeignRenderer::find_media(std::string name)
{
    LOG("Finding MEDIUM: " + name);
    std::unordered_map<std::string, MediaNode *>::const_iterator itr = medias.find(name);

    if (name == "null" || name == "default")
    {
        return nullptr;
    }

    if (itr == medias.end())
    {
        MediaNode *node = new MediaNode(name);
        medias.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

SamplerNode *FeignRenderer::find_sampler(std::string name)
{
    std::unordered_map<std::string, SamplerNode *>::const_iterator itr = samplers.find(name);

    if (itr == samplers.end())
    {
        SamplerNode *node = new SamplerNode(name);
        samplers.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

FilterNode *FeignRenderer::find_filter(std::string name)
{
    std::unordered_map<std::string, FilterNode *>::const_iterator itr = filters.find(name);

    if (itr == filters.end())
    {
        FilterNode *node = new FilterNode(name);
        filters.insert({name, node});

        if (name == "default")
        {
            // node->filter = new GaussFilter(Vec2f(4.0, 4.0), 2.0);
            node->filter = new GaussFilter(Vec2f(2.0, 2.0), 1.0);
            // node->filter = new BoxFilter();
        }

        return node;
    }
    else
    {
        return itr->second;
    }
}

ObjectNode *FeignRenderer::find_object(std::string name)
{
    std::unordered_map<std::string, ObjectNode *>::const_iterator itr = objects.find(name);

    if (itr == objects.end())
    {
        ObjectNode *node = new ObjectNode(name);
        objects.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MeshNode *FeignRenderer::find_mesh(std::string name)
{
    std::unordered_map<std::string, MeshNode *>::const_iterator itr = meshes.find(name);

    if (itr == meshes.end())
    {
        MeshNode *node = new MeshNode(name);
        meshes.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

GeometryShaderNode *FeignRenderer::find_geometry_shader(std::string name)
{
    std::unordered_map<std::string, GeometryShaderNode *>::const_iterator itr = geom_shaders.find(name);

    if (itr == geom_shaders.end())
    {
        GeometryShaderNode *node = new GeometryShaderNode(name);
        geom_shaders.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MaterialShaderNode *FeignRenderer::find_material_shader(std::string name)
{
    std::unordered_map<std::string, MaterialShaderNode *>::const_iterator itr = material_shaders.find(name);

    if (itr == material_shaders.end())
    {
        MaterialShaderNode *node = new MaterialShaderNode(name);
        material_shaders.insert({name, node});

        if (name == "default")
        {
            BSDFNode *bsdf = getInstance()->find_bsdf("default");
            node->shader = new SimpleMaterialShader(bsdf);
        }

        return node;
    }
    else
    {
        return itr->second;
    }
}

TextureNode *FeignRenderer::find_texture(std::string name)
{
    if (name == "" || name == "default")
    {
        return nullptr;
    }

    std::unordered_map<std::string, TextureNode *>::const_iterator itr = textures.find(name);

    if (itr == textures.end())
    {
        TextureNode *node = new TextureNode(name);
        textures.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

DensityFunctionNode *FeignRenderer::find_density_func(std::string name)
{
    if (name == "")
        return nullptr;

    std::unordered_map<std::string, DensityFunctionNode *>::const_iterator itr = density_funcs.find(name);

    if (itr == density_funcs.end())
    {
        LOG("did not find");
        DensityFunctionNode *node = new DensityFunctionNode(name);
        density_funcs.insert({name, node});
        return node;
    }
    else
    {
        LOG("found");
        return itr->second;
    }
}

PhaseFunctionNode *FeignRenderer::find_phase_func(std::string name)
{
    if (name == "")
        return nullptr;

    std::unordered_map<std::string, PhaseFunctionNode *>::const_iterator itr = phase_funcs.find(name);

    if (itr == phase_funcs.end())
    {
        PhaseFunctionNode *node = new PhaseFunctionNode(name);

        if (name == "default")
        {
            node->phase = new HenyeyGreenstein(0.f);
        }

        phase_funcs.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MediumSamplingNode *FeignRenderer::find_medium_sampling(std::string name)
{
    if (name == "")
        return nullptr;

    std::unordered_map<std::string, MediumSamplingNode *>::const_iterator itr = medium_samplings.find(name);

    if (itr == medium_samplings.end())
    {
        MediumSamplingNode *node = new MediumSamplingNode(name);

        if (name == "default")
        {
            node->sampling = new AnalyticalTrans_Samp();
        }

        medium_samplings.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

TransmittanceEstimatorNode *FeignRenderer::find_transmittance_estimator(std::string name)
{
    if (name == "")
        return nullptr;

    std::unordered_map<std::string, TransmittanceEstimatorNode *>::const_iterator itr = trans_ests.find(name);

    if (itr == trans_ests.end())
    {
        TransmittanceEstimatorNode *node = new TransmittanceEstimatorNode(name);

        if (name == "default")
        {
            // node->trans_est = new Trans_RatioTracking(1.0);
            node->trans_est = new Trans_Homogenous();
        }

        trans_ests.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

TransFuncNode *FeignRenderer::find_transmittance_func(std::string name)
{
    if (name == "")
        return nullptr;

    std::unordered_map<std::string, TransFuncNode *>::const_iterator itr = trans_funcs.find(name);

    if (itr == trans_funcs.end())
    {
        TransFuncNode *node = new TransFuncNode(name);

        if (name == "default")
        {
            node->trans_func = new ExpTrans();
        }

        trans_funcs.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

void FeignRenderer::fr_accel(std::string name,
                             std::string type,
                             void *accel_properties)
{
    CLOCKER_START_ONE(ClockerType::API)

    assert(getInstance()->scene);

    // TODO: incorporate spatial partitioning methods here

    if (type == "light_naive")
    {
        if (getInstance()->scene->scene->light_selection)
        {
            delete getInstance()->scene->scene->light_selection;
        }

        getInstance()->scene->scene->light_selection = new NaiveLightAccel();
    }
    else if (type == "light_spatial")
    {
        if (getInstance()->scene->scene->light_selection)
        {
            delete getInstance()->scene->scene->light_selection;
        }

        SpatialLightAccel::Params *params = (SpatialLightAccel::Params *)accel_properties;

        getInstance()->scene->scene->light_selection = new SpatialLightAccel(
            params->width,
            params->height,
            params->depth);
    }
    else
    {
        throw new NotImplementedException("unsupported accelerator: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_scene(std::string name,
                             std::string integrator_node,
                             std::string sampler_node,
                             std::string camera_node,
                             std::string medium_node,
                             std::string keywords,
                             std::string description,
                             bool sdf_mode)
{
    CLOCKER_START_ONE(ClockerType::API)

    std::cout << "sdf_mode: " << sdf_mode << std::endl;
    global_params.sdf_only = sdf_mode;
    std::cout << global_params.sdf_only << std::endl;

    if (getInstance()->scene)
    {
        throw new FeignRendererException("currently only one scene may be constructed");
    }

    IntegratorNode *integrator = getInstance()->find_integrator(integrator_node);
    SamplerNode *sampler = getInstance()->find_sampler(sampler_node);
    CameraNode *camera = getInstance()->find_camera(camera_node);
    MediaNode *media = nullptr;

    if (!medium_node.empty())
        media = getInstance()->find_media(medium_node);

    Scene *scene = new Scene(name,
                             keywords,
                             description,
                             integrator,
                             sampler,
                             camera,
                             media);

    scene->target = getInstance()->target;

    getInstance()->scene = new SceneNode(name, scene);

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_integrator(std::string name,
                                  std::string type,
                                  std::string filter,
                                  void *integrator_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    IntegratorNode *integrator = getInstance()->find_integrator(name);
    FilterNode *filter_node = getInstance()->find_filter(filter);

    if (integrator->integrator)
    {
        throw new FeignRendererException("integrator already defined");
    }

    if (type == "ao")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new Ambient_Occlusion_Integrator(filter_node, params);
    }
    else if (type == "normal")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new NormalIntegrator(filter_node, params);
    }
    else if (type == "nice_normal")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new NiceNormalIntegrator(filter_node, params);
    }
    else if (type == "whitted")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new WhittedIntegrator(filter_node, params);
    }
    else if (type == "cosine_term")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new CosineTermIntegrator(filter_node, params);
    }
    else if (type == "barycentric")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new BarycentricIntegrator(filter_node, params);
    }
    else if (type == "path")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new Path_Unidirectional_Integrator(filter_node, params);
    }
    else if (type == "volpath")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new VolPath_Integrator(filter_node, params);
    }
    else if (type == "volpath_nonexp")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new VolPathNonExp_Integrator(filter_node, params);
    }
    else if (type == "volpath_trans")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new VolPathTrans_Integrator(filter_node, params);
    }
    else if (type == "photon")
    {
        PhotonMapping::Params *params = (PhotonMapping::Params *)integrator_data;
        integrator->integrator = new PhotonMapping(filter_node, params);
    }
    else if (type == "sandbox")
    {
        Integrator::Params *params = (Integrator::Params *)integrator_data;
        integrator->integrator = new Sandbox_Integrator(filter_node, params);
    }
    else
    {
        throw new NotImplementedException("unsupported integrator: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_sampler(std::string name,
                               std::string type,
                               void *sampler_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    SamplerNode *sampler = getInstance()->find_sampler(name);

    if (sampler->sampler)
    {
        throw new FeignRendererException("sampler already defined");
    }

    if (type == "independent")
    {
        Independent::Params *params = (Independent::Params *)sampler_data;
        sampler->sampler = new Independent(params->seed, params->sample_cnt);
    }
    else if (type == "latin")
    {
        throw new NotImplementedException("latin sampler still unsupported");
    }
    else
    {
        throw new NotImplementedException("unsupported sampler: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_camera(std::string name,
                              std::string type,
                              void *camera_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    CameraNode *camera = getInstance()->find_camera(name);

    if (camera->camera)
    {
        throw new FeignRendererException("camera already defined");
    }

    if (type == "perspective")
    {
        Perspective::Params *params = (Perspective::Params *)camera_data;

        Perspective *perspective = new Perspective(params->app_radius,
                                                   params->focal_dist,
                                                   params->fov,
                                                   params->near,
                                                   params->far,
                                                   params->image_res[0],
                                                   params->image_res[1]);

        Vec3f zaxis = (params->target - params->origin).normalized();
        Vec3f xaxis = ((params->up.normalized()) ^ zaxis).normalized();
        Vec3f yaxis = (zaxis ^ xaxis).normalized();

        Matrix4f look_at_matrix = Matrix4f();

        look_at_matrix.setCol(0, Vec4f(xaxis, 0.f));
        look_at_matrix.setCol(1, Vec4f(yaxis, 0.f));
        look_at_matrix.setCol(2, Vec4f(zaxis, 0.f));
        look_at_matrix.setCol(3, Vec4f(params->origin, 1.f));

        perspective->setCameraToWorld(getInstance()->current_transform * Transform(look_at_matrix));

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

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_object(std::string name,
                              std::string mesh,
                              std::string material_shader,
                              std::string emitter,
                              std::string medium,
                              int index)
{
    CLOCKER_START_ONE(ClockerType::API)

    if (index >= 0)
        name += "_" + std::to_string(index);

    ObjectNode *object = getInstance()->find_object(name);

    // objects are allowed to have the same name and instead be refered to
    // by instance
    if (object->mesh)
    {
        fr_object(name,
                  mesh,
                  material_shader,
                  emitter,
                  medium,
                  index + 1);

        return;
    }

    object->transform = getInstance()->current_transform;
    object->mesh = getInstance()->find_mesh(mesh);
    object->material_shader = getInstance()->find_material_shader(material_shader);
    // object->medium = getInstance()->find_media(medium);

    if (emitter != "null" && emitter != "")
    {
        EmitterNode *emitter_node = getInstance()->find_emitter(emitter);
        emitter_node->objectNode = object;
        object->emitter = emitter_node;
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

// TODO: rename this to shape... since not just meshes are supported
// TODO: make a better way of storing all of these TODOs.
void FeignRenderer::fr_mesh(std::string name,
                            std::string type,
                            void *mesh_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    MeshNode *mesh = getInstance()->find_mesh(name);

    if (mesh->mesh)
    {
        throw new FeignRendererException("mesh already defined");
    }

    if (type == "triangle_mesh")
    {
        ObjMesh::Params *params = (ObjMesh::Params *)mesh_data;

        GeometryShaderNode *geom_shader = getInstance()->find_geometry_shader(params->shader);

        MediumBoundry *boundry = nullptr;

        if (params->inside_media != "null" || params->outside_media != "null")
        {
            MediaNode *inside = FeignRenderer::getInstance()->find_media(params->inside_media);
            MediaNode *outside = FeignRenderer::getInstance()->find_media(params->outside_media);

            boundry = new MediumBoundry(inside, outside);
        }

        mesh->mesh = new ObjMesh(params->filename,
                                 params->flip_norms,
                                 boundry,
                                 params->is_null);
        mesh->mesh->geomShader = geom_shader;
    }
    else if (type == "grid")
    {
        GridObj::Params *params = (GridObj::Params *)mesh_data;

        GeometryShaderNode *geom_shader = getInstance()->find_geometry_shader(params->shader);
        TextureNode *terrain_texture = getInstance()->find_texture(params->texture);

        mesh->mesh = new GridObj(params->resolution, terrain_texture);
        mesh->mesh->geomShader = geom_shader;
    }
    else if (type == "sdf_sphere")
    {
        SDFSphere::Params *params = (SDFSphere::Params *)mesh_data;

        MediumBoundry *boundry = nullptr;

        if (params->inside_media != "null" || params->outside_media != "null")
        {
            MediaNode *inside = FeignRenderer::getInstance()->find_media(params->inside_media);
            MediaNode *outside = FeignRenderer::getInstance()->find_media(params->outside_media);

            boundry = new MediumBoundry(inside, outside);
        }

        mesh->mesh = new SDFSphere(params->center,
                                   params->radius,
                                   params->interp,
                                   boundry,
                                   params->is_null);
    }
    else if (type == "sdf_plane")
    {
        SDFPlane::Params *params = (SDFPlane::Params *)mesh_data;

        MediumBoundry *boundry = nullptr;

        if (params->inside_media != "null" || params->outside_media != "null")
        {
            MediaNode *inside = FeignRenderer::getInstance()->find_media(params->inside_media);
            MediaNode *outside = FeignRenderer::getInstance()->find_media(params->outside_media);

            boundry = new MediumBoundry(inside, outside);
        }

        mesh->mesh = new SDFPlane(params->center,
                                  params->normal,
                                  params->interp,
                                  boundry,
                                  params->is_null);
    }
    else if (type == "sdf_box")
    {
        SDFBox::Params *params = (SDFBox::Params *)mesh_data;

        MediumBoundry *boundry = nullptr;

        if (params->inside_media != "null" || params->outside_media != "null")
        {
            MediaNode *inside = FeignRenderer::getInstance()->find_media(params->inside_media);
            MediaNode *outside = FeignRenderer::getInstance()->find_media(params->outside_media);

            boundry = new MediumBoundry(inside, outside);
        }

        mesh->mesh = new SDFBox(params->tlc,
                                params->brc,
                                boundry,
                                params->is_null);
    }
    else if (type == "sdf_cylinder")
    {
        SDFCylinder::Params *params = (SDFCylinder::Params *)mesh_data;

        MediumBoundry *boundry = nullptr;

        if (params->inside_media != "null" || params->outside_media != "null")
        {
            MediaNode *inside = FeignRenderer::getInstance()->find_media(params->inside_media);
            MediaNode *outside = FeignRenderer::getInstance()->find_media(params->outside_media);

            boundry = new MediumBoundry(inside, outside);
        }

        mesh->mesh = new SDFCylinder(params->first,
                                     params->second,
                                     params->radius,
                                     boundry,
                                     params->is_null);
    }
    else if (type == "sdf_cone")
    {
        SDFCone::Params *params = (SDFCone::Params *)mesh_data;

        MediumBoundry *boundry = nullptr;

        if (params->inside_media != "null" || params->outside_media != "null")
        {
            MediaNode *inside = FeignRenderer::getInstance()->find_media(params->inside_media);
            MediaNode *outside = FeignRenderer::getInstance()->find_media(params->outside_media);

            boundry = new MediumBoundry(inside, outside);
        }

        mesh->mesh = new SDFCone(params->first,
                                 params->second,
                                 params->radius_1,
                                 params->radius_2,
                                 boundry,
                                 params->is_null);
    }
    else
    {
        throw new NotImplementedException("mesh type not recognized: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_shader(std::string name,
                              std::string type,
                              void *shader_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    // TODO: incorporate other shader types later

    if (type == "interp_verts_to_sphere")
    {
        GeometryShaderNode *geom_shader = getInstance()->find_geometry_shader(name);

        if (geom_shader->shader)
        {
            throw new FeignRendererException("geometry shader already defined");
        }

        InterpVertsToSphereShader::Params *params = (InterpVertsToSphereShader::Params *)shader_data;
        geom_shader->shader = new InterpVertsToSphereShader(params->prop,
                                                            params->proxy);
    }
    else if (type == "simple_material")
    {
        MaterialShaderNode *material_shader = getInstance()->find_material_shader(name);

        if (material_shader->shader)
        {
            throw new FeignRendererException("material shader already defined");
        }

        SimpleMaterialShader::Params *params = (SimpleMaterialShader::Params *)shader_data;
        BSDFNode *bsdf = getInstance()->find_bsdf(params->bsdf);
        material_shader->shader = new SimpleMaterialShader(bsdf);
    }
    else if (type == "wireframe")
    {
        MaterialShaderNode *shader = getInstance()->find_material_shader(name);

        if (shader->shader)
        {
            throw new FeignRendererException("wireframe shader already defined");
        }

        WireframeMaterialShader::Params *params = (WireframeMaterialShader::Params *)shader_data;
        BSDFNode *wire_bsdf = getInstance()->find_bsdf(params->wireframe_mat);
        BSDFNode *mesh_bsdf = getInstance()->find_bsdf(params->mesh_mat);

        shader->shader = new WireframeMaterialShader(wire_bsdf,
                                                     mesh_bsdf,
                                                     params->threshold);
    }
    else
    {
        throw new NotImplementedException("shader type not recognized: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_media(std::string name,
                             void *medium_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    MediaNode *medium = getInstance()->find_media(name);

    if (medium->media)
    {
        throw new FeignRendererException("media already defined");
    }

    Media::Params *params =
        (Media::Params *)medium_data;

    TransmittanceEstimatorNode *trans_node =
        getInstance()->find_transmittance_estimator(params->trans_node);
    PhaseFunctionNode *phase_node =
        getInstance()->find_phase_func(params->phase_node);
    MediumSamplingNode *med_samp_node =
        getInstance()->find_medium_sampling(params->sampling_node);
    DensityFunctionNode *density_node =
        getInstance()->find_density_func(params->density_func_node);
    TransFuncNode *trans_func_node =
        getInstance()->find_transmittance_func(params->trans_func_node);

    medium->media = new Media(trans_node,
                              phase_node,
                              med_samp_node,
                              density_node,
                              trans_func_node,
                              params->transform,
                              params->abs,
                              params->scat);

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_medium_density(std::string name,
                                      std::string type,
                                      void *density_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    LOG("medium_density: " + name);
    DensityFunctionNode *density_func = getInstance()->find_density_func(name);

    if (density_func->density)
    {
        throw new FeignRendererException("density function already defined");
    }

    if (type == "constant")
    {
        ConstantDensity::Params *params = (ConstantDensity::Params *)density_data;
        density_func->density = new ConstantDensity(params->density);
    }
    else if (type == "point_average")
    {
        // TODO
        throw new NotImplementedException("point average density");
    }
    else if (type == "openvdb")
    {
        OpenVDBDensity::Params *params = (OpenVDBDensity::Params *)density_data;
        density_func->density = new OpenVDBDensity(params->filename);
    }
    else if (type == "noise")
    {
        // TODO
        throw new NotImplementedException("api noise denstiy");
    }
    else if (type == "mandlebrot")
    {
        MandlebrotDensity::Params *params = (MandlebrotDensity::Params *)density_data;
        density_func->density = new MandlebrotDensity(params->max_density);
    }
    else if (type == "sphere")
    {
        // assert(false);
        SphereDensity::Params *params = (SphereDensity::Params *)density_data;
        density_func->density = new SphereDensity(params);
        // if (!density_func->density) assert(false);
    }
    else
    {
        throw new NotImplementedException("medium density not recognized: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_medium_phase(std::string name,
                                    std::string type,
                                    void *phase_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    // TODO

    throw new NotImplementedException("medium phase type not recognized: " + type);

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_medium_sampling(std::string name,
                                       std::string type,
                                       void *sample_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    MediumSamplingNode *medium_sampling = getInstance()->find_medium_sampling(name);

    if (medium_sampling->sampling)
    {
        throw new FeignRendererException("medium sampling already defined");
    }

    if (type == "delta")
    {
        medium_sampling->sampling = new Delta_Tracking(10.0);
    }
    else if (type == "ray")
    {
        if (sample_data)
        {
            Ray_Marching_Samp::Params *params = (Ray_Marching_Samp::Params *)sample_data;
            medium_sampling->sampling = new Ray_Marching_Samp(params->step_size);
        }
        else
        {
            medium_sampling->sampling = new Ray_Marching_Samp();
        }
    }
    else if (type == "equi")
    {
        // TODO
        throw new NotImplementedException("api equi sampling");
    }
    else if (type == "constant")
    {
        medium_sampling->sampling = new AnalyticalTrans_Samp();
    }
    else
    {
        throw new NotImplementedException("medium sampling not recognized: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_medium_transmittance(std::string name,
                                            std::string type,
                                            void *trans_est_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    TransmittanceEstimatorNode *trans_est = getInstance()->find_transmittance_estimator(name);

    if (trans_est->trans_est)
    {
        throw new FeignRendererException("transmittance estimator already defined");
    }

    if (type == "homo")
    {
        trans_est->trans_est = new Trans_Homogenous();
    }
    else if (type == "delta")
    {
        trans_est->trans_est = new Trans_DeltaTracking();
    }
    else if (type == "ratio")
    {
        if (trans_est_data)
        {
            Trans_RatioTracking::Params *params = (Trans_RatioTracking::Params *)trans_est_data;
            trans_est->trans_est = new Trans_RatioTracking(params->maj);
        }
        else
        {
            trans_est->trans_est = new Trans_RatioTracking(10.0);
        }
    }
    else if (type == "pseries")
    {
        trans_est->trans_est = new Trans_PseriesCMF();
    }
    else if (type == "ray")
    {
        if (trans_est_data)
        {
            Trans_RayMarching::Params *params = (Trans_RayMarching::Params *)trans_est_data;
            trans_est->trans_est = new Trans_RayMarching(params->step_size);
        }
        else
        {
            trans_est->trans_est = new Trans_RayMarching();
        }
    }
    else
    {
        throw new NotImplementedException("transmittance estimator not recognized: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_medium_transmittance_func(std::string name,
                                                 std::string type,
                                                 void *trans_func_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    TransFuncNode *trans_func = getInstance()->find_transmittance_func(name);

    if (trans_func->trans_func)
    {
        throw new FeignRendererException("transmittance function already defined");
    }

    if (type == "exp")
    {
        trans_func->trans_func = new ExpTrans();
    }
    else if (type == "lin")
    {
        if (trans_func_data)
        {
            LinearTrans::Params *params = (LinearTrans::Params *)trans_func_data;
            trans_func->trans_func = new LinearTrans(params->maxT);
        }
        else
        {
            trans_func->trans_func = new LinearTrans(1.f);
        }
    }
    else
    {
        throw new NotImplementedException("transmittance function not recognized: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_emitter(std::string name,
                               std::string type,
                               void *emitter_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    EmitterNode *emitter = getInstance()->find_emitter(name);

    if (emitter->emitter)
    {
        throw new FeignRendererException("emitter already defined");
    }

    if (type == "point")
    {
        // assert(getInstance()->scene);
        PointEmitter::Params *params = (PointEmitter::Params *)emitter_data;
        emitter->emitter = new PointEmitter(params->intensity, params->pos);
        getInstance()->scene->scene->addEmitter(emitter->emitter);
    }
    else if (type == "mesh")
    {
        // TODO: should the mesh logic be handled here
        MeshEmitter::Params *params = (MeshEmitter::Params *)emitter_data;
        emitter->emitter = new MeshEmitter(params->intensity);

        getInstance()->scene->scene->addEmitter(emitter->emitter);
    }
    else if (type == "environment")
    {
        EnvironmentEmitter::Params *params = (EnvironmentEmitter::Params *)emitter_data;

        TextureNode *texture = getInstance()->find_texture(params->texture);
        emitter->emitter = new EnvironmentEmitter(texture, params->intensity);
        getInstance()->scene->scene->addEmitter(emitter->emitter);
    }
    else if (type == "directional")
    {
        DirectionalEmitter::Params *params = (DirectionalEmitter::Params *)emitter_data;
        emitter->emitter = new DirectionalEmitter(params->light_dir, params->radiance);
        getInstance()->scene->scene->addEmitter(emitter->emitter);
    }
    else if (type == "directional_mesh")
    {
        DirectionalMeshEmitter::Params *params =
            (DirectionalMeshEmitter::Params *)emitter_data;
        emitter->emitter = new DirectionalMeshEmitter(params->light_dir,
                                                      params->intensity);

        getInstance()->scene->scene->addEmitter(emitter->emitter);
    }
    else if (type == "spot")
    {
        SpotLightEmitter::Params *params = (SpotLightEmitter::Params *)emitter_data;
        emitter->emitter = new SpotLightEmitter(params->light_pos,
                                                params->light_dir,
                                                params->radiance,
                                                params->light_angle);
        getInstance()->scene->scene->addEmitter(emitter->emitter);
    }
    else
    {
        throw new NotImplementedException("emitter type not recognized: " + type);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

// void FeignRenderer::fr_material(std::string name,
//                                 std::string type,
//                                 void* material_data)
// {
//     MaterialNode* material = getInstance()->find_material(name);
//
//     if (material->material)
//     {
//         throw new FeignRendererException("material already defined");
//     }
//
//     if (type == "simple")
//     {
//         SimpleMaterial::Params* params = (SimpleMaterial::Params*)material_data;
//
//         BSDFNode* bsdf = getInstance()->find_bsdf(params->bsdf_name);
//
//         material->material = new SimpleMaterial(bsdf);
//     }
//     else
//     {
//         throw new FeignRendererException("material type not recognized: " + type);
//     }
// }

void FeignRenderer::fr_bsdf(std::string name,
                            std::string type,
                            void *bsdf_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    BSDFNode *bsdf = getInstance()->find_bsdf(name);

    if (bsdf->bsdf)
    {
        throw new FeignRendererException("bsdf already defined");
    }

    if (type == "diffuse")
    {
        Diffuse::Params *params = (Diffuse::Params *)bsdf_data;
        bsdf->bsdf = new Diffuse(params->albedo);
    }
    else if (type == "mirror")
    {
        Mirror::Params *params = (Mirror::Params *)bsdf_data;
        bsdf->bsdf = new Mirror(params->albedo);
    }
    else if (type == "dielectric")
    {
        Dielectric::Params *params = (Dielectric::Params *)bsdf_data;
        bsdf->bsdf = new Dielectric(params->int_ior,
                                    params->ext_ior,
                                    params->albedo);
    }
    else if (type == "one_way_dielectric")
    {
        OneWayDielectric::Params *params = (OneWayDielectric::Params *)bsdf_data;
        bsdf->bsdf = new OneWayDielectric(params->int_ior,
                                          params->ext_ior,
                                          params->albedo);
    }
    // TODO: there should not be two null bsdfs
    else if (type == "nullbsdf")
    {
        bsdf->bsdf = new NullBSDF();
    }
    else if (type == "null")
    {
        bsdf->bsdf = nullptr;
    }
    else if (type == "phong")
    {
        Phong::Params *params = (Phong::Params *)bsdf_data;
        bsdf->bsdf = new Phong(params->kd, params->ks, params->exponent);
    }
    else if (type == "blinn")
    {
        Blinn::Params *params = (Blinn::Params *)bsdf_data;
        bsdf->bsdf = new Blinn(params);
    }
    else
    {
        // unrecognized bsdf
        std::cout << "ERROR: BSDF : " << type << " : does not exist" << std::endl;
        assert(false);
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_texture(std::string name,
                               std::string type,
                               void *texture_data)
{
    CLOCKER_START_ONE(ClockerType::API)

    TextureNode *texture = getInstance()->find_texture(name);

    if (texture->texture)
    {
        throw new FeignRendererException("texture already defined");
    }

    if (type == "image")
    {
        ImageTexture::Params *params = (ImageTexture::Params *)texture_data;
        texture->texture = new ImageTexture(params->filename, params->scale);
        // image textures should always be preprocessed to actually parse the
        // images
        texture->texture->preProcess();
        LOG("inside here");
    }
    else if (type == "sin")
    {
        SinTexture::Params *params = (SinTexture::Params *)texture_data;
        texture->texture = new SinTexture(params->amp,
                                          params->phase,
                                          params->freq,
                                          params->y);
    }
    else
    {
        throw new FeignRendererException(type + " texture not supported");
    }

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_clear_transform()
{
    CLOCKER_START_ONE(ClockerType::API)

    getInstance()->current_transform = Transform();

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_scale(float sx, float sy, float sz)
{
    CLOCKER_START_ONE(ClockerType::API)

    Matrix4f matrix = Matrix4f::scale(Vec3f(sx, sy, sz));
    Transform scale_transform = Transform(matrix);
    getInstance()->current_transform = scale_transform * getInstance()->current_transform;

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_translate(float tx, float ty, float tz)
{
    CLOCKER_START_ONE(ClockerType::API)

    Matrix4f matrix = Matrix4f::translate(Vec3f(tx, ty, tz));
    Transform translate_transform = Transform(matrix);
    getInstance()->current_transform = translate_transform * getInstance()->current_transform;

    CLOCKER_STOP_ONE(ClockerType::API)
}

void FeignRenderer::fr_rotate(float angle, float x, float y, float z)
{
    CLOCKER_START_ONE(ClockerType::API)

    Matrix4f matrix = Matrix4f::rotate(angle, Vec3f(x, y, z));
    Transform rotate_transform = Transform(matrix);
    getInstance()->current_transform = rotate_transform * getInstance()->current_transform;

    CLOCKER_STOP_ONE(ClockerType::API)
}

// this is the big one
#if CLOCKING
void FeignRenderer::flush_renders(ClockerResults* clockings)
#elif
void FeignRenderer::flush_renders()
#endif
{
    CLOCKER_START_ONE(ClockerType::API)

    LOG("global_params.sdf_only flush: " + std::to_string(global_params.sdf_only));
    Scene *scene_obj = getInstance()->scene->scene;

    // first preprocess all meshes
    unsigned int inst_index = 0;

    LOG("pre processing meshes");

    for (auto it : getInstance()->objects)
    {
        Shape *mesh = it.second->mesh->mesh;

        if (!mesh)
        {
            throw new FeignRendererException("mesh is not initialized");
        }

        mesh->transform = it.second->transform * mesh->transform;
        // preprocess more information for importance sampling mesh emitters
        mesh->preProcess(it.second->emitter != nullptr);
        scene_obj->shapes.push_back(mesh);
        scene_obj->objects.push_back(it.second);
        mesh->setInstID(inst_index);

        inst_index++;
    }

    LOG("pre processing emitters");

    // TODO: set this up in a more efficient way
    for (auto it : getInstance()->emitters)
    {
        if (it.second->objectNode)
        {
            it.second->emitter->setMeshNode(it.second->objectNode->mesh);
        }
    }

    LOG("pre processing media");

    global_params.name = "blah";

    for (auto it : getInstance()->medias)
    {
        LOG("MEDIUM");
        if (it.first != "null")
        {
            scene_obj->addMedium(it.second->media);
        }
    }

    LOG("preprocessing scene");

    // preprocess the scene
    scene_obj->preProcess(global_params);

    global_params.name = "blah";

    CLOCKER_START_STOP_ONE(ClockerType::RENDERING, ClockerType::API)

    LOG("entering render");

    // render current scene
    scene_obj->renderScene();

    LOG("render done, cleaning up");

    CLOCKER_STOP_ONE(ClockerType::RENDERING)
#if CLOCKING
    Clocker::return_results(clockings);
#endif

    // clean up used memory after the renders
    clean_up();
}

void FeignRenderer::clean_up()
{
    if (FeignRenderer::instance)
        delete FeignRenderer::instance;
    FeignRenderer::instance = nullptr;
}

FEIGN_END()
