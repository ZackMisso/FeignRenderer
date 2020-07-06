#include <feign/core/api.h>

// January 3, 2020
// This render tests barycentric coordinate shading

FEIGN_BEGIN()

static void jan_3_2020()
{
    std::string test_name = "jan_3_2020";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 200;
    int end_frame = 201;

    for (int i = start_frame; i < end_frame; i++)
    {
        LOG("starting frame: " + std::to_string(i));
        float degree = (M_PI * float(i + 180) / 180.f) / 2.f;
        Vector3f origin = Vector3f(5.0 * cos(degree), 0.0, 5.0 * sin(degree));

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
                                 "wireframe_mat_shad");

        ObjMesh::Params sphere_params("../scenes/meshes/cube_sphere_tri.obj", "");

        FeignRenderer::fr_mesh("sphere_obj",
                               "triangle_mesh",
                               &sphere_params);

        Float proxy = 0.34f;

        if (i >= 100)
        {
            proxy = (1.f - interp_value(i - 100, 310)) * 0.34 + 0.01;
        }

        WireframeMaterialShader::Params shad_params("wireframe_mat", "mesh_mat", proxy);

        FeignRenderer::fr_shader("wireframe_mat_shad",
                                 "wireframe",
                                 &shad_params);

        SimpleMaterial::Params wireframe_mat_params("wireframe_bsdf");
        SimpleMaterial::Params mesh_mat_params("mesh_bsdf");

        FeignRenderer::fr_material("wireframe_mat",
                                   "simple",
                                   &wireframe_mat_params);

        FeignRenderer::fr_material("mesh_mat",
                                  "simple",
                                  &mesh_mat_params);

        Diffuse::Params wireframe_bsdf_params(Color3f(0.9f, 0.1f, 0.1f));

        FeignRenderer::fr_bsdf("wireframe_bsdf",
                               "diffuse",
                               &wireframe_bsdf_params);

        Diffuse::Params mesh_bsdf_params(Color3f(0.02f, 0.02f, 0.02f));

        FeignRenderer::fr_bsdf("mesh_bsdf",
                               "diffuse",
                               &mesh_bsdf_params);

        PointEmitter::Params emitter_params(Color3f(40.f, 40.f, 40.f),
                                            origin + Vector3f(0.0, 1.0, 0.0));

        FeignRenderer::fr_emitter("point_emitter",
                                  "point",
                                  &emitter_params);

        FeignRenderer::flush_renders();

        FeignRenderer::clean_up();
    }

    // system(publish_command.c_str());
}

FEIGN_END()
