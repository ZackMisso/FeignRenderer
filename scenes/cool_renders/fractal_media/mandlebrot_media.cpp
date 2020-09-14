#include "mandlebrot_media.h"
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>
#include <imedit/im_util.h>

FEIGN_BEGIN()

MandlebrotMediaTests::MandlebrotMediaTests()
{
    run();
}

void MandlebrotMediaTests::initialize_materials(int frame)
{
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

void MandlebrotMediaTests::initialize_grid_scene(int frame)
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
    // FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    // FeignRenderer::fr_rotate(-90.f, 0.f, 0.f, 1.f);
    // FeignRenderer::fr_translate(10.f, 0.f, 0.f);
    //
    // FeignRenderer::fr_object("right_wall",
    //                          "right_wall_grid",
    //                          "diffuse_shad");
    //
    // GridObj::Params params_right_wall("", "diffuse_shad", Vec2i(grid_dim, grid_dim));
    // // ObjMesh::Params params_right_wall("../scenes/meshes/plane.obj", "");
    //
    // FeignRenderer::fr_mesh("right_wall_grid",
    //                        "grid",
    //                        &params_right_wall);
    // FeignRenderer::fr_clear_transform();
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

// void MandlebrotMediaTests::initialize_sphere_medium(int frame)
// {
//     Transform identity = Transform();
//     StandardMedium::Params media_params("default",
//                                         "default",
//                                         "default",
//                                         "sphere_density",
//                                         identity,
//                                         Color3f(1.f),
//                                         COLOR_BLACK);
//
//     FeignRenderer::fr_media("env_medium",
//                             "standard",
//                             &media_params);
//
//     SphereDensity::Params medium_density_params(1.f, 1.f);
//
//     FeignRenderer::fr_medium_density("sphere_density",
//                                      "sphere",
//                                      &medium_density_params);
// }

// initialize homogeneous sphere medium where we vary scatter vs. absorption
void MandlebrotMediaTests::initialize_homo_sphere_medium_vary_scatter(int frame)
{
    Transform identity = Transform();

    Float dense = 1.f;
    Color3f scat = Color3f(0.0f);
    Color3f abs = Color3f(0.1f);

    // if (frame < 30)
    // {
    //     dense = 0.f;
    // }
    // else if (frame < 150)
    // {
    //     dense = 2.f * float(frame-30) / 120.f;
    // }
    // else if (frame >= 150 && frame < 360)
    // {
    //     dense = 2.f;
    // }
    // else if (frame < 480)
    // {
    //     dense = 2.f - 2.f * float(frame-360) / 120.f;
    // }
    // else
    // {
    //     dense = 0.f;
    // }
    //
    // if (frame < 195)
    // {
    //     scat = Color3f(0.f);
    //     abs = Color3f(1.f);
    // }
    // else if (frame >= 195 && frame < 315)
    // {
    //     Float val = 1.f * float(frame-195) / 120.f;
    //
    //     scat = Color3f(val);
    //     abs = Color3f(1.f - val);
    // }
    // else if (frame < 500)
    // {
    //     // assert(false);
    //     scat = Color3f(0.999f);
    //     abs = Color3f(0.001f);
    // }

    LOG("frame: " + std::to_string(frame));
    LOG("dense: " + std::to_string(dense));
    LOG("abs: " + std::to_string(abs(0)) + " " + std::to_string(abs(1)) + " " + std::to_string(abs(2)));
    LOG("scat: " + std::to_string(scat(0)) + " " + std::to_string(scat(1)) + " " + std::to_string(scat(2)));
    LOG("");

    // else if (frame < 480)
    // {
    //     dense = 2.f - 2.f * float(frame-360) / 120.f;
    // }
    // else if (frame < 500)
    // {
    //     dense = 0.f;
    // }


    StandardMedium::Params media_params("ratio",
                                        "default",
                                        "delta_sampler",
                                        "mandlebrot_density",
                                        identity,
                                        abs,
                                        scat);

    FeignRenderer::fr_media("mandlebrot_medium",
                            "standard",
                            &media_params);

    MandlebrotDensity::Params mandlebrot_density_params(dense);

    FeignRenderer::fr_medium_density("mandlebrot_density",
                                     "mandlebrot",
                                     &mandlebrot_density_params);

    Trans_RatioTracking::Params ratio_params = Trans_RatioTracking::Params(2.0);
    FeignRenderer::fr_medium_transmittance("ratio",
                                           "ratio",
                                           &ratio_params);

    FeignRenderer::fr_medium_sampling("delta_sampler",
                                      "delta",
                                      nullptr);

    FeignRenderer::fr_clear_transform();

    FeignRenderer::fr_scale(9.0, 9.0, 9.0);

    FeignRenderer::fr_object("medium_bounds",
                             "box_mesh",
                             "default",
                             "null",
                             "box_medium");

    ObjMesh::Params box_mesh("../scenes/meshes/sphere_tri.obj",
                             "",
                             false,
                             "mandlebrot_medium",
                             "null",
                             true);

    FeignRenderer::fr_mesh("box_mesh",
                           "triangle_mesh",
                           &box_mesh);

    FeignRenderer::fr_clear_transform();
}

void MandlebrotMediaTests::initialize_camera(int frame)
{
    Float xz_dist = 9.8;
    Float angle = 2.f * M_PI;
    // Vector3f look_at = Vector3f(0.f, -7.4f, 0.f);
    // Float height = -9.0;
    Vector3f look_at = Vector3f(0.f, 0.f, 0.f);
    Float height = 0.0;
    Float fov = 90.f;

    Perspective::Params cam_params(Vector3f(xz_dist * cos(angle),
                                            height,
                                            xz_dist * sin(angle)),
                                   look_at,
                                   Vector3f(0, 1, 0),
                                   fov, // full res
                                   1e-4f,
                                   1e2f,
                                   10.f,
                                   0.f,
                                   Vec2i(256, 256)); // debug res
                                   // Vec2i(1080, 1080)); // full res

    FeignRenderer::fr_camera("camera",
                             "perspective",
                             &cam_params);
}

void MandlebrotMediaTests::initialize_base_structs(std::string test_name,
                                                   int frame)
{
    char str[5];
    snprintf(str, 5, "%04d", frame);
    std::string name = test_name + "_" + std::string(str);

    FeignRenderer::initialize();

    // TODO: create scene bounds for infinite media
    FeignRenderer::fr_scene(name,
                            "integrator",
                            "sampler",
                            "camera",
                            "null",
                            false);

    int path_depth = 4;

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
    samples = 128;

    Independent::Params samp_params(samples, 0x12345);

    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void MandlebrotMediaTests::initialize_lighting(int frame)
{
    PointEmitter::Params emitter_params(Color3f(50.f*50.f, 60.f*50.f, 37.f*100.f),
                                        Vector3f(4.0, 9.0, 4.0));

    FeignRenderer::fr_emitter("point_emitter",
                              "point",
                              &emitter_params);
}

void MandlebrotMediaTests::flush_render()
{
    FeignRenderer::flush_renders();
    FeignRenderer::clean_up();
}

void MandlebrotMediaTests::run()
{
    std::string test_name = "mandlebrot_media";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";
    // std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/aaaaah/out" + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/aaaaah" + ".mp4";

    // system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 2;
    int end_frame = 3;

    // smoke medium
    for (int frame = start_frame; frame < end_frame; frame++)
    {
        initialize_base_structs(test_name, frame);

        initialize_camera(frame);

        initialize_materials(frame);

        initialize_grid_scene(frame);

        initialize_homo_sphere_medium_vary_scatter(frame);

        // initialize_initial_scene(frame-500);

        // initialize_colored_scene(frame);

        initialize_lighting(frame);

        LOG("rendering frame: " + std::to_string(frame));

        flush_render();
    }

    system(publish_command.c_str());
}

FEIGN_END()
