#include <feign/core/api.h>

// January 6, 2020
// This render tests sdf spheres

FEIGN_BEGIN()

static void jan_6_2020()
{
    std::string test_name = "jan_6_2020";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    // system(rm_command.c_str());
    // system(mkdir_command.c_str());

    int start_frame = 0;
    int end_frame = 0;

    // // setting the global parameters required for this render //
    // global_params.sdf_only = true;
    // ////////////////////////////////////////////////////////////
    // if (global_params.sdf_only)
    // {
    //     LOG("should work");
    // }

    for (int i = start_frame; i < end_frame; i++)
    {
        pcg32 rng(0xadf2, 0x2312);
        LOG("starting frame: " + std::to_string(i));
        float degree = (M_PI * float(i + 180) / 180.f) / 2.f;
        Vector3f origin = Vector3f(5.0 * cos(degree), 0.0, 5.0 * sin(degree));

        char str[5];
        snprintf(str, 5, "%04d", i);
        std::string name = test_name + "_" + std::string(str);

        FeignRenderer::initialize();

        // if (global_params.sdf_only)
        // {
        //     LOG("should work2");
        // }

        FeignRenderer::fr_scene(name,
                                "integrator",
                                "sampler",
                                "camera",
                                "");

        Integrator::Params int_params(1000,
                                      1000,
                                      test_name + "/");

        FeignRenderer::fr_integrator("integrator",
                                     "nice_normal",
                                     "default",
                                     &int_params);

        Independent::Params samp_params(16, 0x12345);

        FeignRenderer::fr_sampler("sampler",
                                  "independent",
                                  &samp_params);

        Perspective::Params cam_params(origin,
                                       Vector3f(0.0, 0.0, 0.0),
                                       Vector3f(0, 1, 0),
                                       50.f,
                                       1e-4f,
                                       1e4f,
                                       10.f,
                                       0.f,
                                       Vec2i(1920, 1080));

        FeignRenderer::fr_camera("camera",
                                 "perspective",
                                 &cam_params);

        FeignRenderer::fr_object("sphere",
                                 "sphere_obj",
                                 "");

        SDFSphere::Params sphere_params(Point3f(0.f, 0.f, 0.f),
                                        0.5f,
                                        0.f);

        FeignRenderer::fr_mesh("sphere_obj",
                               "sdf_sphere",
                               &sphere_params);

        FeignRenderer::flush_renders();

        // if (global_params.sdf_only)
        // {
        //     LOG("should work 7");
        // }

        FeignRenderer::clean_up();
    }

    system(publish_command.c_str());
}

FEIGN_END()
