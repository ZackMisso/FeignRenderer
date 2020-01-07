#include <feign/core/api.h>

// January 1, 2020
// This render displays the cosine term of the rendering equation as a nice
// heatmap. This shader is used as a means to test my api to verify that it
// works.

static void jan_1_2020()
{
    std::string test_name = "jan_1_2020";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -s 1920x1080 -i " + test_name + "/" + test_name + "_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + test_name + ".mp4";

    // system(rm_command.c_str());
    // system(mkdir_command.c_str());

    std::vector<imedit::Pixel> color_map = std::vector<imedit::Pixel>();
    imedit::color_map_inferno(color_map);

    int start_frame = 720;
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

        Integrator::Params int_params(1000, 1000, test_name + "/");

        FeignRenderer::fr_integrator("integrator",
                                     "cosine_term",
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
                               "../scenes/meshes/ajax.obj");

        FeignRenderer::flush_renders();

        FeignRenderer::clean_up();
    }

    for (int i = start_frame; i < end_frame; ++i)
    {
        char str[5];
        snprintf(str, 5, "%04d", i);
        std::string name = test_name + "_" + std::string(str);

        std::string filename = test_name + "/" + name + ".exr";
        Imagef image = Imagef(filename);

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                float proxy = image(j, i, 0);

                if (proxy == -2.f)
                {
                    image(j, i, 0) = 0.f;
                    image(j, i, 1) = 0.f;
                    image(j, i, 2) = 0.f;
                }
                else
                {
                    proxy += 1.f;
                    if (proxy < 0.f) proxy = 0.f;
                    if (proxy > 1.f) proxy = 1.f;

                    imedit::Pixel pixel = color_from_proxy(color_map, 1.f - proxy);

                    image(j, i, 0) = pixel.r;
                    image(j, i, 1) = pixel.g;
                    image(j, i, 2) = pixel.b;
                }
            }
        }

        image.write(test_name + "/" + name + ".png");
    }

    system(publish_command.c_str());
}
