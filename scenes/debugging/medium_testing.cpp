#include "medium_testing.h"
#include <feign/shapes/objmesh.h>

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

    SimpleMaterialShader::Params diffuse_params("diffuse_mat");

    SimpleMaterial::Params diffuse_mat_params("diffuse_bsdf");

    FeignRenderer::fr_shader("diffuse_shad",
                             "simple_material",
                             &diffuse_params);

    FeignRenderer::fr_material("diffuse_mat",
                               "simple",
                               &diffuse_mat_params);

    Diffuse::Params diffuse_bsdf(Color3f(0.5f, 0.5f, 0.5f));

    FeignRenderer::fr_bsdf("diffuse_bsdf",
                           "diffuse",
                           &diffuse_bsdf);
}

void MediumTesting_Debug::initialize_scene(int frame)
{
    // floor //
    FeignRenderer::fr_clear_transform();
    FeignRenderer::fr_scale(10.f, 1.f, 10.f);

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
                                        Color3f(0.5f),
                                        Color3f(0.5f));

    FeignRenderer::fr_media("box_medium",
                            "standard",
                            &media_params);

    ConstantDensity::Params constant_density_params(1.0f);

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
                             true);

    FeignRenderer::fr_mesh("box_mesh",
                           "triangle_mesh",
                           &box_mesh);
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
    Float angle = 1.0 * M_PI * float(frame) / 60.f;
    Perspective::Params cam_params(Vector3f(std::abs(7.f * cos(angle)),
                                            0.001f + 7.f * sin(angle),
                                            0.f),
                                   Vector3f(0.f,
                                            0.f,
                                            0.f),
                                   Vector3f(0, 1, 0),
                                   30.f,
                                   1e-4f,
                                   1e2f,
                                   10.f,
                                   0.f,
                                   Vec2i(256, 256));

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
                            "default",
                            false);

    Integrator::Params int_params(512,
                                  512,
                                  test_name + "/");

    FeignRenderer::fr_integrator("integrator",
                                 "volpath",
                                 "default",
                                 &int_params);

    Independent::Params samp_params(512, 0x12345);

    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void MediumTesting_Debug::initialize_lighting(int frame)
{
    PointEmitter::Params emitter_params(Color3f(225.f*25.f, 300.f*25.f, 375.f*25.f),
                                        Vector3f(0.0, 20.0, 0.0));

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
    // std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    // system(rm_command.c_str());
    // system(mkdir_command.c_str());

    int start_frame = 0;
    int end_frame = 1;

    for (int frame = start_frame; frame < end_frame; frame++)
    {
        initialize_base_structs(test_name, frame);

        initialize_camera(frame);

        initialize_materials(frame);

        // initialize_scene(frame);

        initialize_box_medium(frame);

        initialize_lighting(frame);

        LOG("rendering frame: " + std::to_string(frame));

        flush_render();
    }

    // system(publish_command.c_str());
}
