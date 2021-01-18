#include "wolf_head.h"
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>
#include <imedit/im_util.h>

FEIGN_BEGIN()

WolfHead::WolfHead()
{
    run();
}

void WolfHead::initialize_materials(int frame)
{
    WireframeMaterialShader::Params wireframe_params("diffuse_dark_mat",
                                                     "diffuse_light_mat",
                                                     0.01);

    FeignRenderer::fr_shader("diffuse_shad",
                             "wireframe",
                             &wireframe_params);

    SimpleMaterial::Params diffuse_light_mat_params("diffuse_light_bsdf");
    SimpleMaterial::Params diffuse_dark_mat_params("diffuse_dark_bsdf");

    // Diffuse::Params diffuse_light(Color3f(0.6f, 0.4f, 0.3f));
    Diffuse::Params diffuse_light(Color3f(0.01f, 0.2f, 0.2f));

    FeignRenderer::fr_material("diffuse_light_mat",
                               "simple",
                               &diffuse_light_mat_params);

    FeignRenderer::fr_material("diffuse_dark_mat",
                               "simple",
                               &diffuse_dark_mat_params);

    FeignRenderer::fr_bsdf("diffuse_light_bsdf",
                           "diffuse",
                           &diffuse_light);

    // Diffuse::Params diffuse_dark(Color3f(0.1f, 0.2f, 0.3f));
    Diffuse::Params diffuse_dark(Color3f(0.5f, 0.2f, 0.1f));

    FeignRenderer::fr_bsdf("diffuse_dark_bsdf",
                          "diffuse",
                          &diffuse_dark);
}

void WolfHead::initialize_grid_scene_2(int frame)
{
    int grid_dim = 10;

    // left wall //
    FeignRenderer::fr_scale(100.f, 1.f, 100.f);
    FeignRenderer::fr_rotate(90.f, 0.f, 0.f, 1.f);
    FeignRenderer::fr_translate(-50.f, 0.f, 0.f);

    FeignRenderer::fr_object("left_wall",
                             "left_wall_grid",
                             "diffuse_shad");

    GridObj::Params params_left_wall("", "diffuse_shad", Vec2i(grid_dim, grid_dim));
    // ObjMesh::Params params_left_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("left_wall_grid",
                           "grid",
                           &params_left_wall);
    FeignRenderer::fr_clear_transform();
    ////////////////
}

void WolfHead::initialize_initial_scene(int frame, bool verbose=false)
{
    if (frame < 600)
        return;

    Color3f abs = Color3f(0.3f);
    Color3f scat = Color3f(0.7f);

    Transform identity = Transform();
    identity = identity * Matrix4f::scale(Vector3f(0.7f, 0.7f, 0.7f));
    identity = identity * Matrix4f::translate(Vector3f(0.f, -5.f, 0.f));

    StandardMedium::Params media_params("ratio",
                                        "default",
                                        "delta_sampler",
                                        "vdb_density",
                                        identity,
                                        abs,
                                        scat);

    FeignRenderer::fr_media("box_medium",
                            "standard",
                            &media_params);

    FeignRenderer::fr_medium_transmittance("ratio",
                                           "ratio",
                                           nullptr);

    // OpenVDBDensity::Params vdb_density_params("/Users/corneria/Documents/Research/testscenes/vdb_smoke/cloud-1840.vdb");
    OpenVDBDensity::Params vdb_density_params("wolf_head/wolf_" + std::to_string(frame-599) + ".vdb");

    FeignRenderer::fr_medium_density("vdb_density",
                                     "openvdb",
                                     &vdb_density_params);

    // TODO: fix this
    FeignRenderer::fr_medium_sampling("delta_sampler",
                                      "delta",
                                      nullptr);

    FeignRenderer::fr_clear_transform();

    // FeignRenderer::fr_scale(3.f, 8.f, 3.f);
    FeignRenderer::fr_scale(6.f, 6.f, 6.f);
    FeignRenderer::fr_translate(0.f, 0.f, 0.f);

    FeignRenderer::fr_object("medium_bounds",
                             "box_mesh",
                             "default",
                             "null",
                             "box_medium");

    ObjMesh::Params box_mesh("../scenes/meshes/sphere_tri.obj",
                             "",
                             false,
                             "box_medium",
                             "null",
                             true);

    FeignRenderer::fr_mesh("box_mesh",
                           "triangle_mesh",
                           &box_mesh);

    FeignRenderer::fr_clear_transform();
}

void WolfHead::initialize_camera_2(int frame)
{
    Float xz_dist = std::sqrt(7.0 * 7.0 - 16.0);
    Float angle = 1.0 * M_PI * float(frame) / 480.f;
    Vector3f look_at = Vector3f(0.f, 1.f, 0.f);
    Float height = 4.0;
    Float fov = 30.f;

    fov = 43.f;
    height = 4.f;
    look_at = Vector3f(0.f, -0.5f, 1.f);
    xz_dist = 20.f;
    angle = 2.f * M_PI;

    Perspective::Params cam_params(Vector3f(xz_dist * cos(angle),
                                            height,
                                            xz_dist * sin(angle)+2.f),
                                   look_at,
                                   Vector3f(0, 1, 0),
                                   fov, // full res
                                   1e-4f,
                                   1e2f,
                                   10.f,
                                   0.f,
                                   // Vec2i(256, 256)); // debug res
                                   Vec2i(1920, 1080)); // full res

    FeignRenderer::fr_camera("camera",
                             "perspective",
                             &cam_params);
}

void WolfHead::initialize_base_structs(std::string test_name,
                                       int frame)
{
    char str[5];
    snprintf(str, 5, "%04d", frame);
    std::string name = test_name + "_" + std::string(str);

    FeignRenderer::initialize();

    FeignRenderer::fr_scene(name,
                            "integrator",
                            "sampler",
                            "camera",
                            "null",
                            false);

    int path_depth = 10;

    // if (frame < 180) path_depth = 7;
    // else path_depth = 10;

    Integrator::Params int_params(512,
                                  512,
                                  test_name + "/",
                                  path_depth);

    FeignRenderer::fr_integrator("integrator",
                                 "volpath",
                                 "default",
                                 &int_params);

    int samples = 8;
    if (frame < 180) samples = 2048;
    else samples = 2048 * 2;

    // samples = 16;
    // samples = 8;
    // samples = 8;
    // samples = 256;
    samples = 2048;

    Independent::Params samp_params(samples, 0x12345);

    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void WolfHead::initialize_lighting(int frame)
{
    PointEmitter::Params emitter_params(Color3f(50.f*30.f*1.5, 60.f*30.f*1.5, 37.f*200.f*1.5),
                                        Vector3f(4.0, 9.0, 4.0));

    FeignRenderer::fr_emitter("point_emitter",
                              "point",
                              &emitter_params);

    PointEmitter::Params emitter_params_2(Color3f(50.f*100.f, 300.f, 400.f),
                                          Vector3f(-4.0, -9.0, 4.0));

    FeignRenderer::fr_emitter("point_emitter_2",
                              "point",
                              &emitter_params_2);


    PointEmitter::Params emitter_params_3(Color3f(50.f*100.f*3, 300.f*3, 400.f*3),
                                        Vector3f(-10.0, 0, -2.0));

    FeignRenderer::fr_emitter("point_emitter_3",
                            "point",
                            &emitter_params_3);
}

void WolfHead::flush_render()
{
    FeignRenderer::flush_renders();
    FeignRenderer::clean_up();
}

void WolfHead::run()
{
    std::string test_name = "wolf_head_res";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";
    // std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/aaaaah/out" + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/aaaaah" + ".mp4";

    // system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 600;
    // int start_frame = 501;
    int end_frame = 900;

    // smoke medium
    for (int frame = start_frame; frame < end_frame; frame++)
    {
        initialize_base_structs(test_name, frame-500);

        initialize_camera_2(frame+500);

        initialize_materials(frame);

        initialize_grid_scene_2(frame);

        initialize_initial_scene(frame);

        initialize_lighting(frame);

        LOG("rendering frame: " + std::to_string(frame));

        flush_render();
    }

    system(publish_command.c_str());
}

FEIGN_END()
