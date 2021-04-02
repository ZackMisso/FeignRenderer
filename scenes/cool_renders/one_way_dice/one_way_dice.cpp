#include "one_way_dice.h"
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>
#include <imedit/im_util.h>

FEIGN_BEGIN()

OneWayDice::OneWayDice()
{
    run();
}

void OneWayDice::initialize_materials(int frame)
{
    WireframeMaterialShader::Params wireframe_params("diffuse_dark_mat",
                                                     "diffuse_light_mat",
                                                     0.01);

    FeignRenderer::fr_shader("diffuse_shad",
                             "wireframe",
                             &wireframe_params);

    SimpleMaterial::Params diffuse_light_mat_params("diffuse_light_bsdf");
    SimpleMaterial::Params diffuse_dark_mat_params("diffuse_dark_bsdf");
    SimpleMaterial::Params one_way_mat_params("one_way_bsdf");
    SimpleMaterial::Params diel_mat_params("diel_bsdf");

    Diffuse::Params diffuse_light(Color3f(0.6f, 0.4f, 0.3f));
    Diffuse::Params diffuse_dark(Color3f(0.1f, 0.2f, 0.3f));
    // Diffuse::Params diffuse_light(Color3f(0.01f, 0.2f, 0.2f));

    FeignRenderer::fr_material("diffuse_light_mat",
                               "simple",
                               &diffuse_light_mat_params);

    FeignRenderer::fr_material("diffuse_dark_mat",
                               "simple",
                               &diffuse_dark_mat_params);

    FeignRenderer::fr_bsdf("diffuse_light_bsdf",
                           "diffuse",
                           &diffuse_light);

    FeignRenderer::fr_bsdf("diffuse_dark_bsdf",
                           "diffuse",
                           &diffuse_dark);

    SimpleMaterialShader::Params diffuse_light_shad("diffuse_light_mat");
    FeignRenderer::fr_shader("diffuse_light_shad",
                             "simple_material",
                             &diffuse_light_shad);

    SimpleMaterialShader::Params diffuse_dark_shad("diffuse_dark_mat");
    FeignRenderer::fr_shader("diffuse_dark_shad",
                             "simple_material",
                             &diffuse_dark_shad);

    Float int_eta = 1.0;
    Float b_scale = 0.90;
    Float g_scale = 0.93;
    Color3f albedo = Color3f(1.f, g_scale, b_scale);

    if(frame < 100)
    {
        albedo = Color3f(0.f);
    }
    else if (frame < 460)
    {
        double al_val = std::min(double(frame-100) / double(360.0), 1.0);
        albedo = Color3f(al_val, al_val * g_scale, al_val * b_scale);
        int_eta = 1.0;
    }
    else if (frame < 820) int_eta = double(frame - 460) / double(360.0) * 0.5 + 1.0;
    else if (frame < 1000)
    {
        double al_val = 1.0-std::min(double(frame-820) / double(396.0+36.0), 1.0);
        albedo = Color3f(al_val, al_val * g_scale, al_val * b_scale);
        // albedo = Color3f(1.0-std::min(double(frame-820) / double(396.0), 1.0));
        int_eta = 1.5;
    }
    else if (frame < 1180)
    {
        double al_val = 1.0-std::min(double(frame-820) / double(396.0+36.0), 1.0);
        albedo = Color3f(al_val, al_val * g_scale, al_val * b_scale);
        // albedo = Color3f(1.0-std::min(double(frame-820) / double(396.0), 1.0));
        int_eta = 0.5 - double(frame - 1000) / double(180.0) * 0.5 + 1.0;
    }
    else if (frame < 1500)
    {
        double al_val = 0.2f;
        albedo = Color3f(al_val, al_val * g_scale, al_val * b_scale);
        int_eta = 1.0;
    }
    else if (frame < 1550)
    {
        double al_val = (1.0 - double(frame - 1500) / 50.0) * 0.2;
        albedo = Color3f(al_val, al_val * g_scale, al_val * b_scale);
        int_eta = 1.0;
    }
    else
    {
        int_eta = 1.0;
        albedo = Color3f(0.f);
    }

    OneWayDielectric::Params one_way_bsdf(int_eta, 1.0, albedo);

    FeignRenderer::fr_bsdf("one_way_bsdf",
                           "one_way_dielectric",
                           &one_way_bsdf);

    FeignRenderer::fr_material("one_way_mat",
                               "simple",
                               &one_way_mat_params);

    SimpleMaterialShader::Params one_way_shad("one_way_mat");
    FeignRenderer::fr_shader("one_way_shad",
                             "simple_material",
                             &one_way_shad);

    Dielectric::Params diel_bsdf(int_eta, 1.0, albedo);

    FeignRenderer::fr_bsdf("diel_bsdf",
                           "dielectric",
                           &diel_bsdf);

    FeignRenderer::fr_material("diel_mat",
                          "simple",
                          &diel_mat_params);

    SimpleMaterialShader::Params diel_shad("diel_mat");
    FeignRenderer::fr_shader("diel_shad",
                             "simple_material",
                             &diel_shad);

    // Diffuse::Params diffuse_dark(Color3f(0.1f, 0.2f, 0.3f));
    // Diffuse::Params diffuse_dark(Color3f(0.5f, 0.2f, 0.1f));
}

void OneWayDice::initialize_grid(int frame)
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

void OneWayDice::initialize_scene(int frame)
{
    FeignRenderer::fr_rotate(double(frame) / 2.f, 0.f, 1.f, 0.f);
    FeignRenderer::fr_scale(0.5f, 0.5f, 0.5f);

    FeignRenderer::fr_object("dice_outside",
                             "dice_outside_mesh",
                             "diel_shad");

    FeignRenderer::fr_object("dice_inside",
                             "dice_inside_mesh",
                             "one_way_shad");

    // GridObj::Params params_left_wall("", "diffuse_shad", Vec2i(grid_dim, grid_dim));
    // ObjMesh::Params params_left_wall("../scenes/meshes/plane.obj", "");


    ObjMesh::Params outside_mesh("../scenes/meshes/dice/glass_dice_full_outside.obj",
                                 "",
                                 false);

    FeignRenderer::fr_mesh("dice_outside_mesh",
                           "triangle_mesh",
                           &outside_mesh);

    ObjMesh::Params inside_mesh("../scenes/meshes/dice/glass_dice_full_inside.obj",
                                "",
                                false);

    FeignRenderer::fr_mesh("dice_inside_mesh",
                          "triangle_mesh",
                          &inside_mesh);

    FeignRenderer::fr_clear_transform();
}

void OneWayDice::initialize_camera(int frame)
{
    Float xz_dist = std::sqrt(7.0 * 7.0 - 16.0);
    Float angle = 1.0 * M_PI * float(frame) / 480.f;
    Vector3f look_at = Vector3f(0.f, 1.f, 0.f);
    Float height = 4.0;
    Float fov = 30.f;

    fov = 60.f;
    height = 4.f;
    look_at = Vector3f(0.f, -0.5f, 0.f);
    xz_dist = 20.f;
    angle = 2.f * M_PI;

    Perspective::Params cam_params(Vector3f(20.f,
                                            height,
                                            1.f),
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

void OneWayDice::initialize_base_structs(std::string test_name,
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

    int path_depth = 5;

    // if (frame < 180) path_depth = 7;
    // else path_depth = 10;

    Integrator::Params int_params(512,
                                  512,
                                  test_name + "/",
                                  path_depth);

    FeignRenderer::fr_integrator("integrator",
                                 "path",
                                 "default",
                                 &int_params);

    int samples = 8;
    if (frame < 180) samples = 2048;
    else samples = 2048 * 2;

    // samples = 16;
    // samples = 8;
    // samples = 8;
    // samples = 256;
    // samples = 16;
    samples = 256;

    Independent::Params samp_params(samples, 0x12345);

    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void OneWayDice::initialize_lighting(int frame)
{
    PointEmitter::Params emitter_params(Color3f(50.f*30.f*1.5, 60.f*30.f*1.5, 37.f*200.f*1.5),
                                        Vector3f(10.0, 4.0, 1.0));

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

    FeignRenderer::fr_scale(0.5f, 0.5f, 0.5f);
    FeignRenderer::fr_rotate(double(frame) / 2.f, 0.f, 1.f, 0.f);

    MeshEmitter::Params mesh_emitter_params(Color3f(0.5f, 0.5f, 0.5f));
    FeignRenderer::fr_emitter("mesh_emitter",
                            "mesh",
                            &mesh_emitter_params);

    FeignRenderer::fr_object("dice_emitter_obj",
                             "dice_emitter_mesh",
                             "diffuse_dark_shad",
                             "mesh_emitter");

    ObjMesh::Params light_mesh("../scenes/meshes/dice/glass_dice_light_small.obj", "", false);

    FeignRenderer::fr_mesh("dice_emitter_mesh",
                           "triangle_mesh",
                           &light_mesh);

    FeignRenderer::fr_clear_transform();
}

void OneWayDice::flush_render()
{
    FeignRenderer::flush_renders();
    FeignRenderer::clean_up();
}

void OneWayDice::run()
{
    std::string test_name = "one_way_dice";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";
    // std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/aaaaah/out" + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/aaaaah" + ".mp4";

    // system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 0;
    // int start_frame = 501;
    int end_frame = 300;

    // smoke medium
    for (int frame = start_frame; frame < end_frame; frame++)
    {
        initialize_base_structs(test_name, frame);

        initialize_camera(frame);

        initialize_materials(frame);

        initialize_grid(frame);

        initialize_scene(frame);

        initialize_lighting(frame);

        LOG("rendering frame: " + std::to_string(frame));

        flush_render();
    }

    // system(publish_command.c_str());
}

FEIGN_END()
