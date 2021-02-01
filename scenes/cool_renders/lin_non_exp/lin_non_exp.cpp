#include "lin_non_exp.h"
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>
#include <imedit/im_util.h>

FEIGN_BEGIN()

LinNonExpScene::LinNonExpScene()
{
    run();
}

void LinNonExpScene::initialize_materials(int frame)
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

void LinNonExpScene::initialize_grid_scene(int frame)
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

// initialize homogeneous sphere medium where we vary scatter vs. absorption
void LinNonExpScene::initialize_homo_sphere_medium_vary_scatter(int frame)
{
    Transform identity = Transform();

    Float dense = 1.f;
    Color3f scat = Color3f(0.12f);
    Color3f abs = Color3f(0.12f);

    StandardMedium::Params media_params("ray",
                                        "default",
                                        "ray_sampler",
                                        "nonexp_density",
                                        "trans_func",
                                        identity,
                                        abs,
                                        scat);

    FeignRenderer::fr_media("nonexp_medium",
                            "standard",
                            &media_params);

    ConstantDensity::Params mandlebrot_density_params(dense);

    FeignRenderer::fr_medium_density("nonexp_density",
                                     "constant",
                                     &mandlebrot_density_params);

    Trans_RayMarching::Params ray_params = Trans_RayMarching::Params(0.5);
    FeignRenderer::fr_medium_transmittance("ray",
                                           "ray",
                                           &ray_params);

    // Float maxT = 6.1f - Float(frame+1) / 100.f;
    Float maxT = 10.1f - Float(frame+1) / 10.f;
    LinearTrans::Params nonexp_params(maxT);
    FeignRenderer::fr_medium_transmittance_func("trans_func",
                                                "lin",
                                                &nonexp_params);

    Ray_Marching_Samp::Params raysamp_params = Ray_Marching_Samp::Params(0.5);
    FeignRenderer::fr_medium_sampling("ray_sampler",
                                      "ray",
                                      &raysamp_params);

    FeignRenderer::fr_clear_transform();

    FeignRenderer::fr_scale(19.0, 19.0, 19.0);

    FeignRenderer::fr_object("medium_bounds",
                             "box_mesh",
                             "default",
                             "null",
                             "box_medium");

    ObjMesh::Params box_mesh("../scenes/meshes/sphere_tri.obj",
                             "",
                             false,
                             "nonexp_medium",
                             "null",
                             true);

    FeignRenderer::fr_mesh("box_mesh",
                           "triangle_mesh",
                           &box_mesh);

    FeignRenderer::fr_clear_transform();
}

void LinNonExpScene::initialize_camera(int frame)
{
    // Float xz_dist = 9.8;
    Float xz_dist = 20.0;
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

void LinNonExpScene::initialize_base_structs(std::string test_name,
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

    int path_depth = 8;

    Integrator::Params int_params(512,
                                  512,
                                  test_name + "/",
                                  path_depth);

    FeignRenderer::fr_integrator("integrator",
                                 "volpath_nonexp",
                                 "default",
                                 &int_params);

    int samples = 8;
    if (frame < 180) samples = 2048;
    else samples = 2048 * 2;

    samples = 256;
    // samples = 4096;

    Independent::Params samp_params(samples, 0x12345);

    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void LinNonExpScene::initialize_lighting(int frame)
{
    PointEmitter::Params emitter_params(Color3f(50.f*50.f, 60.f*50.f, 37.f*100.f),
                                        Vector3f(-4.0, 4.0, -4.0));

    FeignRenderer::fr_emitter("point_emitter",
                              "point",
                              &emitter_params);
}

void LinNonExpScene::flush_render()
{
    FeignRenderer::flush_renders();
    FeignRenderer::clean_up();
}

void LinNonExpScene::run()
{
    std::string test_name = "lin_non_exp";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";
    // std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/aaaaah/out" + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/aaaaah" + ".mp4";

    // system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 0;
    int end_frame = 100;

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
