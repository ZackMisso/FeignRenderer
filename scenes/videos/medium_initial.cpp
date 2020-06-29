#include "medium_initial.h"
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>

MediumTesting_Debug::MediumTesting_Debug()
{
    run();
}

void MediumTesting_Debug::initialize_materials(int frame)
{
    // we want 3 different materials:
    // a dark diffuse material,
    // a mirror material,
    // and a light material

    // SimpleMaterialShader::Params diffuse_params("diffuse_mat");

    // SimpleMaterial::Params diffuse_mat_params("diffuse_bsdf");
    //
    // FeignRenderer::fr_shader("diffuse_shad",
    //                          "simple_material",
    //                          &diffuse_params);
    //
    // FeignRenderer::fr_material("diffuse_mat",
    //                            "simple",
    //                            &diffuse_mat_params);
    //
    // Diffuse::Params diffuse_bsdf(Color3f(0.6f, 0.4f, 0.3f));
    //
    // FeignRenderer::fr_bsdf("diffuse_bsdf",
    //                        "diffuse",
    //                        &diffuse_bsdf);

    WireframeMaterialShader::Params wireframe_params("diffuse_dark_mat",
                                                     "diffuse_light_mat",
                                                     0.01);

    FeignRenderer::fr_shader("diffuse_shad",
                             "wireframe",
                             &wireframe_params);

    SimpleMaterial::Params diffuse_light_mat_params("diffuse_light_bsdf");
    SimpleMaterial::Params diffuse_dark_mat_params("diffuse_dark_bsdf");

    Diffuse::Params diffuse_light(Color3f(0.6f, 0.4f, 0.3f));

    FeignRenderer::fr_material("diffuse_light_mat",
                               "simple",
                               &diffuse_light_mat_params);

    FeignRenderer::fr_material("diffuse_dark_mat",
                               "simple",
                               &diffuse_dark_mat_params);

    FeignRenderer::fr_bsdf("diffuse_light_bsdf",
                           "diffuse",
                           &diffuse_light);

    Diffuse::Params diffuse_dark(Color3f(0.1f, 0.2f, 0.3f));

    FeignRenderer::fr_bsdf("diffuse_dark_bsdf",
                          "diffuse",
                          &diffuse_dark);
}

void MediumTesting_Debug::initialize_grid_scene(int frame)
{
    int grid_dim = 10;
    // LOG("floor");
    // floor //
    FeignRenderer::fr_clear_transform();
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_translate(0.f, -10.f, 0.f);

    FeignRenderer::fr_object("floor",
                             "floor_grid",
                             "diffuse_shad");

    GridObj::Params params_floor("", "diffuse_shad", Vec2i(grid_dim, grid_dim));

    FeignRenderer::fr_mesh("floor_grid",
                           "grid",
                           &params_floor);
    FeignRenderer::fr_clear_transform();
    ////////////////

    // LOG("front");
    // front wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(-90.f, 1.f, 0.f, 0.f);
    FeignRenderer::fr_translate(0.f, 0.f, -10.f);

    FeignRenderer::fr_object("front_wall",
                             "front_wall_grid",
                             "diffuse_shad");

    GridObj::Params params_front_wall("", "diffuse_shad", Vec2i(grid_dim, grid_dim));
    // ObjMesh::Params params_front_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("front_wall_grid",
                           "grid",
                           &params_front_wall);
    FeignRenderer::fr_clear_transform();
    //////////////

    // LOG("back");
    // back wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(90.f, 1.f, 0.f, 0.f);
    FeignRenderer::fr_translate(0.f, 0.f, 10.f);

    FeignRenderer::fr_object("back_wall",
                             "back_wall_grid",
                             "diffuse_shad");

    GridObj::Params params_back_wall("", "diffuse_shad", Vec2i(grid_dim, grid_dim));
    // ObjMesh::Params params_back_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("back_wall_grid",
                           "grid",
                           &params_back_wall);
    FeignRenderer::fr_clear_transform();
    //////////////

    // LOG("left");
    // left wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(90.f, 0.f, 0.f, 1.f);
    FeignRenderer::fr_translate(-10.f, 0.f, 0.f);

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

    // LOG("right");
    // right wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(-90.f, 0.f, 0.f, 1.f);
    FeignRenderer::fr_translate(10.f, 0.f, 0.f);

    FeignRenderer::fr_object("right_wall",
                             "right_wall_grid",
                             "diffuse_shad");

    GridObj::Params params_right_wall("", "diffuse_shad", Vec2i(grid_dim, grid_dim));
    // ObjMesh::Params params_right_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("right_wall_grid",
                           "grid",
                           &params_right_wall);
    FeignRenderer::fr_clear_transform();
    ////////////////

    // LOG("ceiling");
    // ceiling wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(180.f, 0.f, 0.f, 1.f);
    FeignRenderer::fr_translate(0.f, 10.f, 0.f);

    FeignRenderer::fr_object("ceil_wall",
                             "ceil_wall_grid",
                             "diffuse_shad");

    GridObj::Params params_ceil_wall("", "diffuse_shad", Vec2i(grid_dim, grid_dim));
    // ObjMesh::Params params_ceil_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("ceil_wall_grid",
                           "grid",
                           &params_ceil_wall);
    FeignRenderer::fr_clear_transform();

}

void MediumTesting_Debug::initialize_scene(int frame)
{
    // floor //
    FeignRenderer::fr_clear_transform();
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_translate(0.f, 10.f, -10.f);

    FeignRenderer::fr_object("floor",
                             "floor_plane",
                             "diffuse_shad");

    ObjMesh::Params params_floor("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("floor_plane",
                           "triangle_mesh",
                           &params_floor);
    FeignRenderer::fr_clear_transform();
    ////////////////

    // front wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(-90.f, 1.f, 0.f, 0.f);
    FeignRenderer::fr_translate(0.f, 0.f, -10.f);

    FeignRenderer::fr_object("front_wall",
                             "front_wall_plane",
                             "diffuse_shad");

    ObjMesh::Params params_front_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("front_wall_plane",
                           "triangle_mesh",
                           &params_front_wall);
    FeignRenderer::fr_clear_transform();
    //////////////

    // back wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(90.f, 1.f, 0.f, 0.f);
    FeignRenderer::fr_translate(0.f, 0.f, 10.f);

    FeignRenderer::fr_object("back_wall",
                             "back_wall_plane",
                             "diffuse_shad");

    ObjMesh::Params params_back_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("back_wall_plane",
                           "triangle_mesh",
                           &params_back_wall);
    FeignRenderer::fr_clear_transform();
    //////////////

    // left wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(90.f, 0.f, 0.f, 1.f);
    FeignRenderer::fr_translate(-10.f, 0.f, 0.f);

    FeignRenderer::fr_object("left_wall",
                             "left_wall_plane",
                             "diffuse_shad");

    ObjMesh::Params params_left_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("left_wall_plane",
                           "triangle_mesh",
                           &params_left_wall);
    FeignRenderer::fr_clear_transform();
    ////////////////

    // right wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(-90.f, 0.f, 0.f, 1.f);
    FeignRenderer::fr_translate(10.f, 0.f, 0.f);

    FeignRenderer::fr_object("right_wall",
                             "right_wall_plane",
                             "diffuse_shad");

    ObjMesh::Params params_right_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("right_wall_plane",
                           "triangle_mesh",
                           &params_right_wall);
    FeignRenderer::fr_clear_transform();
    ////////////////

    // ceiling wall //
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    FeignRenderer::fr_rotate(180.f, 0.f, 0.f, 1.f);
    FeignRenderer::fr_translate(0.f, 10.f, 0.f);

    FeignRenderer::fr_object("ceil_wall",
                             "ceil_wall_plane",
                             "diffuse_shad");

    ObjMesh::Params params_ceil_wall("../scenes/meshes/plane.obj", "");

    FeignRenderer::fr_mesh("ceil_wall_plane",
                           "triangle_mesh",
                           &params_ceil_wall);
    FeignRenderer::fr_clear_transform();
    ////////////////
    //
    // // back wall //
    // FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    // FeignRenderer::fr_rotate(-90.f, 0.f, 0.f, 1.f);
    // FeignRenderer::fr_translate(10.f, 0.f, 0.f);
    //
    // FeignRenderer::fr_object("right_wall",
    //                          "right_wall_plane",
    //                          "diffuse_shad");
    //
    // ObjMesh::Params params_right_wall("../scenes/meshes/plane.obj", "");
    //
    // FeignRenderer::fr_mesh("right_wall_plane",
    //                        "triangle_mesh",
    //                        &params_right_wall);
    // FeignRenderer::fr_clear_transform();
    ////////////////
}

void MediumTesting_Debug::initialize_sphere_medium(int frame)
{
    Transform identity = Transform();
    StandardMedium::Params media_params("default",
                                        "default",
                                        "default",
                                        "sphere_density",
                                        identity,
                                        Color3f(1.f),
                                        COLOR_BLACK);

    FeignRenderer::fr_media("env_medium",
                            "standard",
                            &media_params);

    SphereDensity::Params medium_density_params(1.f, 1.f);

    FeignRenderer::fr_medium_density("sphere_density",
                                     "sphere",
                                     &medium_density_params);
}

void MediumTesting_Debug::initialize_box_medium(int frame)
{
    Transform identity = Transform();

    StandardMedium::Params media_params("default",
                                        "default",
                                        "default",
                                        "const_density",
                                        identity,
                                        Color3f(1.0f),
                                        Color3f(0.0f));

    FeignRenderer::fr_media("box_medium",
                            "standard",
                            &media_params);

    ConstantDensity::Params constant_density_params(2.0f);

    FeignRenderer::fr_medium_density("const_density",
                                     "constant",
                                     &constant_density_params);

    FeignRenderer::fr_medium_sampling("const_sampler",
                                      "constant",
                                      nullptr);

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
}

void MediumTesting_Debug::initialize_initial_scene(int frame)
{

}

// initialize homogeneous sphere medium where we vary scatter vs. absorption
void MediumTesting_Debug::initialize_homo_sphere_medium_vary_scatter(int frame)
{
    Transform identity = Transform();

    Float dense = 0.f;
    Color3f scat = Color3f(0.f);
    Color3f abs = Color3f(0.f);

    if (frame < 30)
    {
        dense = 0.f;
    }
    else if (frame < 150)
    {
        dense = 2.f * float(frame-30) / 120.f;
    }
    else if (frame >= 150 && frame < 360)
    {
        dense = 2.f;
    }
    else if (frame < 480)
    {
        dense = 2.f - 2.f * float(frame-360) / 120.f;
    }
    else
    {
        dense = 0.f;
    }

    if (frame < 195)
    {
        scat = Color3f(0.f);
        abs = Color3f(1.f);
    }
    else if (frame >= 195 && frame < 315)
    {
        Float val = 1.f * float(frame-195) / 120.f;

        scat = Color3f(val);
        abs = Color3f(1.f - val);
    }
    else if (frame < 500)
    {
        // assert(false);
        scat = Color3f(0.999f);
        abs = Color3f(0.001f);
    }
    // else if (frame < 480)
    // {
    //     dense = 2.f - 2.f * float(frame-360) / 120.f;
    // }
    // else if (frame < 500)
    // {
    //     dense = 0.f;
    // }


    StandardMedium::Params media_params("default",
                                        "default",
                                        "default",
                                        "const_density",
                                        identity,
                                        abs,
                                        scat);

    FeignRenderer::fr_media("box_medium",
                            "standard",
                            &media_params);

    ConstantDensity::Params constant_density_params(dense);

    FeignRenderer::fr_medium_density("const_density",
                                     "constant",
                                     &constant_density_params);

    FeignRenderer::fr_medium_sampling("const_sampler",
                                      "constant",
                                      nullptr);

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
}

void MediumTesting_Debug::initialize_homo_cornell_scene_vary_phase(int frame)
{

}

void MediumTesting_Debug::initialize_hetero_smoke_vary_scatter(int frame)
{

}

void MediumTesting_Debug::initialize_fully_absorbing_global_media(int frame)
{
    // TODO
}

void MediumTesting_Debug::initialize_bounded_fully_absorbing_media(int frame)
{
    // TODO
}

void MediumTesting_Debug::initialize_absorb_scat_global_media(int frame)
{
    // TODO
}

void MediumTesting_Debug::initialize_absorb_scat_bounded_media(int frame)
{
    // TODO
}

void MediumTesting_Debug::initialize_varying_absorb_scat_global_media(int frame)
{
    // TODO
}

void MediumTesting_Debug::initialize_varying_absorb_scat_bounded_media(int frame)
{
    // TODO
}

void MediumTesting_Debug::initialize_final_media(int frame)
{
    // TODO
}

void MediumTesting_Debug::initialize_camera(int frame)
{
    Float xz_dist = std::sqrt(7.0 * 7.0 - 16.0);
    Float angle = 1.0 * M_PI * float(frame) / 480.f;
    Perspective::Params cam_params(Vector3f(xz_dist * cos(angle),
                                            4.0,
                                            xz_dist * sin(angle)),
                                   Vector3f(0.f,
                                            0.f,
                                            0.f),
                                   Vector3f(0, 1, 0),
                                   // 30.f, // debug res
                                   30.f, // full res
                                   1e-4f,
                                   1e2f,
                                   10.f,
                                   0.f,
                                   // Vec2i(256, 256)); // debug res
                                   Vec2i(1080, 1080)); // full res

    FeignRenderer::fr_camera("camera",
                             "perspective",
                             &cam_params);
}

void MediumTesting_Debug::initialize_base_structs(std::string test_name,
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

    if (frame < 180) path_depth = 7;
    else path_depth = 10;

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
    else samples = 2048 * 5;

    Independent::Params samp_params(samples, 0x12345);

    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void MediumTesting_Debug::initialize_lighting(int frame)
{
    PointEmitter::Params emitter_params(Color3f(50.f*50.f, 60.f*50.f, 37.f*100.f),
                                        Vector3f(4.0, 9.0, 4.0));

    FeignRenderer::fr_emitter("point_emitter",
                              "point",
                              &emitter_params);
}

void MediumTesting_Debug::flush_render()
{
    FeignRenderer::flush_renders();
    FeignRenderer::clean_up();
}

void MediumTesting_Debug::run()
{
    std::string test_name = "medium_testing";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    // system(rm_command.c_str());
    // system(mkdir_command.c_str());

    int start_frame = 0;
    int end_frame = 500;

    for (int frame = start_frame; frame < end_frame; frame++)
    {
        initialize_base_structs(test_name, frame);

        initialize_camera(frame);

        initialize_materials(frame);

        // initialize_scene(frame);
        initialize_grid_scene(frame);
        // assert(false);

        initialize_homo_sphere_medium_vary_scatter(frame);
        // initialize_box_medium(frame);

        initialize_lighting(frame);

        LOG("rendering frame: " + std::to_string(frame));

        flush_render();
    }

    system(publish_command. c_str());
}
