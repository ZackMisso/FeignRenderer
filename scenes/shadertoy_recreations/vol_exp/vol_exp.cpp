#include "vol_exp.h"
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>

FEIGN_BEGIN()

VolExpShaderToy::VolExpShaderToy()
{
    run();
}

void VolExpShaderToy::initialize_media(int frame)
{
    // TODO: debug why this is breaking
    
    Transform identity = Transform();
    StandardMedium::Params media_params("ratio",
                                        "default",
                                        "delta_sampler",
                                        "sphere_density",
                                        identity,
                                        Color3f(1.f),
                                        Color3f(1.0f));

    FeignRenderer::fr_media("vol_explosion",
                            "standard",
                            &media_params);

    SphereDensity::Params medium_density_params(1.f, 1.f);

    FeignRenderer::fr_medium_density("sphere_density",
                                     "sphere",
                                     &medium_density_params);

    FeignRenderer::fr_medium_sampling("delta_sampler",
                                      "delta",
                                      nullptr);

    FeignRenderer::fr_medium_transmittance("ratio",
                                           "ratio",
                                           nullptr);

    FeignRenderer::fr_object("medium_bounds",
                             "box_mesh",
                             "default",
                             "null",
                             "vol_explosion");

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

void VolExpShaderToy::initialize_camera(int frame)
{
    Float xz_dist = std::sqrt(7.0 * 7.0 - 16.0);
    Float angle = 1.0 * M_PI * float(frame) / 480.f;
    Vector3f look_at = Vector3f(0.f, 0.f, 0.f);
    Float height = 4.0;
    Float fov = 30.f;

    Perspective::Params cam_params(Vector3f(0.0,
                                            0.0,
                                            -4.0),
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

void VolExpShaderToy::initialize_base_structs(std::string test_name,
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
    else samples = 2048 * 2;

    // samples = 16;
    samples = 128;

    Independent::Params samp_params(samples, 0x12345);

    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void VolExpShaderToy::initialize_lighting(int frame)
{
    PointEmitter::Params emitter_params(Color3f(50.f, 50.f, 37.f),
                                        Vector3f(0.0, 3.0, 0.0));

    FeignRenderer::fr_emitter("point_emitter",
                              "point",
                              &emitter_params);
}

void VolExpShaderToy::flush_render()
{
    FeignRenderer::flush_renders();
    FeignRenderer::clean_up();
}

void VolExpShaderToy::run()
{
    std::string test_name = "vol_exp";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";
    // std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/aaaaah/out" + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/aaaaah" + ".mp4";

    // system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 0;
    // int start_frame = 501;
    int end_frame = 1;

    // smoke medium
    for (int frame = start_frame; frame < end_frame; frame++)
    {
        initialize_base_structs(test_name, frame);

        initialize_camera(frame);

        initialize_lighting(frame);

        initialize_media(frame);

        flush_render();
    }
}

FEIGN_END()
