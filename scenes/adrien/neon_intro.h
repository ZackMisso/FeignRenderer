#include <feign/core/api.h>
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>

static void neon_intro()
{
    std::string test_name = "neon_intro";
    int take = 18;

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -s 1920x1080 -i " + test_name + "/" + test_name + "_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + test_name + ".mp4";

    // system(rm_command.c_str()); $MaceWhip441
    // system(mkdir_command.c_str());

    // std::vector<imedit::Pixel> color_map = std::vector<imedit::Pixel>();
    // imedit::color_map_inferno(color_map);

    int start_frame = 0;
    int end_frame = 1;

    for (int i = start_frame; i < end_frame; ++i)
    {
        LOG("starting frame: " + std::to_string(i));
        // float degree = (M_PI * float(i + 180) / 180.f);
        // Vector3f origin = Vector3f(-1.24528 + 65.0 * cos(degree), 51.0, -17.5872 + 65.0 * sin(degree));

        char str[5];
        snprintf(str, 5, "%04d", i);
        std::string name = test_name + "_frame_" + std::string(str) + "_take_" + std::to_string(take);

        FeignRenderer::initialize();

        FeignRenderer::fr_scene(name,
                                "integrator",
                                "sampler",
                                "camera",
                                "");

        Integrator::Params int_params(1000, 1000, test_name + "/");

        FeignRenderer::fr_integrator("integrator",
                                     "whitted",
                                     "default",
                                     &int_params);

        Independent::Params samp_params(4, 0x12345);

        FeignRenderer::fr_sampler("sampler",
                                  "independent",
                                  &samp_params);

        Perspective::Params cam_params(Vector3f(0.f, 2.f, -2.f),
                                       Vector3f(0.f, 0.f, 0.7f),
                                       Vector3f(0, 1, 0),
                                       65.f,
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
                                 "wireframe_mat");

        GridObj::Params mesh_params("disp_texture", "", Vec2i(30, 30));

        FeignRenderer::fr_mesh("plane_obj",
                               "grid",
                               &mesh_params);

        // ObjMesh::Params mesh_params("../scenes/meshes/plane.obj", "");
        //
        // FeignRenderer::fr_mesh("plane_obj",
        //                        "triangle_mesh",
        //                        &mesh_params);

        ImageTexture::Params im_params("../scenes/textures/smile.png");

        FeignRenderer::fr_texture("disp_texture",
                                  "image",
                                  &im_params);

        WireframeMaterial::Params wireframe_mat_params("wireframe_bsdf",
                                                       "mesh_bsdf",
                                                       0.01);

        SimpleMaterial::Params simple_mat_params("wireframe_bsdf");

        FeignRenderer::fr_material("simple_mat",
                                   "simple",
                                   &simple_mat_params);

        FeignRenderer::fr_material("wireframe_mat",
                                   "wireframe",
                                   &wireframe_mat_params);

        Diffuse::Params wireframe_bsdf_params(Color3f(0.8f, 0.8f, 0.8f));

        FeignRenderer::fr_bsdf("wireframe_bsdf",
                               "diffuse",
                               &wireframe_bsdf_params);

        Diffuse::Params mesh_bsdf_params(Color3f(0.03f, 0.03f, 0.03f));

        FeignRenderer::fr_bsdf("mesh_bsdf",
                               "diffuse",
                               &mesh_bsdf_params);

        for (int j = 0; j < 20; ++j)
        {
            float degree = 2.0 * M_PI * float(j) / 20.f;
            Vector3f origin = Vector3f(2.0 * cos(degree), 4.0, 2.0 * sin(degree));

            PointEmitter::Params emitter_params(Color3f(20.f, 20.f, 20.f),
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
