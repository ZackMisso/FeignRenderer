#include "hall_of_tiles.h"
#include "tile_effects.h"
#include <feign/shapes/objmesh.h>

FEIGN_BEGIN()

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
    Mirror::Params mirror_bsdf(Color3f(0.98f));

    FeignRenderer::fr_bsdf("dark_diffuse_bsdf",
                           "diffuse",
                           &dark_diffuse_bsdf);

    FeignRenderer::fr_bsdf("mirror_bsdf",
                           "mirror",
                           &mirror_bsdf);
}

void add_tile_to_scene(HOT_Tile& tile, int& index)
{
    std::string material = "dark_diffuse_shad";
    std::string emitter = "";

    int object_type = tile.base_object_type;
    if (tile.override_object_type != -1)
        object_type = tile.override_object_type;

    // if (tile.light_scale > 1.f) std::cout << "AUUUUUGGGGAA" << std::endl;

    if (object_type == ACCENT_LIGHT)
    {
        // CREATE BLUE LIGHTS
        material = "dark_diffuse_shad";
        emitter = "tile_emitter_" + std::to_string(index);

        // if (tile.light_scale > 1.f) std::cout << "FUUUUCK" << std::endl;

        MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 0.7f, 0.8f) * tile.light_scale);

        FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
    }
    else if (object_type == HEAD_LIGHT)
    {
        // CREATE YELLOW LIGHTS
        material = "dark_diffuse_shad";
        emitter = "tile_emitter_" + std::to_string(index);

        MeshEmitter::Params mesh_emitter_params(Color3f(0.8f,
                                                        1.0f, 0.f) * tile.light_scale);

        FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
    }
    else if (object_type == DIFFUSE)
    {
        material = "dark_diffuse_shad";
    }
    else if (object_type == MIRROR)
    {
        material = "mirror_shad";
    }

    FeignRenderer::fr_clear_transform();

    FeignRenderer::fr_scale(0.95, 0.95, 0.95);
    FeignRenderer::fr_translate(tile.pos(0), tile.pos(1), tile.pos(2));
    FeignRenderer::fr_rotate(tile.z_rot, 0.f, 0.f, 1.f);

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

    FeignRenderer::fr_clear_transform();
}

void HallOfTiles::initialize_hallway(int frame)
{
    pcg32 rng = pcg32(0xacdc, 0x5493);

    std::vector<HOT_TileEffect*> all_tile_fx = std::vector<HOT_TileEffect*>();

    std::vector<HOT_Tile> bottom_tiles = std::vector<HOT_Tile>();
    std::vector<HOT_Tile> top_tiles = std::vector<HOT_Tile>();
    std::vector<HOT_Tile> left_tiles = std::vector<HOT_Tile>();
    std::vector<HOT_Tile> right_tiles = std::vector<HOT_Tile>();

    // all_tile_fx.push_back(new HOT_TileEffect_SetHeadLightLightScale(0, 4920, 2.8f));
    // all_tile_fx.push_back(new HOT_TileEffect_SetAccentLightLightScale(0, 4920, 0.3f));

    all_tile_fx.push_back(new HOT_TileEffect_SetHeadLightLightScale(0, 4920, 0.0f));
    all_tile_fx.push_back(new HOT_TileEffect_SetAccentLightLightScale(0, 4920, 0.0f));


    for (int start = 0; start < 2136; start += 56)
        all_tile_fx.push_back(new HOT_TileEffect_HeadLightAlight(start, start+100, 100.f));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(224, 274, 9.f));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(280, 330, 9.f));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(337, 387, 9.f));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(393, 443, 9.f));
    // all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(449, 499, 9.f));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(563, 663, 9.f));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(563+56, 663+56, 9.f));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(790, 890, 9.f));
    all_tile_fx.push_back(new HOT_TileEffect_AccentSetLightBeam(790, 890, 0.1));
    all_tile_fx.push_back(new HOT_TileEffect_SetAccentLightLightScale(890, 4920, 0.1));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(790+56, 890+56, 9.f));

    // all_tile_fx.push_back(new HOT_TileEffect_RotationWave(790+56+112, 890+112, 12.f, 0.f, 90.f));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(448, 498, 9.f,
                                                             false, false, true, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(448+12, 498+12, 9.f,
                                                             false, false, false, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(448+12*2, 498+12*2, 9.f,
                                                             true, false, false, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(448+12*3, 498+12*3, 9.f,
                                                             false, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(448+12*3 + 6, 498+12*3 + 6, 9.f,
                                                            false, false, true, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(448+12*4, 498+12*4, 9.f,
                                                            true, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(508, 558, 9.f,
                                                             false, false, true, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(508+12*1, 558+12*1, 9.f,
                                                             false, false, false, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(508+12*2, 558+12*2, 9.f,
                                                             true, false, false, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(508+12*3, 558+12*3, 9.f,
                                                             false, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(508+12*3+6, 558+12*3+6, 9.f,
                                                             false, false, true, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(508+12*4, 558+12*4, 9.f,
                                                             true, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(675, 725, 9.f,
                                                             false, false, true, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(675+12, 725+12, 9.f,
                                                             false, false, false, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(675+12*2, 725+12*2, 9.f,
                                                             true, false, false, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(675+12*3, 725+12*3, 9.f,
                                                             false, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(675+12*3 + 6, 725+12*3 + 6, 9.f,
                                                            false, false, true, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(675+12*4, 725+12*4, 9.f,
                                                            true, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(735, 785, 9.f,
                                                             false, false, true, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(735+12*1, 785+12*1, 9.f,
                                                             false, false, false, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(735+12*2, 785+12*2, 9.f,
                                                             true, false, false, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(735+12*3, 785+12*3, 9.f,
                                                             false, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(735+12*3+6, 785+12*3+6, 9.f,
                                                             false, false, true, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(735+12*4, 785+12*4, 9.f,
                                                             true, true, false, false));


    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(902, 952, 9.f,
                                                             false, false, true, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(902+12, 952+12, 9.f,
                                                             false, false, false, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(902+12*2, 952+12*2, 9.f,
                                                             true, false, false, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(902+12*3, 952+12*3, 9.f,
                                                             false, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(902+12*3 + 6, 952+12*3 + 6, 9.f,
                                                             false, false, true, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(902+12*4, 952+12*4, 9.f,
                                                             true, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962, 1012, 9.f,
                                                             false, false, true, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*1, 1012+12*1, 9.f,
                                                             false, false, false, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*2, 1012+12*2, 9.f,
                                                             true, false, false, false));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*3, 1012+12*3, 9.f,
                                                             false, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*3+6, 1012+12*3+6, 9.f,
                                                             false, false, true, true));
    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*4, 1012+12*4, 9.f,
                                                             true, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*5, 1012+12*5, 9.f,
                                                             false, false, true, true));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*5+6, 1012+12*5+6, 9.f,
                                                             false, false, true, true));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*6+6, 1012+12*6+6, 9.f,
                                                             false, false, true, true));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*7, 1012+12*7, 9.f,
                                                             false, false, true, true));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*8, 1012+12*8, 9.f,
                                                             false, false, true, true));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*8+6, 1012+12*8+6, 9.f,
                                                             false, false, true, true));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*9+6, 1012+12*9+6, 9.f,
                                                             true, false, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*9+6, 1012+12*9+6, 9.f,
                                                             false, true, false, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*9+6, 1012+12*9+6, 9.f,
                                                             false, false, true, false));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*9+6, 1012+12*9+6, 9.f,
                                                             false, false, false, true));

    all_tile_fx.push_back(new HOT_TileEffect_AccentLightWave(962+12*9+6, 1012+12*9+6, 9.f,
                                                             true, true, true, true));

    all_tile_fx.push_back(new HOT_TileEffect_RotationWave(1070, 1340, 60.f, 0.f, 540.f));

    // 56 / 4 = 14

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
                         std::abs(0.01f);

            sum = diffuse_prob + mirror_prob + light_prob;

            diffuse_prob /= sum;
            mirror_prob /= sum;
            light_prob /= sum;

            Vector3f pos = Vector3f(-4.5f + float(j), -5.f, float(i) - 0.5f);

            if (prob < diffuse_prob)
            {
                // add a diffuse tile
                bottom_tiles.push_back(HOT_Tile(DIFFUSE, pos));
            }
            else if (prob < diffuse_prob + mirror_prob)
            {
                // add a mirror tile
                bottom_tiles.push_back(HOT_Tile(MIRROR, pos));
            }
            else
            {
                // add a accent tile
                // bottom_tiles.push_back(HOT_Tile(ACCENT_LIGHT, pos));
                bottom_tiles.push_back(HOT_Tile(DIFFUSE, pos));
            }

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

            pos = Vector3f(-4.5f + float(j), 5.f, float(i) - 0.5f);

            if ((i%40 == 0 && j >=4 && j <= 6) ||
                (i%40 == 1 && j == 5) || (i%39 == 1 && j == 5))
            {
                // create a head light
                top_tiles.push_back(HOT_Tile(HEAD_LIGHT, pos));
                rng.nextDouble();
                rng.nextDouble();
            }
            else if (prob < diffuse_prob)
            {
                // create a diffuse tile
                top_tiles.push_back(HOT_Tile(DIFFUSE, pos));
            }
            else if (prob < diffuse_prob + mirror_prob)
            {
                // create a mirror tile
                top_tiles.push_back(HOT_Tile(MIRROR, pos));
            }
            else
            {
                // create accent light
                top_tiles.push_back(HOT_Tile(ACCENT_LIGHT, pos));
            }

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

            pos = Vector3f(-5.f, -4.5f + float(j), float(i) - 0.5f);

            diffuse_prob /= sum;
            mirror_prob /= sum;
            light_prob /= sum;

            if (prob < diffuse_prob)
            {
                // create a diffuse tile
                left_tiles.push_back(HOT_Tile(DIFFUSE, pos));
            }
            else if (prob < diffuse_prob + mirror_prob)
            {
                // create a mirror tile
                left_tiles.push_back(HOT_Tile(MIRROR, pos));
            }
            else
            {
                // create an accent tile
                left_tiles.push_back(HOT_Tile(ACCENT_LIGHT, pos));
            }

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

            pos = Vector3f(5.f, -4.5f + float(j), float(i) - 0.5f);

            diffuse_prob /= sum;
            mirror_prob /= sum;
            light_prob /= sum;

            if (prob < diffuse_prob)
            {
                // create a diffuse tile
                right_tiles.push_back(HOT_Tile(DIFFUSE, pos));
            }
            else if (prob < diffuse_prob + mirror_prob)
            {
                // create a mirror tile
                right_tiles.push_back(HOT_Tile(MIRROR, pos));
            }
            else
            {
                // create an accent tile
                right_tiles.push_back(HOT_Tile(ACCENT_LIGHT, pos));
            }
        }
    }

    for (int i = 0; i < all_tile_fx.size(); ++i)
    {
        if (all_tile_fx[i]->applies_to_top) all_tile_fx[i]->apply_to_tiles(top_tiles, frame);
        if (all_tile_fx[i]->applies_to_bottom) all_tile_fx[i]->apply_to_tiles(bottom_tiles, frame);
        if (all_tile_fx[i]->applies_to_left) all_tile_fx[i]->apply_to_tiles(left_tiles, frame);
        if (all_tile_fx[i]->applies_to_right) all_tile_fx[i]->apply_to_tiles(right_tiles, frame);
    }

    index = 0;
    for (int i = 0; i < bottom_tiles.size(); ++i)
    {
        add_tile_to_scene(bottom_tiles[i], index);
    }
    for (int i = 0; i < top_tiles.size(); ++i)
    {
        add_tile_to_scene(top_tiles[i], index);
    }
    for (int i = 0; i < left_tiles.size(); ++i)
    {
        add_tile_to_scene(left_tiles[i], index);
    }
    for (int i = 0; i < right_tiles.size(); ++i)
    {
        add_tile_to_scene(right_tiles[i], index);
    }
}

// notes of entire video
// frames 0-32 - introduction of yellow lights into the scene
// frames 33-56 - dim yellow lights
// every 24 frame (1 second intervals brighten then dim yellow lights)

void HallOfTiles::initialize_camera(int frame)
{
    // for the first 40 seconds, the camera should remain completely still
    int initial_still_time = 30 * 24;
    int actual_frame = std::max(frame - initial_still_time, 0);

    Perspective::Params cam_params(Vector3f(0.0,
                                            -0.4,
                                            0.0 + 0.1 * float(actual_frame)),
                                   Vector3f(0.0,
                                            -0.4,
                                            1.0 + 0.1 * float(actual_frame)),
                                   Vector3f(0, 1, 0),
                                   50.f,
                                   1e-4f,
                                   1e4f,
                                   10.f,
                                   0.f,
                                   // Vec2i(1920, 1080));
                                   Vec2i(256, 144));

    // Perspective::Params cam_params(Vector3f(0.0 + 0.45 * cos(1.f / 12.f * float(actual_frame)),
    //                                         -0.4 - 0.6 * std::abs(cos(0.04f * float(actual_frame))),
    //                                         0.0 + 0.1 * float(actual_frame)),
    //                                Vector3f(0.0 + 0.45 * cos(1.f / 12.f * float(actual_frame)),
    //                                         -0.4 - 0.6 * std::abs(cos(0.04f * float(actual_frame))),
    //                                         1.0 + 0.1 * float(actual_frame)),
    //                                Vector3f(0, 1, 0),
    //                                50.f,
    //                                1e-4f,
    //                                1e4f,
    //                                10.f,
    //                                0.f,
    //                                Vec2i(256, 256));

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

    // FeignRenderer::fr_accel("light_accel",
    //                         "light_spatial",
    //                         &light_accel_params);

    LOG("media");
    HomogeneousAbsorbingMedia::Params media_params(0.01);
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

    Independent::Params samp_params(64, 0x12345);
    // Independent::Params samp_params(20000, 0x12345);

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
    std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";

    // system(rm_command.c_str());
    system(mkdir_command.c_str());

    int start_frame = 1000;
    int end_frame = 1500;

    for (int frame = start_frame; frame < end_frame; frame++)
    {
        if (frame == 241) continue;
        LOG("Rendering Frame: " + std::to_string(frame));
        // float degree = (M_PI * float(frame + 180) / 180.f) / 2.f;
        // Vector3f origin = Vector3f(5.0 * cos(degree), 0.0, 5.0 * sin(degree));

        LOG("initializing base structs");
        initialize_base_structs(test_name, frame);

        LOG("initislizing camera");
        initialize_camera(frame);

        LOG("initializing hallway");
        initialize_hallway(frame);

        LOG("initializing the materials");
        initialize_materials(frame);

        LOG("initializing lighting");
        initialize_lighting(frame);

        LOG("rendering frame: " + std::to_string(frame));

        flush_render();
    }

    system(publish_command.c_str());
}

FEIGN_END()
