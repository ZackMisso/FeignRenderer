#include <feign/core/api.h>

// January 3, 2020
// This render tests wireframe shading

float interp_value(int index, int flip)
{
    if (index > flip)
    {
        return 1.f - float(index - flip) / float(flip);
    }

    return 1.f - float(flip - index) / float(flip);
}

static void jan_3_2020()
{
    std::string test_name = "jan_3_2020";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -s 1920x1080 -i " + test_name + "/" + test_name + "_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + test_name + ".mp4";

    // system(rm_command.c_str());
    // system(mkdir_command.c_str());

    int start_frame = 630;
    int end_frame = 720;

    for (int i = start_frame; i < end_frame; ++i)
    {
        LOG("starting frame: " + std::to_string(i));
        float degree = (M_PI * float(i + 180) / 180.f) / 2.f;
        Vector3f origin = Vector3f(-1.24528 + 67.0 * cos(degree), 53.0, -17.5872 + 67.0 * sin(degree));

        char str[5];
        snprintf(str, 5, "%04d", i);
        std::string name = test_name + "_" + std::string(str);

        FeignRenderer::initialize();

        FeignRenderer::fr_scene(name,
                                "integrator",
                                "sampler",
                                "camera",
                                "");

        Integrator::Params int_params(1000,
                                      1000,
                                      test_name + "/");

        FeignRenderer::fr_integrator("integrator",
                                     "normal",
                                     "default",
                                     &int_params);

        Independent::Params samp_params(16, 0x12345);

        FeignRenderer::fr_sampler("sampler",
                                  "independent",
                                  &samp_params);

        Perspective::Params cam_params(origin,
                                       Vector3f(-1.24528, 19.2211, -17.5872),
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

        FeignRenderer::fr_object("ajax",
                                 "ajax_obj",
                                 "null");

        FeignRenderer::fr_mesh("ajax_obj",
                               "triangle_mesh",
                               "../scenes/meshes/ajax.obj",
                               "test_shader");

        InterpVertsToSphereShader::Params shad_params(0.6f,
                                                      interp_value(i, 360));

        FeignRenderer::fr_shader("test_shader",
                                 "interp_verts_to_sphere",
                                 &shad_params);

        FeignRenderer::flush_renders();

        FeignRenderer::clean_up();
    }

    system(publish_command.c_str());
}
