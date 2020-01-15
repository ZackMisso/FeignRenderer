#include <feign/core/api.h>

// January 5, 2020
// This render tests spherical radar shader

Point3f create_rand_point_on_sphere(float r, pcg32& rng)
{
    float theta = rng.nextFloat() * M_PI;
    float psi = rng.nextFloat() * 2.0 * M_PI;

    return Point3f(r * sin(theta) * cos(psi),
                   r * sin(theta) * sin(psi),
                   r * cos(theta));
}

static void jan_5_2020()
{
    std::string test_name = "jan_5_2020";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    // system(rm_command.c_str());
    // system(mkdir_command.c_str());

    int start_frame = 670;
    int end_frame = 720;

    for (int i = start_frame; i < end_frame; i++)
    {
        pcg32 rng(0xadf2, 0x2312);
        LOG("starting frame: " + std::to_string(i));
        float degree = (M_PI * float(0 + 180) / 180.f) / 2.f;
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
                                 "radar_mat");

        ObjMesh::Params sphere_params("../scenes/meshes/cube_sphere_full.obj", "");

        FeignRenderer::fr_mesh("sphere_obj",
                               "triangle_mesh",
                               &sphere_params);

        Float proxy = float(i);

        float one = 300.0 + rng.nextFloat() * 319.0;
        float two = 300.0 + rng.nextFloat() * 319.0;
        float three = 300.0 + rng.nextFloat() * 319.0;
        float four = 300.0 + rng.nextFloat() * 319.0;
        float five = 300.0 + rng.nextFloat() * 319.0;
        float six = 300.0 + rng.nextFloat() * 319.0;
        float seven = 300.0 + rng.nextFloat() * 319.0;
        float eight = 300.0 + rng.nextFloat() * 319.0;
        float nine = 300.0 + rng.nextFloat() * 319.0;
        float ten = 300.0 + rng.nextFloat() * 319.0;
        float eleven = 300.0 + rng.nextFloat() * 319.0;
        float twelve = 300.0 + rng.nextFloat() * 319.0;
        float thirteen = 300.0 + rng.nextFloat() * 319.0;
        float fourteen = 300.0 + rng.nextFloat() * 319.0;
        float fifeteen = 300.0 + rng.nextFloat() * 319.0;

        RadarMaterial::Params radar_mat_params("radar_bsdf",
                                               "mesh_bsdf",
                                               std::vector<Point3f> {Point3f(-sqrt(3.0) / 2.0, 0.f, 0.f),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng),
                                                                     create_rand_point_on_sphere(sqrt(3.0) / 2.0, rng)},
                                               std::vector<Float> {sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f),
                                                                   sqrt(3.f)},
                                               std::vector<Float> {0.0,
                                                                   100.0,
                                                                   100.0,
                                                                   200.0,
                                                                   200.0,
                                                                   200.0,
                                                                   300.0,
                                                                   one,
                                                                   two,
                                                                   three,
                                                                   four,
                                                                   five,
                                                                   six,
                                                                   seven,
                                                                   eight,
                                                                   nine,
                                                                   ten,
                                                                   eleven,
                                                                   twelve,
                                                                   thirteen,
                                                                   fourteen,
                                                                   fifeteen,
                                                                   620.0},
                                               std::vector<Float> {100.0,
                                                                   200.0,
                                                                   200.0,
                                                                   300.0,
                                                                   300.0,
                                                                   300.0,
                                                                   400.0,
                                                                   one + 100.f,
                                                                   two + 100.f,
                                                                   three + 100.f,
                                                                   four + 100.f,
                                                                   five + 100.f,
                                                                   six + 100.f,
                                                                   seven + 100.f,
                                                                   eight + 100.f,
                                                                   nine + 100.f,
                                                                   ten + 100.f,
                                                                   eleven + 100.f,
                                                                   twelve + 100.f,
                                                                   thirteen + 100.f,
                                                                   fourteen + 100.f,
                                                                   fifeteen + 100.f,
                                                                   719.0},
                                               0.05,
                                               0.1,
                                               proxy);

        FeignRenderer::fr_material("radar_mat",
                                   "radar",
                                   &radar_mat_params);

        Diffuse::Params radar_bsdf_params(Color3f(0.1f, 1.0f, 0.1f));

        FeignRenderer::fr_bsdf("radar_bsdf",
                               "diffuse",
                               &radar_bsdf_params);

        FeignRenderer::fr_bsdf("mesh_bsdf",
                               "nullbsdf",
                               nullptr);

        PointEmitter::Params emitter_params(Color3f(40.f, 40.f, 40.f),
                                            origin + Vector3f(0.0, 1.0, 0.0));

        FeignRenderer::fr_emitter("point_emitter",
                                  "point",
                                  "null",
                                  "null",
                                  &emitter_params);

        FeignRenderer::flush_renders();

        FeignRenderer::clean_up();
    }

    // system(publish_command.c_str());
}
