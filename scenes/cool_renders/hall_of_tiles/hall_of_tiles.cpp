#include "hall_of_tiles.h"
#include <feign/shapes/objmesh.h>

HallOfTiles::HallOfTiles()
{
    run();
}

void HallOfTiles::initialize_materials(int frame)
{
    // we want 3 different materials:
    // a dark diffuse material,
    // a mirror material,
    // and a light material

    SimpleMaterialShader::Params dark_diffuse_params("dark_diffuse_mat");
    SimpleMaterialShader::Params mirror_params("mirror_mat");

    SimpleMaterial::Params dark_diffuse_mat_params("dark_diffuse_bsdf");
    SimpleMaterial::Params mirror_mat_params("mirror_bsdf");

    FeignRenderer::fr_shader("dark_diffuse_shad",
                             "simple_material",
                             &dark_diffuse_params);

    FeignRenderer::fr_shader("mirror_shad",
                             "simple_material",
                             &mirror_params);

    FeignRenderer::fr_material("dark_diffuse_mat",
                               "simple",
                               &dark_diffuse_mat_params);

    FeignRenderer::fr_material("mirror_mat",
                               "simple",
                               &mirror_mat_params);

    Diffuse::Params dark_diffuse_bsdf(Color3f(0.5f, 0.5f, 0.5f));
    Mirror::Params mirror_bsdf(Color3f(1.f));

    FeignRenderer::fr_bsdf("dark_diffuse_bsdf",
                           "diffuse",
                           &dark_diffuse_bsdf);

    FeignRenderer::fr_bsdf("mirror_bsdf",
                           "mirror",
                           &mirror_bsdf);
}

void HallOfTiles::initialize_hallway(int frame)
{
    pcg32 rng = pcg32(0xacdc, 0x5493);

    float mesh_light_intensity = 0.1;

    int index = 0;
    for (int i = 0; i < 400; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            float light_prob = 0.f;
            float diffuse_prob = 0.f;
            float mirror_prob = 0.f;
            float sum = 0.f;
            float prob = rng.nextFloat();
            std::string material = "dark_diffuse_shad";
            std::string emitter = "";

            // bottom tiles
            diffuse_prob = 5.f;
            mirror_prob = 5.f;
            light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
                         std::abs(0.05f);

            sum = diffuse_prob + mirror_prob + light_prob;

            diffuse_prob /= sum;
            mirror_prob /= sum;
            light_prob /= sum;

            if (prob < diffuse_prob) material = "dark_diffuse_shad";
            else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
            else
            {
                material = "dark_diffuse_shad";
                emitter = "tile_emitter_" + std::to_string(index);

                MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity);

                FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
            }

            FeignRenderer::fr_clear_transform();

            FeignRenderer::fr_translate(-4.5f + float(j), -4.5f, float(i) - 0.5f);

            FeignRenderer::fr_object("tile_" + std::to_string(index),
                                     "tile_obj_" + std::to_string(index),
                                     material,
                                     emitter);

            ObjMesh::Params params_1("../scenes/meshes/cube_tile.obj", "");

            FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
                                   "triangle_mesh",
                                   &params_1);

            index++;

            // top tiles
            light_prob = 0.f;
            diffuse_prob = 0.f;
            mirror_prob = 0.f;
            sum = 0.f;
            prob = rng.nextFloat();
            material = "dark_diffuse_shad";
            emitter = "";

            diffuse_prob = 6.f;
            mirror_prob = 4.f;
            light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
                         std::abs(j-5);

            sum = diffuse_prob + mirror_prob + light_prob;

            diffuse_prob /= sum;
            mirror_prob /= sum;
            light_prob /= sum;

            if ((i%40 == 0 && j >=4 && j <= 6) ||
                (i%40 == 1 && j == 5) || (i%39 == 1 && j == 5))
            {
                material = "dark_diffuse_shad";
                emitter = "tile_emitter_" + std::to_string(index);

                MeshEmitter::Params mesh_emitter_params(Color3f(2.f * rng.nextFloat() + 4.f,
                                                                2.f * rng.nextFloat() + 5.f, 0.f) * mesh_light_intensity * 2.f);

                FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
            }
            else if (prob < diffuse_prob) material = "dark_diffuse_shad";
            else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
            else
            {
                material = "dark_diffuse_shad";
                emitter = "tile_emitter_" + std::to_string(index);

                MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity);

                FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
            }

            FeignRenderer::fr_clear_transform();

            FeignRenderer::fr_translate(-4.5f + float(j), 4.5f, float(i) - 0.5f);

            FeignRenderer::fr_object("tile_" + std::to_string(index),
                                     "tile_obj_" + std::to_string(index),
                                     material,
                                     emitter);

            FeignRenderer::fr_clear_transform();

            ObjMesh::Params params_2("../scenes/meshes/cube_tile.obj", "");

            FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
                                   "triangle_mesh",
                                   &params_2);

            index++;

            // left tiles
            light_prob = 0.f;
            diffuse_prob = 0.f;
            mirror_prob = 0.f;
            sum = 0.f;
            prob = rng.nextFloat();
            material = "dark_diffuse_shad";
            emitter = "";

            diffuse_prob = 3.f;
            mirror_prob = 7.f;
            light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
                         0.5*std::abs(j-10);

            sum = diffuse_prob + mirror_prob + light_prob;

            diffuse_prob /= sum;
            mirror_prob /= sum;
            light_prob /= sum;

            if (prob < diffuse_prob) material = "dark_diffuse_shad";
            else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
            else
            {
                material = "dark_diffuse_shad";
                emitter = "tile_emitter_" + std::to_string(index);

                MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity);

                FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
            }

            FeignRenderer::fr_clear_transform();

            FeignRenderer::fr_translate(-4.5f, -4.5f + float(j), float(i) - 0.5f);

            FeignRenderer::fr_object("tile_" + std::to_string(index),
                                     "tile_obj_" + std::to_string(index),
                                     material,
                                     emitter);

            FeignRenderer::fr_clear_transform();

            ObjMesh::Params params_3("../scenes/meshes/cube_tile.obj", "");

            FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
                                   "triangle_mesh",
                                   &params_3);

            index++;

            // right tiles
            light_prob = 0.f;
            diffuse_prob = 0.f;
            mirror_prob = 0.f;
            sum = 0.f;
            prob = rng.nextFloat();
            material = "dark_diffuse_shad";
            emitter = "";

            diffuse_prob = 3.f;
            mirror_prob = 7.f;
            light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
                         0.5*std::abs(j-10);

            sum = diffuse_prob + mirror_prob + light_prob;

            diffuse_prob /= sum;
            mirror_prob /= sum;
            light_prob /= sum;

            if (prob < diffuse_prob) material = "dark_diffuse_shad";
            else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
            else
            {
                material = "dark_diffuse_shad";
                emitter = "tile_emitter_" + std::to_string(index);

                MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity);

                FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
            }

            FeignRenderer::fr_clear_transform();

            FeignRenderer::fr_translate(4.5f, -4.5f + float(j), float(i) - 0.5f);

            FeignRenderer::fr_object("tile_" + std::to_string(index),
                                     "tile_obj_" + std::to_string(index),
                                     material,
                                     emitter);

            FeignRenderer::fr_clear_transform();

            ObjMesh::Params params_4("../scenes/meshes/cube_tile.obj", "");

            FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
                                   "triangle_mesh",
                                   &params_4);

            index++;

            FeignRenderer::fr_clear_transform();
        }
    }


}

void HallOfTiles::initialize_camera(int frame)
{
    Perspective::Params cam_params(Vector3f(0.0 + 0.45 * cos(0.04f * frame),
                                            -0.2 - 0.3 * std::abs(cos(0.04f * frame)),
                                            0.0 + 0.1 * float(frame)),
                                   Vector3f(0.0 + 0.45 * cos(0.04f * frame),
                                            -0.2 - 0.3 * std::abs(cos(0.04f * frame)),
                                            1.0 + 0.1 * float(frame)),
                                   Vector3f(0, 1, 0),
                                   50.f,
                                   1e-4f,
                                   1e4f,
                                   10.f,
                                   0.f,
                                   Vec2i(256, 256));

    FeignRenderer::fr_camera("camera",
                             "perspective",
                             &cam_params);
}

void HallOfTiles::initialize_base_structs(std::string test_name,
                                          int frame)
{
    char str[5];
    snprintf(str, 5, "%04d", frame);
    std::string name = test_name + "_" + std::string(str);

    LOG("initializing");
    FeignRenderer::initialize();

    LOG("scene");
    FeignRenderer::fr_scene(name,
                            "integrator",
                            "sampler",
                            "camera",
                            "env_medium",
                            false);

    LOG("light accel");
    SpatialLightAccel::Params light_accel_params(1, 1, 10);

    FeignRenderer::fr_accel("light_accel",
                            "light_spatial",
                            &light_accel_params);

    LOG("media");
    HomogeneousAbsorbingMedia::Params media_params(0.02);
    FeignRenderer::fr_media("env_medium",
                            "homo_abs",
                            &media_params);

    Integrator::Params int_params(512,
                                  512,
                                  test_name + "/");

    LOG("integrator");
    FeignRenderer::fr_integrator("integrator",
                                 "volpath",
                                 "default",
                                 &int_params);

    Independent::Params samp_params(1024, 0x12345);

    LOG("sampler");
    FeignRenderer::fr_sampler("sampler",
                              "independent",
                              &samp_params);
}

void HallOfTiles::initialize_lighting(int frame)
{
    PointEmitter::Params emitter_params(Color3f(250.f, 250.f, 250.f),
                                        Vector3f(0.0, 1.0, -2.0));

    FeignRenderer::fr_emitter("point_emitter",
                              "point",
                              &emitter_params);
}

void HallOfTiles::flush_render()
{
    FeignRenderer::flush_renders();
    FeignRenderer::clean_up();
}

void HallOfTiles::run()
{
    std::string test_name = "hall_of_tiles";

    std::string rm_command = "rm -rf " + test_name + "/";
    std::string mkdir_command = "mkdir " + test_name + "/";
    std::string publish_command = "ffmpeg -r 60 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    // system(rm_command.c_str());
    // system(mkdir_command.c_str());

    int start_frame = 0;
    int end_frame = 400;

    // for (int frame = start_frame; frame < end_frame; frame++)
    // {
    //     LOG("Rendering Frame: " + std::to_string(frame));
    //     // float degree = (M_PI * float(frame + 180) / 180.f) / 2.f;
    //     // Vector3f origin = Vector3f(5.0 * cos(degree), 0.0, 5.0 * sin(degree));
    //
    //     LOG("initializing base structs");
    //     initialize_base_structs(test_name, frame);
    //
    //     LOG("initislizing camera");
    //     initialize_camera(frame);
    //
    //     LOG("initializing hallway");
    //     initialize_hallway(frame);
    //
    //     LOG("initializing the materials");
    //     initialize_materials(frame);
    //
    //     LOG("initializing lighting");
    //     initialize_lighting(frame);
    //
    //     LOG("rendering frame: " + std::to_string(frame));
    //
    //     flush_render();
    // }

    system(publish_command.c_str());
}
