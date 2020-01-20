/**
 * Author:    Zackary Misso
 * Version:   0.1.1
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
#include <feign/core/texture.h>

// media includes
#include <feign/media/media.h>
#include <feign/media/phase/phase.h>

// shapes
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>


FeignRenderer* FeignRenderer::instance = nullptr;

FeignRenderer::FeignRenderer()
{
    bsdfs = std::unordered_map<std::string, BSDFNode*>();
    cameras = std::unordered_map<std::string, CameraNode*>();
    emitters = std::unordered_map<std::string, EmitterNode*>();
    medias = std::unordered_map<std::string, MediaNode*>();
    integrators = std::unordered_map<std::string, IntegratorNode*>();
    samplers = std::unordered_map<std::string, SamplerNode*>();
    filters = std::unordered_map<std::string, FilterNode*>();
    materials = std::unordered_map<std::string, MaterialNode*>();
    objects = std::unordered_map<std::string, ObjectNode*>();
    meshes = std::unordered_map<std::string, MeshNode*>();
    geom_shaders = std::unordered_map<std::string, GeometryShaderNode*>();
    material_shaders = std::unordered_map<std::string, MaterialShaderNode*>();
    textures = std::unordered_map<std::string, TextureNode*>();

    bsdfs.clear();
    cameras.clear();
    emitters.clear();
    medias.clear();
    integrators.clear();
    samplers.clear();
    filters.clear();
    materials.clear();
    objects.clear();
    meshes.clear();
    geom_shaders.clear();
    material_shaders.clear();
    textures.clear();

    current_transform = Transform();
    scene = nullptr;
}

FeignRenderer::~FeignRenderer()
{
    for (auto it : bsdfs) delete it.second;
    for (auto it : cameras) delete it.second;
    for (auto it : emitters) delete it.second;
    for (auto it : medias) delete it.second;
    for (auto it : integrators) delete it.second;
    for (auto it : samplers) delete it.second;
    for (auto it : filters) delete it.second;
    for (auto it : materials) delete it.second;
    for (auto it : objects) delete it.second;
    for (auto it : meshes) delete it.second;
    for (auto it : geom_shaders) delete it.second;
    for (auto it : material_shaders) delete it.second;
    for (auto it : textures) delete it.second;

    delete scene;
    scene = nullptr;

    bsdfs.clear();
    cameras.clear();
    emitters.clear();
    medias.clear();
    integrators.clear();
    samplers.clear();
    filters.clear();
    materials.clear();
    objects.clear();
    meshes.clear();
    geom_shaders.clear();
    textures.clear();
    material_shaders.clear();
}

void FeignRenderer::initialize()
{
    FeignRenderer::instance = new FeignRenderer();
}

BSDFNode* FeignRenderer::find_bsdf(std::string name)
{
    std::unordered_map<std::string, BSDFNode*>::const_iterator itr = bsdfs.find(name);

    if (itr == bsdfs.end())
    {
        BSDFNode* node = new BSDFNode(name);
        bsdfs.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

IntegratorNode* FeignRenderer::find_integrator(std::string name)
{
    std::unordered_map<std::string, IntegratorNode*>::const_iterator itr = integrators.find(name);

    if (itr == integrators.end())
    {
        IntegratorNode* node = new IntegratorNode(name);
        integrators.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

CameraNode* FeignRenderer::find_camera(std::string name)
{
    std::unordered_map<std::string, CameraNode*>::const_iterator itr = cameras.find(name);

    if (itr == cameras.end())
    {
        CameraNode* node = new CameraNode(name);
        cameras.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

EmitterNode* FeignRenderer::find_emitter(std::string name)
{
    std::unordered_map<std::string, EmitterNode*>::const_iterator itr = emitters.find(name);

    if (itr == emitters.end())
    {
        EmitterNode* node = new EmitterNode(name);
        emitters.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MediaNode* FeignRenderer::find_media(std::string name)
{
    std::unordered_map<std::string, MediaNode*>::const_iterator itr = medias.find(name);

    if (itr == medias.end())
    {
        MediaNode* node = new MediaNode(name);
        medias.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

SamplerNode* FeignRenderer::find_sampler(std::string name)
{
    std::unordered_map<std::string, SamplerNode*>::const_iterator itr = samplers.find(name);

    if (itr == samplers.end())
    {
        SamplerNode* node = new SamplerNode(name);
        samplers.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

FilterNode* FeignRenderer::find_filter(std::string name)
{
    std::unordered_map<std::string, FilterNode*>::const_iterator itr = filters.find(name);

    if (itr == filters.end())
    {
        FilterNode* node = new FilterNode(name);
        filters.insert({name, node});

        if (name == "default")
        {
            node->filter = new GaussFilter(Vec2f(2.0, 2.0), 2.0);
        }

        return node;
    }
    else
    {
        return itr->second;
    }
}

MaterialNode* FeignRenderer::find_material(std::string name)
{
    std::unordered_map<std::string, MaterialNode*>::const_iterator itr = materials.find(name);

    if (itr == materials.end())
    {
        MaterialNode* node = new MaterialNode(name);
        materials.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

ObjectNode* FeignRenderer::find_object(std::string name)
{
    std::unordered_map<std::string, ObjectNode*>::const_iterator itr = objects.find(name);

    if (itr == objects.end())
    {
        ObjectNode* node = new ObjectNode(name);
        objects.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MeshNode* FeignRenderer::find_mesh(std::string name)
{
    std::unordered_map<std::string, MeshNode*>::const_iterator itr = meshes.find(name);

    if (itr == meshes.end())
    {
        MeshNode* node = new MeshNode(name);
        meshes.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

GeometryShaderNode* FeignRenderer::find_geometry_shader(std::string name)
{
    std::unordered_map<std::string, GeometryShaderNode*>::const_iterator itr = geom_shaders.find(name);

    if (itr == geom_shaders.end())
    {
        GeometryShaderNode* node = new GeometryShaderNode(name);
        geom_shaders.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

MaterialShaderNode* FeignRenderer::find_material_shader(std::string name)
{
    std::unordered_map<std::string, MaterialShaderNode*>::const_iterator itr = material_shaders.find(name);

    if (itr == material_shaders.end())
    {
        MaterialShaderNode* node = new MaterialShaderNode(name);
        material_shaders.insert({name, node});
        return node;
    }
    else
    {
        return itr->second;
    }
}

TextureNode* FeignRenderer::find_texture(std::string name)
{
    std::unordered_map<std::string, TextureNode*>::const_iterator itr = textures.find(name);

    if (itr == textures.end())
    {
        TextureNode* node = new TextureNode(name);
        textures.insert({name, node});
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
    if (getInstance()->scene)
    {
        throw new FeignRendererException("currently only one scene may be constructed");
    }

    IntegratorNode* integrator = getInstance()->find_integrator(integrator_node);
    SamplerNode* sampler = getInstance()->find_sampler(sampler_node);
    CameraNode* camera = getInstance()->find_camera(camera_node);
    MediaNode* media = nullptr;

    if (!medium_node.empty()) media = getInstance()->find_media(medium_node);

    getInstance()->scene = new SceneNode(name, new Scene(name,
                                                         integrator,
                                                         sampler,
                                                         camera,
                                                         media));
}

void FeignRenderer::fr_integrator(std::string name,
                                  std::string type,
                                  std::string filter,
                                  void* integrator_data)
{
    IntegratorNode* integrator = getInstance()->find_integrator(name);
    FilterNode* filter_node = getInstance()->find_filter(filter);

    if (integrator->integrator)
    {
        throw new FeignRendererException("integrator already defined");
    }

    if (type == "normal")
    {
        Integrator::Params* params = (Integrator::Params*)integrator_data;
        integrator->integrator = new NormalIntegrator(filter_node,
                                                      params->location,
                                                      params->max_time,
                                                      params->max_heuristic);
    }
    else if (type == "nice_normal")
    {
        Integrator::Params* params = (Integrator::Params*)integrator_data;
        integrator->integrator = new NiceNormalIntegrator(filter_node,
                                                          params->location,
                                                          params->max_time,
                                                          params->max_heuristic);
    }
    else if (type == "whitted")
    {
        Integrator::Params* params = (Integrator::Params*)integrator_data;
        integrator->integrator = new WhittedIntegrator(filter_node,
                                                       params->location,
                                                       params->max_time,
                                                       params->max_heuristic);
    }
    else if (type == "cosine_term")
    {
        Integrator::Params* params = (Integrator::Params*)integrator_data;
        integrator->integrator = new CosineTermIntegrator(filter_node,
                                                          params->location,
                                                          params->max_time,
                                                          params->max_heuristic);
    }
    else if (type == "barycentric")
    {
        Integrator::Params* params = (Integrator::Params*)integrator_data;
        integrator->integrator = new BarycentricIntegrator(filter_node,
                                                           params->location,
                                                           params->max_time,
                                                           params->max_heuristic);
    }
    else
    {
        throw new NotImplementedException("unsupported integrator: " + type);
    }
}

void FeignRenderer::fr_sampler(std::string name,
                               std::string type,
                               void* sampler_data)
{
    LOG("samplers: " + std::to_string(getInstance()->samplers.size()));
    SamplerNode* sampler = getInstance()->find_sampler(name);

    if (sampler->sampler)
    {
        throw new FeignRendererException("sampler already defined");
    }

    if (type == "independent")
    {
        Independent::Params* params = (Independent::Params*)sampler_data;
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
}

void FeignRenderer::fr_camera(std::string name,
                              std::string type,
                              void* camera_data)
{
    CameraNode* camera = getInstance()->find_camera(name);

    if (camera->camera)
    {
        throw new FeignRendererException("camera already defined");
    }

    if (type == "perspective")
    {
        Perspective::Params* params = (Perspective::Params*)camera_data;

        Perspective* perspective = new Perspective(params->app_radius,
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
}

void FeignRenderer::fr_object(std::string name,
                              std::string mesh,
                              std::string material_shader,
                              int index)
{
    if (index >= 0) name += "_" + std::to_string(index);

    ObjectNode* object = getInstance()->find_object(name);

    // objects are allowed to have the same name and instead be refered to
    // by instance
    if (object->mesh)
    {
        fr_object(name,
                  mesh,
                  material_shader,
                  index+1);

        return;
    }

    object->transform = getInstance()->current_transform;
    object->mesh = getInstance()->find_mesh(mesh);
    object->material_shader = getInstance()->find_material_shader(material_shader);
}

void FeignRenderer::fr_mesh(std::string name,
                            std::string type,
                            void* mesh_data)
{
    MeshNode* mesh = getInstance()->find_mesh(name);

    if (mesh->mesh)
    {
        throw new FeignRendererException("mesh already defined");
    }

    if (type == "triangle_mesh")
    {
        ObjMesh::Params* params = (ObjMesh::Params*)mesh_data;

        GeometryShaderNode* geom_shader = getInstance()->find_geometry_shader(params->shader);

        mesh->mesh = new ObjMesh(params->filename);
        mesh->mesh->geomShader = geom_shader;
    }
    else if (type == "grid")
    {
        GridObj::Params* params = (GridObj::Params*)mesh_data;

        GeometryShaderNode* geom_shader = getInstance()->find_geometry_shader(params->shader);
        TextureNode* terrain_texture = getInstance()->find_texture(params->texture);

        mesh->mesh = new GridObj(params->resolution, terrain_texture);
        mesh->mesh->geomShader = geom_shader;
    }
    else if (type == "sdf_sphere")
    {
        SDFSphere::Params* params = (SDFSphere::Params*)mesh_data;

        mesh->mesh = new SDFSphere(params->center,
                                   params->radius,
                                   params->interp);
    }
    else
    {
        throw new NotImplementedException("mesh type not recognized: " + type);
    }
}

void FeignRenderer::fr_shader(std::string name,
                              std::string type,
                              void* shader_data)
{
    // TODO: incorporate other shader types later

    if (type == "interp_verts_to_sphere")
    {
        GeometryShaderNode* geom_shader = getInstance()->find_geometry_shader(name);

        if (geom_shader->shader)
        {
            throw new FeignRendererException("geometry shader already defined");
        }

        InterpVertsToSphereShader::Params* params = (InterpVertsToSphereShader::Params*)shader_data;
        geom_shader->shader = new InterpVertsToSphereShader(params->prop,
                                                            params->proxy);
    }
    else if (type == "simple_material")
    {
        MaterialShaderNode* material_shader = getInstance()->find_material_shader(name);

        if (material_shader->shader)
        {
            throw new FeignRendererException("material shader already defined");
        }

        SimpleMaterialShader::Params* params = (SimpleMaterialShader::Params*)shader_data;
        MaterialNode* material = getInstance()->find_material(params->material);
        material_shader->shader = new SimpleMaterialShader(material);
    }
    else if (type == "wireframe")
    {
        MaterialShaderNode* shader = getInstance()->find_material_shader(name);

        if (shader->shader)
        {
            throw new FeignRendererException("wireframe shader already defined");
        }

        WireframeMaterialShader::Params* params = (WireframeMaterialShader::Params*)shader_data;
        MaterialNode* wire_mat = getInstance()->find_material(params->wireframe_mat);
        MaterialNode* mesh_mat = getInstance()->find_material(params->mesh_mat);

        shader->shader = new WireframeMaterialShader(wire_mat,
                                                     mesh_mat,
                                                     params->threshold);
    }
    else if (type == "radar")
    {
        MaterialShaderNode* shader = getInstance()->find_material_shader(name);

        if (shader->shader)
        {
            throw new FeignRendererException("radar shader already defined");
        }

        RadarMaterialShader::Params* params = (RadarMaterialShader::Params*)shader_data;
        MaterialNode* radar_mat = getInstance()->find_material(params->radar_mat);
        MaterialNode* mesh_mat = getInstance()->find_material(params->mesh_mat);

        shader->shader = new RadarMaterialShader(radar_mat,
                                                 mesh_mat,
                                                 params->start_points,
                                                 params->end_dist,
                                                 params->start_times,
                                                 params->end_times,
                                                 params->band_width,
                                                 params->fall_off,
                                                 params->proxy);
    }
    else
    {
        throw new NotImplementedException("shader type not recognized: " + type);
    }
}

void FeignRenderer::fr_emitter(std::string name,
                               std::string type,
                               std::string mesh,
                               std::string material,
                               void* emitter_data)
{
    EmitterNode* emitter = getInstance()->find_emitter(name);

    if (emitter->emitter)
    {
        throw new FeignRendererException("emitter already defined");
    }

    if (type == "point")
    {
        assert(getInstance()->scene);
        PointEmitter::Params* params = (PointEmitter::Params*)emitter_data;
        emitter->emitter = new PointEmitter(params->intensity, params->pos);
        getInstance()->scene->scene->emitters.push_back(emitter->emitter);
    }
    else
    {
        throw new NotImplementedException("emitter type not recognized: " + type);
    }
}

void FeignRenderer::fr_material(std::string name,
                                std::string type,
                                void* material_data)
{
    MaterialNode* material = getInstance()->find_material(name);

    if (material->material)
    {
        throw new FeignRendererException("material already defined");
    }

    if (type == "simple")
    {
        SimpleMaterial::Params* params = (SimpleMaterial::Params*)material_data;

        BSDFNode* bsdf = getInstance()->find_bsdf(params->bsdf_name);

        material->material = new SimpleMaterial(bsdf);
    }
    else if (type == "wireframe")
    {
        throw new FeignRendererException("zack broke this too");
        // WireframeMaterial::Params* params = (WireframeMaterial::Params*)material_data;
        //
        // BSDFNode* wireframe_bsdf = getInstance()->find_bsdf(params->wireframe_bsdf);
        // BSDFNode* mesh_bsdf = getInstance()->find_bsdf(params->mesh_bsdf);
        //
        // material->material = new WireframeMaterial(wireframe_bsdf,
        //                                            mesh_bsdf,
        //                                            params->threshold);
    }
    else if (type == "radar")
    {
        throw new FeignRendererException("zack broke this");
        // RadarMaterial::Params* params = (RadarMaterial::Params*)material_data;
        //
        // BSDFNode* radar_bsdf = getInstance()->find_bsdf(params->radar_bsdf);
        // BSDFNode* mesh_bsdf = getInstance()->find_bsdf(params->mesh_bsdf);
        //
        // material->material = new RadarMaterial(radar_bsdf,
        //                                        mesh_bsdf,
        //                                        params->start_points,
        //                                        params->end_dist,
        //                                        params->start_times,
        //                                        params->end_times,
        //                                        params->band_width,
        //                                        params->fall_off,
        //                                        params->proxy);
    }
    else
    {
        throw new FeignRendererException("material type not recognized: " + type);
    }
}

void FeignRenderer::fr_bsdf(std::string name,
                            std::string type,
                            void* bsdf_data)
{
    BSDFNode* bsdf = getInstance()->find_bsdf(name);

    if (bsdf->bsdf)
    {
        throw new FeignRendererException("bsdf already defined");
    }

    if (type == "diffuse")
    {
        Diffuse::Params* params = (Diffuse::Params*)bsdf_data;
        bsdf->bsdf = new Diffuse(params->albedo);
    }
    else if (type == "mirror")
    {
        Mirror::Params* params = (Mirror::Params*)bsdf_data;
        bsdf->bsdf = new Mirror(params->albedo);
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
    else
    {
        // unrecognized bsdf
        std::cout << "ERROR: BSDF : " << type << " : does not exist" << std::endl;
        assert(false);
    }
}

void FeignRenderer::fr_texture(std::string name,
                               std::string type,
                               void* texture_data)
{
    TextureNode* texture = getInstance()->find_texture(name);

    if (texture->texture)
    {
        throw new FeignRendererException("texture already defined");
    }

    if (type == "image")
    {
        ImageTexture::Params* params = (ImageTexture::Params*)texture_data;
        texture->texture = new ImageTexture(params->filename, params->scale);
    }
    else if (type == "sin")
    {
        SinTexture::Params* params = (SinTexture::Params*)texture_data;
        texture->texture = new SinTexture(params->amp,
                                          params->phase,
                                          params->freq,
                                          params->y);
    }
    else
    {
        throw new FeignRendererException(type + " texture not supported");
    }
}

void FeignRenderer::fr_clear_transform()
{
    getInstance()->current_transform = Transform();
}

void FeignRenderer::fr_scale(float sx, float sy, float sz)
{
    Matrix4f matrix = Matrix4f::scale(Vec3f(sx, sy, sz));
    Transform scale_transform = Transform(matrix);
    getInstance()->current_transform = scale_transform * getInstance()->current_transform;
}

void FeignRenderer::fr_translate(float tx, float ty, float tz)
{
    Matrix4f matrix = Matrix4f::translate(Vec3f(tx, ty, tz));
    Transform translate_transform = Transform(matrix);
    getInstance()->current_transform = translate_transform * getInstance()->current_transform;
}

void FeignRenderer::fr_rotate(float angle, float x, float y, float z)
{
    Matrix4f matrix = Matrix4f::rotate(angle, Vec3f(x, y, z));
    Transform rotate_transform = Transform(matrix);
    getInstance()->current_transform = rotate_transform * getInstance()->current_transform;
}

// this is the big one
void FeignRenderer::flush_renders()
{
    // if (global_params.sdf_only)
    // {
    //     LOG("should work 6");
    // }

    Scene* scene_obj = getInstance()->scene->scene;

    // if (global_params.sdf_only)
    // {
    //     LOG("should work4");
    // }

    assert(scene_obj);

    // first preprocess all meshes
    unsigned int inst_index = 0;
    for (auto it : getInstance()->objects)
    {
        Shape* mesh = it.second->mesh->mesh;

        if (!mesh)
        {
            throw new FeignRendererException("mesh is not initialized");
        }

        mesh->transform = it.second->transform * mesh->transform;
        mesh->preProcess();
        scene_obj->shapes.push_back(mesh);
        scene_obj->objects.push_back(it.second);
        mesh->setInstID(inst_index);

        inst_index++;
    }

    // if (global_params.sdf_only)
    // {
    //     LOG("should work5");
    // }

    // preprocess the scene
    scene_obj->preProcess();

    // render current scene
    scene_obj->renderScene();

    // clean up used memory after the renders
    clean_up();
}

void FeignRenderer::clean_up()
{
    if (FeignRenderer::instance) delete FeignRenderer::instance;
    FeignRenderer::instance = nullptr;
}
