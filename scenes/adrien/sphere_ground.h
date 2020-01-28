#include <feign/core/api.h>
#include <feign/shapes/grid_obj.h>

static void sphere_ground()
{
    std::string test_name = "sphere_ground";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 0;
    int end_frame = 1;

    for (int i = start_frame; i < end_frame; i++)
    {
        pcg32 rng(0xadf2, 0x2312);
        LOG("starting frame: " + std::to_string(i));
        float degree = (M_PI * float(0 + 180) / 180.f) / 2.f;
        Vector3f origin = Vector3f(5.0 * cos(degree), 1.0, -40.0 * sin(degree));

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

        Independent::Params samp_params(1, 0x12345);

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
                                       Vec2i(1920/4, 1080/4));

        FeignRenderer::fr_camera("camera",
                                 "perspective",
                                 &cam_params);

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_scale(4.f, 4.f, 4.f);
        FeignRenderer::fr_translate(0.f, 9.0 * interp_value(i, 100) - 5.f, 0.f);

        FeignRenderer::fr_object("sphere",
                                 "sphere_obj",
                                 "diffuse_shad");

        FeignRenderer::fr_clear_transform();
        FeignRenderer::fr_scale(100.f, 100.f, 100.f);

        FeignRenderer::fr_object("plane",
                              "plane_obj",
                              "diffuse_shad");

        GridObj::Params grid_params("displacement",
                                    "",
                                    Vec2i(400, 400));




        FeignRenderer::fr_clear_transform();

        ObjMesh::Params sphere_params("../scenes/meshes/cube_sphere_full.obj", "");
        ObjMesh::Params floor_params("../scenes/meshes/plane.obj", "");



        FeignRenderer::fr_mesh("sphere_obj",
                               "triangle_mesh",
                               &sphere_params);

        // FeignRenderer::fr_mesh("plane_obj",
        //                        "triangle_mesh",
        //                        &floor_params);


        FeignRenderer::fr_mesh("plane_obj",
                               "grid",
                               &grid_params);


        SimpleMaterialShader::Params shad_params("simple_mat");

        FeignRenderer::fr_shader("diffuse_shad",
                                 "simple_material",
                                 &shad_params);

        SimpleMaterial::Params mesh_mat_p("mesh_bsdf");

        FeignRenderer::fr_material("simple_mat",
                                   "simple",
                                   &mesh_mat_p);

        Diffuse::Params radar_bsdf_params(Color3f(0.5f, 0.5f, 0.5f));

        FeignRenderer::fr_bsdf("mesh_bsdf",
                               "diffuse",
                               &radar_bsdf_params);

        PointEmitter::Params emitter_params(Color3f(400.f, 400.f, 400.f),
                                            origin + Vector3f(0.0, 1.0, 0.0));

        SinTexture::Params floor_tex(Vec3f(0.5f / 100.f, 0.0, 0.0),
                                     -Vec3f(i, 0.0, 0.0) * 0.04,
                                     Vec3f(100.f, 0.0, 0.0),
                                     Vec3f(0.f, 0.0, 0.0));

        FeignRenderer::fr_texture("displacement",
                                  "sin",
                                  &floor_tex);

        FeignRenderer::fr_emitter("point_emitter",
                                  "point",
                                  &emitter_params);

        // TODO: implement wave displacement

        PointEmitter::Params emitter_params_2(Color3f(800.f, 800.f, 800.f),
                                              Vector3f(0.f, 5.0 * interp_value(i, 100) - 0.2f, 4.f));

        FeignRenderer::fr_emitter("point_emitter_2",
                                  "point",
                                  &emitter_params_2);

        FeignRenderer::flush_renders();

        FeignRenderer::clean_up();
    }

    system(publish_command.c_str());
}
