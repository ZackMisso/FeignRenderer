#include <feign/core/api.h>

// January 4, 2020
// This render tests terrain maps

static void jan_4_2020()
{
    std::string test_name = "jan_4_2020";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    // system(rm_command.c_str());
    // system(mkdir_command.c_str());

    int start_frame = 630;
    int end_frame = 720;

    for (int i = start_frame; i < end_frame; i++)
    {
        LOG("starting frame: " + std::to_string(i));
        float degree = (M_PI * float(0 + 180) / 180.f) / 2.f;
        Vector3f origin = Vector3f(2.5 * cos(degree), 2.5, 2.5 * sin(degree));

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
                                     "whitted",
                                     "default",
                                     &int_params);

        Independent::Params samp_params(64, 0x12345);

        FeignRenderer::fr_sampler("sampler",
                                  "independent",
                                  &samp_params);

        Perspective::Params cam_params(origin,
                                       Vector3f(0.0, 0.0, -0.8),
                                       Vector3f(0, 1, 0),
                                       30.f,
                                       1e-4f,
                                       1e4f,
                                       10.f,
                                       0.f,
                                       Vec2i(1920, 1080));

        FeignRenderer::fr_camera("camera",
                                 "perspective",
                                 &cam_params);

        FeignRenderer::fr_object("plane",
                                 "plane_obj",
                                 "simple_mat");

        GridObj::Params mesh_params("disp_texture", "", Vec2i(1024, 1024));

        FeignRenderer::fr_mesh("plane_obj",
                               "grid",
                               &mesh_params);

        Float proxy = interp_value(i, 360);

        ImageTexture::Params im_params("../scenes/textures/final_noise.png",
                                       Vec3f(proxy));

        FeignRenderer::fr_texture("disp_texture",
                                  "image",
                                  &im_params);

        SimpleMaterial::Params simple_mat_params("wireframe_bsdf");

        FeignRenderer::fr_material("simple_mat",
                                   "simple",
                                   &simple_mat_params);

        Diffuse::Params wireframe_bsdf_params(Color3f(0.8f, 0.8f, 0.8f));

        FeignRenderer::fr_bsdf("wireframe_bsdf",
                               "diffuse",
                               &wireframe_bsdf_params);

        for (int j = 0; j < 20; ++j)
        {
            float degree = 2.0 * M_PI * float(j) / 20.f;
            Vector3f origin = Vector3f(2.0 * cos(degree), 4.0, 2.0 * sin(degree));

            PointEmitter::Params emitter_params(Color3f(10.f, 10.f, 10.f),
                                                origin);

            FeignRenderer::fr_emitter("point_emitter_" + std::to_string(j),
                                      "point",
                                      "null",
                                      "null",
                                      &emitter_params);
        }

        FeignRenderer::flush_renders();

        FeignRenderer::clean_up();
    }

    // system(publish_command.c_str());
}
