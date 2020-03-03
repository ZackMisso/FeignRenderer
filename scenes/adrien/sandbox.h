#include <feign/core/api.h>
#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>
#include <feign/stats/clocker.h>

std::string test_name = "sandbox";

std::string rm_command = "rm -rf " + test_name + "/";
std::string mkdir_command = "mkdir " + test_name + "/";
std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

int SPP = 2;
int FLOOR_RES = 64;
Vector3f origin = Vector3f (00.f, 9.f, -30.f);

void initialize_scene_stuff(int frame)
{
    LOG("starting frame: " + std::to_string(frame));

    char str[5];
    snprintf(str, 5, "%04d", frame);
    std::string name = test_name + "_" + std::string(str);

    FeignRenderer::initialize();

    #if CLOCKING
        Clocker::startClock("parse");
    #endif

    FeignRenderer::fr_scene(name,
                            "integrator",
                            "sampler",
                            "camera",
                            "");

    Integrator::Params int_params(720,720, test_name + "/");

    FeignRenderer::fr_integrator("integrator",
                                 // "nice_normal",
                                 "whitted",
                                 "default",
                                 &int_params);

    Independent::Params samp_params(SPP, 0x12345);

    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void initialize_camera(int frame)
{
    Perspective::Params cam_params(origin,
                                   Vector3f(0.f, 9.5f, 0.f),
                                   Vector3f(0, 1, 0),
                                   35.f,
                                   1e-4f,
                                   1e4f,
                                   10.f,
                                   0.f,
                                   Vec2i(256,256));

    FeignRenderer::fr_camera("camera",
                             "perspective",
                             &cam_params);
}

void initialize_floor(int frame)
{
    SimpleMaterialShader::Params shad_params("console_mat");

    FeignRenderer::fr_shader("console_shad",
                             "simple_material",
                             &shad_params);

    SimpleMaterial::Params mesh_mat_p("console_bsdf");
    Diffuse::Params mesh_bsdf_params(Color3f(0.7f, 0.7f, 0.7f));

    FeignRenderer::fr_material("console_mat",
                               "simple",
                               &mesh_mat_p);

    FeignRenderer::fr_bsdf("console_bsdf",
                           "diffuse",
                           &mesh_bsdf_params);

    GridObj::Params mesh_params("", "", Vec2i(FLOOR_RES, FLOOR_RES));

    FeignRenderer::fr_clear_transform();

    FeignRenderer::fr_scale(10.f, 1.f, 10.f);

    FeignRenderer::fr_object("floor",
                             "floor_obj",
                             "console_shad");

    FeignRenderer::fr_clear_transform();

    FeignRenderer::fr_mesh("floor_obj",
                           "grid",
                           &mesh_params);
}

void initialize_room()
{
    {
        SimpleMaterialShader::Params shad_params("left_mat");

        FeignRenderer::fr_shader("left_shad",
                                 "simple_material",
                                 &shad_params);

        SimpleMaterial::Params mesh_mat_p("left_bsdf");
        Diffuse::Params mesh_bsdf_params(Color3f(0.9f, 0.2f, 0.2f));

        FeignRenderer::fr_material("left_mat",
                                   "simple",
                                   &mesh_mat_p);

        FeignRenderer::fr_bsdf("left_bsdf",
                               "diffuse",
                               &mesh_bsdf_params);
    }

    {
        SimpleMaterialShader::Params shad_params("right_mat");

        FeignRenderer::fr_shader("right_shad",
                                "simple_material",
                                &shad_params);

        SimpleMaterial::Params mesh_mat_p("right_bsdf");
        Diffuse::Params mesh_bsdf_params(Color3f(0.2f, 0.2f, 0.9f));

        FeignRenderer::fr_material("right_mat",
                                  "simple",
                                  &mesh_mat_p);

        FeignRenderer::fr_bsdf("right_bsdf",
                              "diffuse",
                              &mesh_bsdf_params);
    }

    // roof
    {
        GridObj::Params mesh_params("", "", Vec2i(FLOOR_RES, FLOOR_RES));

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_scale(10.f, 1.f, 10.f);
        FeignRenderer::fr_rotate(180.f, 1.f, 0.f, 0.f);
        FeignRenderer::fr_translate(0.f, 20.f, 0.f);

        FeignRenderer::fr_object("roof",
                                 "roof_obj",
                                 "console_shad");

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_mesh("roof_obj",
                               "grid",
                               &mesh_params);
    }
    // back wall
    {
        GridObj::Params mesh_params("", "", Vec2i(FLOOR_RES, FLOOR_RES));

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_scale(10.f, 1.f, 10.f);
        FeignRenderer::fr_rotate(90.f, 1.f, 0.f, 0.f);
        FeignRenderer::fr_translate(0.f, 10.f, 10.f);

        FeignRenderer::fr_object("back_wall",
                                 "back_wall_obj",
                                 "console_shad");

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_mesh("back_wall_obj",
                               "grid",
                               &mesh_params);
    }
    // front wall
    // {
    //     GridObj::Params mesh_params("", "", Vec2i(FLOOR_RES, FLOOR_RES));
    //
    //     FeignRenderer::fr_clear_transform();
    //
    //     FeignRenderer::fr_scale(10.f, 1.f, 10.f);
    //     FeignRenderer::fr_rotate(-90.f, 1.f, 0.f, 0.f);
    //     FeignRenderer::fr_translate(0.f, 10.f, -10.f);
    //
    //     FeignRenderer::fr_object("front_wall",
    //                              "front_wall_obj",
    //                              "console_shad");
    //
    //     FeignRenderer::fr_clear_transform();
    //
    //     FeignRenderer::fr_mesh("front_wall_obj",
    //                            "grid",
    //                            &mesh_params);
    // }
    // left wall
    {
        GridObj::Params mesh_params("", "", Vec2i(FLOOR_RES, FLOOR_RES));

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_scale(10.f, 1.f, 10.f);
        FeignRenderer::fr_rotate(-90.f, 0.f, 0.f, 1.f);
        FeignRenderer::fr_translate(10.f, 10.f, 0.f);

        FeignRenderer::fr_object("left_wall",
                                 "left_wall_obj",
                                 "left_shad");

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_mesh("left_wall_obj",
                               "grid",
                               &mesh_params);
    }
    // right wall
    {
        GridObj::Params mesh_params("", "", Vec2i(FLOOR_RES, FLOOR_RES));

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_scale(10.f, 1.f, 10.f);
        FeignRenderer::fr_rotate(90.f, 0.f, 0.f, 1.f);
        FeignRenderer::fr_translate(-10.f, 10.f, 0.f);

        FeignRenderer::fr_object("right_wall",
                                 "right_wall_obj",
                                 "right_shad");

        FeignRenderer::fr_clear_transform();

        FeignRenderer::fr_mesh("right_wall_obj",
                               "grid",
                               &mesh_params);
    }
}

void initialize_lighting(int frame)
{
    // float degree = (M_PI * float(frame + 180) / 180.f);
    // Vector3f origin = Vector3f(-1.24528 + 65.0 * cos(degree), 51.0, -17.5872 + 65.0 * sin(degree));
    // Vector3f origin = Vector3f(0.f, 0.f, -5.f);

    PointEmitter::Params emitter_params(Color3f(1000.f, 1000.f, 1000.f),
                                        Vector3f(0.0, 10.0, 0.0));

    FeignRenderer::fr_emitter("point_emitter",
                              "point",
                              &emitter_params);

    // DirectionalEmitter::Params dir_params(Vector3f(0.f, -0.12f, -1.f), Color3f(1.f, 1.f, 1.f));
    //
    // FeignRenderer::fr_emitter("dir_emitter",
    //                           "directional",
    //                           &dir_params);

    // // TODO: WHY IS THIS POSITIVE AND NOT NEGATIVE
    // SpotLightEmitter::Params dir_params(Point3f(0.f, 5.f, -1.f),
    //                                     Vector3f(0.f, -1.0f, -0.1f),
    //                                     Color3f(1.f, 1.f, 1.f),
    //                                     20.f);
    //
    // FeignRenderer::fr_emitter("dir_emitter",
    //                           "spot",
    //                           &dir_params);
}

static void sandbox()
{
    system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 0;
    int end_frame = 1;

    for (int i = start_frame; i < end_frame; ++i)
    {
        initialize_scene_stuff(i);

        initialize_camera(i);

        initialize_floor(i);
        initialize_room();

        initialize_lighting(i);

        #if CLOCKING
           Clocker::endClock("parse");
        #endif

        FeignRenderer::flush_renders();

        FeignRenderer::clean_up();
    }
}
