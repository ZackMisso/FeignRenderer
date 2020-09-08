#include "hall_of_tiles.h"
#include <feign/shapes/objmesh.h>

FEIGN_BEGIN()

#define DIFFUSE 0
#define MIRROR 1
#define HEAD_LIGHT 2
#define ACCENT_LIGHT 3

#define HARDCODED_WAY false

struct HOT_Tile
{
    HOT_Tile(int type, Vector3f pos)
        : base_object_type(type), pos(pos)
    {
        override_object_type = -1;
        light_scale = 1.f;
    }

    // 0 = diffuse
    // 1 = mirror
    // 2 = head light   (yellow)
    // 3 = accent light (blue)
    int base_object_type;

    // if the object should be overrided for an effect, it will be (diffuse)
    // material only
    int override_object_type;

    // the scale for the light
    float light_scale;

    Vector3f pos;
};

// TODO: add a repeating mechanic maybe?

struct HOT_TileEffect
{
    HOT_TileEffect()
        : applies_to_bottom(true),
          applies_to_top(true),
          applies_to_left(true),
          applies_to_right(true),
          start_frame(0),
          end_frame(4920) { }

    HOT_TileEffect(bool bot,
                   bool top,
                   bool left,
                   bool right)
        : applies_to_top(top),
          applies_to_bottom(bot),
          applies_to_right(right),
          applies_to_left(left),
          start_frame(0),
          end_frame(4920) { }

    HOT_TileEffect(bool bot,
                   bool top,
                   bool left,
                   bool right,
                   int start,
                   int end)
        : applies_to_top(top),
          applies_to_bottom(bot),
          applies_to_right(right),
          applies_to_left(left),
          start_frame(start),
          end_frame(end) { }

    HOT_TileEffect(int start, int end)
        : applies_to_top(true),
          applies_to_bottom(true),
          applies_to_right(true),
          applies_to_left(true),
          start_frame(start),
          end_frame(end) { }

    virtual void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const = 0;

    bool is_active(int frame) const
    {
        return frame >= start_frame && frame < end_frame;
    }

    bool applies_to_bottom;
    bool applies_to_top;
    bool applies_to_left;
    bool applies_to_right;

    int start_frame;
    int end_frame;
};

struct HOT_TileEffect_SetHeadLightLightScale : public HOT_TileEffect
{
    HOT_TileEffect_SetHeadLightLightScale(int start_frame,
                                          int end_frame,
                                          Float scale)
        : HOT_TileEffect(start_frame, end_frame),
          scale(scale) { }

    void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const
    {
        if (!is_active(frame)) return;

        for (int i = 0; i < tiles.size(); ++i)
        {
            if (tiles[i].base_object_type == HEAD_LIGHT)
            {
                tiles[i].light_scale = scale;
            }
        }
    }

    Float scale;
};

struct HOT_TileEffect_SetAccentLightLightScale : public HOT_TileEffect
{
    HOT_TileEffect_SetAccentLightLightScale(int start_frame,
                                            int end_frame,
                                            Float scale)
        : HOT_TileEffect(start_frame, end_frame),
          scale(scale) { }

    void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const
    {
        if (!is_active(frame)) return;

        for (int i = 0; i < tiles.size(); ++i)
        {
            if (tiles[i].base_object_type == ACCENT_LIGHT)
            {
                tiles[i].light_scale = scale;
            }
        }
    }

    Float scale;
};

struct HOT_TileEffect_HeadLightAlight : public HOT_TileEffect
{
    HOT_TileEffect_HeadLightAlight(int start_frame,
                                   int end_frame,
                                   Float fall_off_dist)
    {
        start_z = 0.f;
        end_z = 420.f;
    }

    void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const
    {
        if (!is_active(frame)) return;

        Float proxy = Float(frame - start_frame) / Float(end_frame - start_frame);
        Float zpos = proxy * end_z + (1.0 - proxy) * start_z;

        // TODO: maybe implement different kinds of fall off, currently just using
        //       linear falloff
        for (int i = 0; i < tiles.size(); ++i)
        {
            if (tiles[i].base_object_type == HEAD_LIGHT)
            {
                Float zdist = std::abs(tiles[i].pos(2) - zpos);

                zdist = std::max(1.0 - zdist / fall_off_dist, 0.0);

                tiles[i].light_scale = std::max(tiles[i].light_scale, zdist);
            }
        }
    }

    Float start_z;
    Float end_z;
    Float fall_off_dist;
};

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

    std::vector<HOT_TileEffect*> all_tile_fx = std::vector<HOT_TileEffect*>();

    std::vector<HOT_Tile> bottom_tiles = std::vector<HOT_Tile>();
    std::vector<HOT_Tile> top_tiles = std::vector<HOT_Tile>();
    std::vector<HOT_Tile> left_tiles = std::vector<HOT_Tile>();
    std::vector<HOT_Tile> right_tiles = std::vector<HOT_Tile>();

    all_tile_fx.push_back(new HOT_TileEffect_SetHeadLightLightScale(0, 4920, 1.f));
    all_tile_fx.push_back(new HOT_TileEffect_SetAccentLightLightScale(0, 4920, 1.f));

    // how to light the cyan meshes:
    // MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);

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

            if (prob < diffuse_prob)
            {
                // add a diffuse tile
                // TODO
            }
            else if (prob < diffuse_prob + mirror_prob)
            {
                // add a mirror tile
                // TODO
            }
            else
            {
                // add a accent tile
                // TODO
            }

            if (prob < diffuse_prob) material = "dark_diffuse_shad";
            else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
            else
            {
                material = "dark_diffuse_shad";
                emitter = "tile_emitter_" + std::to_string(index);

                #ifdef HARDCODED_WAY
                float frame_intensity = 1.f;
                if (frame < 240)
                {
                    frame_intensity = 0.f;
                }
                #endif
                // else
                // {
                //     frame_intensity = 1.f * mesh_light_intensity;
                // }

                MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);

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
                // CREATE YELLOW LIGHTS
                material = "dark_diffuse_shad";
                emitter = "tile_emitter_" + std::to_string(index);

                float frame_intensity = 0.f;
                if (frame <= 32)
                {
                    frame_intensity = double(frame) / 32.f;
                }
                else if (frame < 32 + 24) // 55
                {
                    int tmp = (frame-32) % 24;
                    frame_intensity = double(24 - tmp) / 24.0;
                }
                else if (frame < 32*2 + 24) // 87
                {
                    frame_intensity = double(frame - 32 - 24) / 32.0;
                }
                else if (frame < 32*2 + 24*2) // 111
                {
                    int tmp = (frame - 32*2 - 24) % 24;
                    frame_intensity = double(24 - tmp) / 24.0;
                }
                else if (frame < 32*3 + 24*2) // 143
                {
                    frame_intensity = double(frame - 2*32 - 2*24) / 32.0;
                }
                else if (frame < 32*3 + 24*3) // 167
                {
                    int tmp = (frame - 32*3 - 24*2) % 24;
                    frame_intensity = double(24 - tmp) / 24.0;
                }
                else if (frame < 32*4 + 24*3) // 199
                {
                    frame_intensity = double(frame - 3*32 - 3*24) / 32.0;
                }

                MeshEmitter::Params mesh_emitter_params(Color3f(2.f * rng.nextFloat() + 4.f,
                                                                2.f * rng.nextFloat() + 5.f, 0.f) * mesh_light_intensity * 2.f * frame_intensity);

                FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
            }
            else if (prob < diffuse_prob) material = "dark_diffuse_shad";
            else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
            else
            {
                // CREATE BLUE LIGHTS
                material = "dark_diffuse_shad";
                emitter = "tile_emitter_" + std::to_string(index);

                float frame_intensity = 1.f;
                if (frame < 240)
                {
                    frame_intensity = 0.f;
                }

                MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);

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

                float frame_intensity = 1.f;
                if (frame < 240)
                {
                    frame_intensity = 0.f;
                }

                MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);

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

                float frame_intensity = 1.f;
                if (frame < 240)
                {
                    frame_intensity = 0.f;
                }

                MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);

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

    // int index = 0;
    // for (int i = 0; i < 400; ++i)
    // {
    //     for (int j = 0; j < 10; ++j)
    //     {
    //         float light_prob = 0.f;
    //         float diffuse_prob = 0.f;
    //         float mirror_prob = 0.f;
    //         float sum = 0.f;
    //         float prob = rng.nextFloat();
    //         std::string material = "dark_diffuse_shad";
    //         std::string emitter = "";
    //
    //         // bottom tiles
    //         diffuse_prob = 5.f;
    //         mirror_prob = 5.f;
    //         light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
    //                      std::abs(0.05f);
    //
    //         sum = diffuse_prob + mirror_prob + light_prob;
    //
    //         diffuse_prob /= sum;
    //         mirror_prob /= sum;
    //         light_prob /= sum;
    //
    //         if (prob < diffuse_prob) material = "dark_diffuse_shad";
    //         else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
    //         else
    //         {
    //             material = "dark_diffuse_shad";
    //             emitter = "tile_emitter_" + std::to_string(index);
    //
    //             float frame_intensity = 1.f;
    //             if (frame < 240)
    //             {
    //                 frame_intensity = 0.f;
    //             }
    //             // else
    //             // {
    //             //     frame_intensity = 1.f * mesh_light_intensity;
    //             // }
    //
    //             MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
    //
    //             FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
    //         }
    //
    //         FeignRenderer::fr_clear_transform();
    //
    //         FeignRenderer::fr_translate(-4.5f + float(j), -4.5f, float(i) - 0.5f);
    //
    //         FeignRenderer::fr_object("tile_" + std::to_string(index),
    //                                  "tile_obj_" + std::to_string(index),
    //                                  material,
    //                                  emitter);
    //
    //         ObjMesh::Params params_1("../scenes/meshes/cube_tile.obj", "");
    //
    //         FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
    //                                "triangle_mesh",
    //                                &params_1);
    //
    //         index++;
    //
    //         // top tiles
    //         light_prob = 0.f;
    //         diffuse_prob = 0.f;
    //         mirror_prob = 0.f;
    //         sum = 0.f;
    //         prob = rng.nextFloat();
    //         material = "dark_diffuse_shad";
    //         emitter = "";
    //
    //         diffuse_prob = 6.f;
    //         mirror_prob = 4.f;
    //         light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
    //                      std::abs(j-5);
    //
    //         sum = diffuse_prob + mirror_prob + light_prob;
    //
    //         diffuse_prob /= sum;
    //         mirror_prob /= sum;
    //         light_prob /= sum;
    //
    //         if ((i%40 == 0 && j >=4 && j <= 6) ||
    //             (i%40 == 1 && j == 5) || (i%39 == 1 && j == 5))
    //         {
    //             // CREATE YELLOW LIGHTS
    //             material = "dark_diffuse_shad";
    //             emitter = "tile_emitter_" + std::to_string(index);
    //
    //             float frame_intensity = 0.f;
    //             if (frame <= 32)
    //             {
    //                 frame_intensity = double(frame) / 32.f;
    //             }
    //             else if (frame < 32 + 24) // 55
    //             {
    //                 int tmp = (frame-32) % 24;
    //                 frame_intensity = double(24 - tmp) / 24.0;
    //             }
    //             else if (frame < 32*2 + 24) // 87
    //             {
    //                 frame_intensity = double(frame - 32 - 24) / 32.0;
    //             }
    //             else if (frame < 32*2 + 24*2) // 111
    //             {
    //                 int tmp = (frame - 32*2 - 24) % 24;
    //                 frame_intensity = double(24 - tmp) / 24.0;
    //             }
    //             else if (frame < 32*3 + 24*2) // 143
    //             {
    //                 frame_intensity = double(frame - 2*32 - 2*24) / 32.0;
    //             }
    //             else if (frame < 32*3 + 24*3) // 167
    //             {
    //                 int tmp = (frame - 32*3 - 24*2) % 24;
    //                 frame_intensity = double(24 - tmp) / 24.0;
    //             }
    //             else if (frame < 32*4 + 24*3) // 199
    //             {
    //                 frame_intensity = double(frame - 3*32 - 3*24) / 32.0;
    //             }
    //
    //             MeshEmitter::Params mesh_emitter_params(Color3f(2.f * rng.nextFloat() + 4.f,
    //                                                             2.f * rng.nextFloat() + 5.f, 0.f) * mesh_light_intensity * 2.f * frame_intensity);
    //
    //             FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
    //         }
    //         else if (prob < diffuse_prob) material = "dark_diffuse_shad";
    //         else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
    //         else
    //         {
    //             // CREATE BLUE LIGHTS
    //             material = "dark_diffuse_shad";
    //             emitter = "tile_emitter_" + std::to_string(index);
    //
    //             float frame_intensity = 1.f;
    //             if (frame < 240)
    //             {
    //                 frame_intensity = 0.f;
    //             }
    //
    //             MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
    //
    //             FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
    //         }
    //
    //         FeignRenderer::fr_clear_transform();
    //
    //         FeignRenderer::fr_translate(-4.5f + float(j), 4.5f, float(i) - 0.5f);
    //
    //         FeignRenderer::fr_object("tile_" + std::to_string(index),
    //                                  "tile_obj_" + std::to_string(index),
    //                                  material,
    //                                  emitter);
    //
    //         FeignRenderer::fr_clear_transform();
    //
    //         ObjMesh::Params params_2("../scenes/meshes/cube_tile.obj", "");
    //
    //         FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
    //                                "triangle_mesh",
    //                                &params_2);
    //
    //         index++;
    //
    //         // left tiles
    //         light_prob = 0.f;
    //         diffuse_prob = 0.f;
    //         mirror_prob = 0.f;
    //         sum = 0.f;
    //         prob = rng.nextFloat();
    //         material = "dark_diffuse_shad";
    //         emitter = "";
    //
    //         diffuse_prob = 3.f;
    //         mirror_prob = 7.f;
    //         light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
    //                      0.5*std::abs(j-10);
    //
    //         sum = diffuse_prob + mirror_prob + light_prob;
    //
    //         diffuse_prob /= sum;
    //         mirror_prob /= sum;
    //         light_prob /= sum;
    //
    //         if (prob < diffuse_prob) material = "dark_diffuse_shad";
    //         else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
    //         else
    //         {
    //             material = "dark_diffuse_shad";
    //             emitter = "tile_emitter_" + std::to_string(index);
    //
    //             float frame_intensity = 1.f;
    //             if (frame < 240)
    //             {
    //                 frame_intensity = 0.f;
    //             }
    //
    //             MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
    //
    //             FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
    //         }
    //
    //         FeignRenderer::fr_clear_transform();
    //
    //         FeignRenderer::fr_translate(-4.5f, -4.5f + float(j), float(i) - 0.5f);
    //
    //         FeignRenderer::fr_object("tile_" + std::to_string(index),
    //                                  "tile_obj_" + std::to_string(index),
    //                                  material,
    //                                  emitter);
    //
    //         FeignRenderer::fr_clear_transform();
    //
    //         ObjMesh::Params params_3("../scenes/meshes/cube_tile.obj", "");
    //
    //         FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
    //                                "triangle_mesh",
    //                                &params_3);
    //
    //         index++;
    //
    //         // right tiles
    //         light_prob = 0.f;
    //         diffuse_prob = 0.f;
    //         mirror_prob = 0.f;
    //         sum = 0.f;
    //         prob = rng.nextFloat();
    //         material = "dark_diffuse_shad";
    //         emitter = "";
    //
    //         diffuse_prob = 3.f;
    //         mirror_prob = 7.f;
    //         light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
    //                      0.5*std::abs(j-10);
    //
    //         sum = diffuse_prob + mirror_prob + light_prob;
    //
    //         diffuse_prob /= sum;
    //         mirror_prob /= sum;
    //         light_prob /= sum;
    //
    //         if (prob < diffuse_prob) material = "dark_diffuse_shad";
    //         else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
    //         else
    //         {
    //             material = "dark_diffuse_shad";
    //             emitter = "tile_emitter_" + std::to_string(index);
    //
    //             float frame_intensity = 1.f;
    //             if (frame < 240)
    //             {
    //                 frame_intensity = 0.f;
    //             }
    //
    //             MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
    //
    //             FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
    //         }
    //
    //         FeignRenderer::fr_clear_transform();
    //
    //         FeignRenderer::fr_translate(4.5f, -4.5f + float(j), float(i) - 0.5f);
    //
    //         FeignRenderer::fr_object("tile_" + std::to_string(index),
    //                                  "tile_obj_" + std::to_string(index),
    //                                  material,
    //                                  emitter);
    //
    //         FeignRenderer::fr_clear_transform();
    //
    //         ObjMesh::Params params_4("../scenes/meshes/cube_tile.obj", "");
    //
    //         FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
    //                                "triangle_mesh",
    //                                &params_4);
    //
    //         index++;
    //
    //         FeignRenderer::fr_clear_transform();
    //     }
    // }
}

// notes of entire video
// frames 0-32 - introduction of yellow lights into the scene
// frames 33-56 - dim yellow lights
// every 24 frame (1 second intervals brighten then dim yellow lights)

void HallOfTiles::initialize_camera(int frame)
{
    // for the first 40 seconds, the camera should remain completely still
    int initial_still_time = 40 * 24;
    int actual_frame = std::max(frame - initial_still_time, 0);

    Perspective::Params cam_params(Vector3f(0.0 + 0.45 * cos(0.04f * actual_frame),
                                            -0.2 - 0.3 * std::abs(cos(0.04f * actual_frame)),
                                            0.0 + 0.1 * float(actual_frame)),
                                   Vector3f(0.0 + 0.45 * cos(0.04f * actual_frame),
                                            -0.2 - 0.3 * std::abs(cos(0.04f * actual_frame)),
                                            1.0 + 0.1 * float(actual_frame)),
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

    Independent::Params samp_params(256, 0x12345);

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

    int start_frame = 0;
    int end_frame = 9*24;

    for (int frame = start_frame; frame < end_frame; frame++)
    {
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

// PRE CHANGING ALL THINGS

// FEIGN_BEGIN()
//
// HallOfTiles::HallOfTiles()
// {
//     run();
// }
//
// void HallOfTiles::initialize_materials(int frame)
// {
//     // we want 3 different materials:
//     // a dark diffuse material,
//     // a mirror material,
//     // and a light material
//
//     SimpleMaterialShader::Params dark_diffuse_params("dark_diffuse_mat");
//     SimpleMaterialShader::Params mirror_params("mirror_mat");
//
//     SimpleMaterial::Params dark_diffuse_mat_params("dark_diffuse_bsdf");
//     SimpleMaterial::Params mirror_mat_params("mirror_bsdf");
//
//     FeignRenderer::fr_shader("dark_diffuse_shad",
//                              "simple_material",
//                              &dark_diffuse_params);
//
//     FeignRenderer::fr_shader("mirror_shad",
//                              "simple_material",
//                              &mirror_params);
//
//     FeignRenderer::fr_material("dark_diffuse_mat",
//                                "simple",
//                                &dark_diffuse_mat_params);
//
//     FeignRenderer::fr_material("mirror_mat",
//                                "simple",
//                                &mirror_mat_params);
//
//     Diffuse::Params dark_diffuse_bsdf(Color3f(0.5f, 0.5f, 0.5f));
//     Mirror::Params mirror_bsdf(Color3f(1.f));
//
//     FeignRenderer::fr_bsdf("dark_diffuse_bsdf",
//                            "diffuse",
//                            &dark_diffuse_bsdf);
//
//     FeignRenderer::fr_bsdf("mirror_bsdf",
//                            "mirror",
//                            &mirror_bsdf);
// }
//
// void HallOfTiles::initialize_hallway(int frame)
// {
//     pcg32 rng = pcg32(0xacdc, 0x5493);
//
//     float mesh_light_intensity = 0.1;
//
//     int index = 0;
//     for (int i = 0; i < 400; ++i)
//     {
//         for (int j = 0; j < 10; ++j)
//         {
//             float light_prob = 0.f;
//             float diffuse_prob = 0.f;
//             float mirror_prob = 0.f;
//             float sum = 0.f;
//             float prob = rng.nextFloat();
//             std::string material = "dark_diffuse_shad";
//             std::string emitter = "";
//
//             // bottom tiles
//             diffuse_prob = 5.f;
//             mirror_prob = 5.f;
//             light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
//                          std::abs(0.05f);
//
//             sum = diffuse_prob + mirror_prob + light_prob;
//
//             diffuse_prob /= sum;
//             mirror_prob /= sum;
//             light_prob /= sum;
//
//             if (prob < diffuse_prob) material = "dark_diffuse_shad";
//             else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
//             else
//             {
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 1.f;
//                 if (frame < 240)
//                 {
//                     frame_intensity = 0.f;
//                 }
//                 // else
//                 // {
//                 //     frame_intensity = 1.f * mesh_light_intensity;
//                 // }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//
//             FeignRenderer::fr_clear_transform();
//
//             FeignRenderer::fr_translate(-4.5f + float(j), -4.5f, float(i) - 0.5f);
//
//             FeignRenderer::fr_object("tile_" + std::to_string(index),
//                                      "tile_obj_" + std::to_string(index),
//                                      material,
//                                      emitter);
//
//             ObjMesh::Params params_1("../scenes/meshes/cube_tile.obj", "");
//
//             FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
//                                    "triangle_mesh",
//                                    &params_1);
//
//             index++;
//
//             // top tiles
//             light_prob = 0.f;
//             diffuse_prob = 0.f;
//             mirror_prob = 0.f;
//             sum = 0.f;
//             prob = rng.nextFloat();
//             material = "dark_diffuse_shad";
//             emitter = "";
//
//             diffuse_prob = 6.f;
//             mirror_prob = 4.f;
//             light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
//                          std::abs(j-5);
//
//             sum = diffuse_prob + mirror_prob + light_prob;
//
//             diffuse_prob /= sum;
//             mirror_prob /= sum;
//             light_prob /= sum;
//
//             if ((i%40 == 0 && j >=4 && j <= 6) ||
//                 (i%40 == 1 && j == 5) || (i%39 == 1 && j == 5))
//             {
//                 // CREATE YELLOW LIGHTS
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 0.f;
//                 if (frame <= 32)
//                 {
//                     frame_intensity = double(frame) / 32.f;
//                 }
//                 else if (frame < 32 + 24) // 55
//                 {
//                     int tmp = (frame-32) % 24;
//                     frame_intensity = double(24 - tmp) / 24.0;
//                 }
//                 else if (frame < 32*2 + 24) // 87
//                 {
//                     frame_intensity = double(frame - 32 - 24) / 32.0;
//                 }
//                 else if (frame < 32*2 + 24*2) // 111
//                 {
//                     int tmp = (frame - 32*2 - 24) % 24;
//                     frame_intensity = double(24 - tmp) / 24.0;
//                 }
//                 else if (frame < 32*3 + 24*2) // 143
//                 {
//                     frame_intensity = double(frame - 2*32 - 2*24) / 32.0;
//                 }
//                 else if (frame < 32*3 + 24*3) // 167
//                 {
//                     int tmp = (frame - 32*3 - 24*2) % 24;
//                     frame_intensity = double(24 - tmp) / 24.0;
//                 }
//                 else if (frame < 32*4 + 24*3) // 199
//                 {
//                     frame_intensity = double(frame - 3*32 - 3*24) / 32.0;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(2.f * rng.nextFloat() + 4.f,
//                                                                 2.f * rng.nextFloat() + 5.f, 0.f) * mesh_light_intensity * 2.f * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//             else if (prob < diffuse_prob) material = "dark_diffuse_shad";
//             else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
//             else
//             {
//                 // CREATE BLUE LIGHTS
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 1.f;
//                 if (frame < 240)
//                 {
//                     frame_intensity = 0.f;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//
//             FeignRenderer::fr_clear_transform();
//
//             FeignRenderer::fr_translate(-4.5f + float(j), 4.5f, float(i) - 0.5f);
//
//             FeignRenderer::fr_object("tile_" + std::to_string(index),
//                                      "tile_obj_" + std::to_string(index),
//                                      material,
//                                      emitter);
//
//             FeignRenderer::fr_clear_transform();
//
//             ObjMesh::Params params_2("../scenes/meshes/cube_tile.obj", "");
//
//             FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
//                                    "triangle_mesh",
//                                    &params_2);
//
//             index++;
//
//             // left tiles
//             light_prob = 0.f;
//             diffuse_prob = 0.f;
//             mirror_prob = 0.f;
//             sum = 0.f;
//             prob = rng.nextFloat();
//             material = "dark_diffuse_shad";
//             emitter = "";
//
//             diffuse_prob = 3.f;
//             mirror_prob = 7.f;
//             light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
//                          0.5*std::abs(j-10);
//
//             sum = diffuse_prob + mirror_prob + light_prob;
//
//             diffuse_prob /= sum;
//             mirror_prob /= sum;
//             light_prob /= sum;
//
//             if (prob < diffuse_prob) material = "dark_diffuse_shad";
//             else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
//             else
//             {
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 1.f;
//                 if (frame < 240)
//                 {
//                     frame_intensity = 0.f;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//
//             FeignRenderer::fr_clear_transform();
//
//             FeignRenderer::fr_translate(-4.5f, -4.5f + float(j), float(i) - 0.5f);
//
//             FeignRenderer::fr_object("tile_" + std::to_string(index),
//                                      "tile_obj_" + std::to_string(index),
//                                      material,
//                                      emitter);
//
//             FeignRenderer::fr_clear_transform();
//
//             ObjMesh::Params params_3("../scenes/meshes/cube_tile.obj", "");
//
//             FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
//                                    "triangle_mesh",
//                                    &params_3);
//
//             index++;
//
//             // right tiles
//             light_prob = 0.f;
//             diffuse_prob = 0.f;
//             mirror_prob = 0.f;
//             sum = 0.f;
//             prob = rng.nextFloat();
//             material = "dark_diffuse_shad";
//             emitter = "";
//
//             diffuse_prob = 3.f;
//             mirror_prob = 7.f;
//             light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
//                          0.5*std::abs(j-10);
//
//             sum = diffuse_prob + mirror_prob + light_prob;
//
//             diffuse_prob /= sum;
//             mirror_prob /= sum;
//             light_prob /= sum;
//
//             if (prob < diffuse_prob) material = "dark_diffuse_shad";
//             else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
//             else
//             {
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 1.f;
//                 if (frame < 240)
//                 {
//                     frame_intensity = 0.f;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//
//             FeignRenderer::fr_clear_transform();
//
//             FeignRenderer::fr_translate(4.5f, -4.5f + float(j), float(i) - 0.5f);
//
//             FeignRenderer::fr_object("tile_" + std::to_string(index),
//                                      "tile_obj_" + std::to_string(index),
//                                      material,
//                                      emitter);
//
//             FeignRenderer::fr_clear_transform();
//
//             ObjMesh::Params params_4("../scenes/meshes/cube_tile.obj", "");
//
//             FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
//                                    "triangle_mesh",
//                                    &params_4);
//
//             index++;
//
//             FeignRenderer::fr_clear_transform();
//         }
//     }
// }
//
// // notes of entire video
// // frames 0-32 - introduction of yellow lights into the scene
// // frames 33-56 - dim yellow lights
// // every 24 frame (1 second intervals brighten then dim yellow lights)
//
// void HallOfTiles::initialize_camera(int frame)
// {
//     // for the first 40 seconds, the camera should remain completely still
//     int initial_still_time = 40 * 24;
//     int actual_frame = std::max(frame - initial_still_time, 0);
//
//     Perspective::Params cam_params(Vector3f(0.0 + 0.45 * cos(0.04f * actual_frame),
//                                             -0.2 - 0.3 * std::abs(cos(0.04f * actual_frame)),
//                                             0.0 + 0.1 * float(actual_frame)),
//                                    Vector3f(0.0 + 0.45 * cos(0.04f * actual_frame),
//                                             -0.2 - 0.3 * std::abs(cos(0.04f * actual_frame)),
//                                             1.0 + 0.1 * float(actual_frame)),
//                                    Vector3f(0, 1, 0),
//                                    50.f,
//                                    1e-4f,
//                                    1e4f,
//                                    10.f,
//                                    0.f,
//                                    Vec2i(256, 256));
//
//     FeignRenderer::fr_camera("camera",
//                              "perspective",
//                              &cam_params);
// }
//
// void HallOfTiles::initialize_base_structs(std::string test_name,
//                                           int frame)
// {
//     char str[5];
//     snprintf(str, 5, "%04d", frame);
//     std::string name = test_name + "_" + std::string(str);
//
//     LOG("initializing");
//     FeignRenderer::initialize();
//
//     LOG("scene");
//     FeignRenderer::fr_scene(name,
//                             "integrator",
//                             "sampler",
//                             "camera",
//                             "env_medium",
//                             false);
//
//     LOG("light accel");
//     SpatialLightAccel::Params light_accel_params(1, 1, 10);
//
//     FeignRenderer::fr_accel("light_accel",
//                             "light_spatial",
//                             &light_accel_params);
//
//     LOG("media");
//     HomogeneousAbsorbingMedia::Params media_params(0.01);
//     FeignRenderer::fr_media("env_medium",
//                             "homo_abs",
//                             &media_params);
//
//     Integrator::Params int_params(512,
//                                   512,
//                                   test_name + "/");
//
//     LOG("integrator");
//     FeignRenderer::fr_integrator("integrator",
//                                  "volpath",
//                                  "default",
//                                  &int_params);
//
//     Independent::Params samp_params(256, 0x12345);
//
//     LOG("sampler");
//     FeignRenderer::fr_sampler("sampler",
//                               "independent",
//                               &samp_params);
// }
//
// void HallOfTiles::initialize_lighting(int frame)
// {
//     PointEmitter::Params emitter_params(Color3f(250.f, 250.f, 250.f),
//                                         Vector3f(0.0, 1.0, -2.0));
//
//     FeignRenderer::fr_emitter("point_emitter",
//                               "point",
//                               &emitter_params);
// }
//
// void HallOfTiles::flush_render()
// {
//     FeignRenderer::flush_renders();
//     FeignRenderer::clean_up();
// }
//
// void HallOfTiles::run()
// {
//     std::string test_name = "hall_of_tiles";
//
//     std::string rm_command = "rm -rf " + test_name + "/";
//     std::string mkdir_command = "mkdir " + test_name + "/";
//     std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";
//
//     // system(rm_command.c_str());
//     system(mkdir_command.c_str());
//
//     int start_frame = 0;
//     int end_frame = 9*24;
//
//     for (int frame = start_frame; frame < end_frame; frame++)
//     {
//         LOG("Rendering Frame: " + std::to_string(frame));
//         // float degree = (M_PI * float(frame + 180) / 180.f) / 2.f;
//         // Vector3f origin = Vector3f(5.0 * cos(degree), 0.0, 5.0 * sin(degree));
//
//         LOG("initializing base structs");
//         initialize_base_structs(test_name, frame);
//
//         LOG("initislizing camera");
//         initialize_camera(frame);
//
//         LOG("initializing hallway");
//         initialize_hallway(frame);
//
//         LOG("initializing the materials");
//         initialize_materials(frame);
//
//         LOG("initializing lighting");
//         initialize_lighting(frame);
//
//         LOG("rendering frame: " + std::to_string(frame));
//
//         flush_render();
//     }
//
//     system(publish_command.c_str());
// }
//
// FEIGN_END()







// OLD VERSION BEFORE I MESS THINGS UP
// #include "hall_of_tiles.h"
// #include <feign/shapes/objmesh.h>
//
// FEIGN_BEGIN()
//
// HallOfTiles::HallOfTiles()
// {
//     run();
// }
//
// void HallOfTiles::initialize_materials(int frame)
// {
//     // we want 3 different materials:
//     // a dark diffuse material,
//     // a mirror material,
//     // and a light material
//
//     SimpleMaterialShader::Params dark_diffuse_params("dark_diffuse_mat");
//     SimpleMaterialShader::Params mirror_params("mirror_mat");
//
//     SimpleMaterial::Params dark_diffuse_mat_params("dark_diffuse_bsdf");
//     SimpleMaterial::Params mirror_mat_params("mirror_bsdf");
//
//     FeignRenderer::fr_shader("dark_diffuse_shad",
//                              "simple_material",
//                              &dark_diffuse_params);
//
//     FeignRenderer::fr_shader("mirror_shad",
//                              "simple_material",
//                              &mirror_params);
//
//     FeignRenderer::fr_material("dark_diffuse_mat",
//                                "simple",
//                                &dark_diffuse_mat_params);
//
//     FeignRenderer::fr_material("mirror_mat",
//                                "simple",
//                                &mirror_mat_params);
//
//     Diffuse::Params dark_diffuse_bsdf(Color3f(0.5f, 0.5f, 0.5f));
//     Mirror::Params mirror_bsdf(Color3f(1.f));
//
//     FeignRenderer::fr_bsdf("dark_diffuse_bsdf",
//                            "diffuse",
//                            &dark_diffuse_bsdf);
//
//     FeignRenderer::fr_bsdf("mirror_bsdf",
//                            "mirror",
//                            &mirror_bsdf);
// }
//
// void HallOfTiles::initialize_hallway(int frame)
// {
//     pcg32 rng = pcg32(0xacdc, 0x5493);
//
//     float mesh_light_intensity = 0.1;
//
//     int index = 0;
//     for (int i = 0; i < 400; ++i)
//     {
//         for (int j = 0; j < 10; ++j)
//         {
//             float light_prob = 0.f;
//             float diffuse_prob = 0.f;
//             float mirror_prob = 0.f;
//             float sum = 0.f;
//             float prob = rng.nextFloat();
//             std::string material = "dark_diffuse_shad";
//             std::string emitter = "";
//
//             // bottom tiles
//             diffuse_prob = 5.f;
//             mirror_prob = 5.f;
//             light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
//                          std::abs(0.05f);
//
//             sum = diffuse_prob + mirror_prob + light_prob;
//
//             diffuse_prob /= sum;
//             mirror_prob /= sum;
//             light_prob /= sum;
//
//             if (prob < diffuse_prob) material = "dark_diffuse_shad";
//             else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
//             else
//             {
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 1.f;
//                 if (frame < 240)
//                 {
//                     frame_intensity = 0.f;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//
//             FeignRenderer::fr_clear_transform();
//
//             FeignRenderer::fr_translate(-4.5f + float(j), -4.5f, float(i) - 0.5f);
//
//             FeignRenderer::fr_object("tile_" + std::to_string(index),
//                                      "tile_obj_" + std::to_string(index),
//                                      material,
//                                      emitter);
//
//             ObjMesh::Params params_1("../scenes/meshes/cube_tile.obj", "");
//
//             FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
//                                    "triangle_mesh",
//                                    &params_1);
//
//             index++;
//
//             // top tiles
//             light_prob = 0.f;
//             diffuse_prob = 0.f;
//             mirror_prob = 0.f;
//             sum = 0.f;
//             prob = rng.nextFloat();
//             material = "dark_diffuse_shad";
//             emitter = "";
//
//             diffuse_prob = 6.f;
//             mirror_prob = 4.f;
//             light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
//                          std::abs(j-5);
//
//             sum = diffuse_prob + mirror_prob + light_prob;
//
//             diffuse_prob /= sum;
//             mirror_prob /= sum;
//             light_prob /= sum;
//
//             if ((i%40 == 0 && j >=4 && j <= 6) ||
//                 (i%40 == 1 && j == 5) || (i%39 == 1 && j == 5))
//             {
//                 // CREATE YELLOW LIGHTS
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 0.f;
//                 if (frame <= 32)
//                 {
//                     frame_intensity = double(frame) / 32.f;
//                 }
//                 else if (((frame-32) / 24) % 2 == 0)
//                 {
//                     int tmp = (frame-32) % 24;
//                     frame_intensity = double(24 - tmp) / 24.0;
//                 }
//                 else
//                 {
//                     int tmp = (frame-32) % 24;
//                     frame_intensity = double(tmp) / 24.0;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(2.f * rng.nextFloat() + 4.f,
//                                                                 2.f * rng.nextFloat() + 5.f, 0.f) * mesh_light_intensity * 2.f * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//             else if (prob < diffuse_prob) material = "dark_diffuse_shad";
//             else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
//             else
//             {
//                 // CREATE BLUE LIGHTS
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 1.f;
//                 if (frame < 240)
//                 {
//                     frame_intensity = 0.f;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//
//             FeignRenderer::fr_clear_transform();
//
//             FeignRenderer::fr_translate(-4.5f + float(j), 4.5f, float(i) - 0.5f);
//
//             FeignRenderer::fr_object("tile_" + std::to_string(index),
//                                      "tile_obj_" + std::to_string(index),
//                                      material,
//                                      emitter);
//
//             FeignRenderer::fr_clear_transform();
//
//             ObjMesh::Params params_2("../scenes/meshes/cube_tile.obj", "");
//
//             FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
//                                    "triangle_mesh",
//                                    &params_2);
//
//             index++;
//
//             // left tiles
//             light_prob = 0.f;
//             diffuse_prob = 0.f;
//             mirror_prob = 0.f;
//             sum = 0.f;
//             prob = rng.nextFloat();
//             material = "dark_diffuse_shad";
//             emitter = "";
//
//             diffuse_prob = 3.f;
//             mirror_prob = 7.f;
//             light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
//                          0.5*std::abs(j-10);
//
//             sum = diffuse_prob + mirror_prob + light_prob;
//
//             diffuse_prob /= sum;
//             mirror_prob /= sum;
//             light_prob /= sum;
//
//             if (prob < diffuse_prob) material = "dark_diffuse_shad";
//             else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
//             else
//             {
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 1.f;
//                 if (frame < 240)
//                 {
//                     frame_intensity = 0.f;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//
//             FeignRenderer::fr_clear_transform();
//
//             FeignRenderer::fr_translate(-4.5f, -4.5f + float(j), float(i) - 0.5f);
//
//             FeignRenderer::fr_object("tile_" + std::to_string(index),
//                                      "tile_obj_" + std::to_string(index),
//                                      material,
//                                      emitter);
//
//             FeignRenderer::fr_clear_transform();
//
//             ObjMesh::Params params_3("../scenes/meshes/cube_tile.obj", "");
//
//             FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
//                                    "triangle_mesh",
//                                    &params_3);
//
//             index++;
//
//             // right tiles
//             light_prob = 0.f;
//             diffuse_prob = 0.f;
//             mirror_prob = 0.f;
//             sum = 0.f;
//             prob = rng.nextFloat();
//             material = "dark_diffuse_shad";
//             emitter = "";
//
//             diffuse_prob = 3.f;
//             mirror_prob = 7.f;
//             light_prob = std::max(float(i % 30) - 24.f, 0.1f) *
//                          0.5*std::abs(j-10);
//
//             sum = diffuse_prob + mirror_prob + light_prob;
//
//             diffuse_prob /= sum;
//             mirror_prob /= sum;
//             light_prob /= sum;
//
//             if (prob < diffuse_prob) material = "dark_diffuse_shad";
//             else if (prob < diffuse_prob + mirror_prob) material = "mirror_shad";
//             else
//             {
//                 material = "dark_diffuse_shad";
//                 emitter = "tile_emitter_" + std::to_string(index);
//
//                 float frame_intensity = 1.f;
//                 if (frame < 240)
//                 {
//                     frame_intensity = 0.f;
//                 }
//
//                 MeshEmitter::Params mesh_emitter_params(Color3f(0.f, 7.f, 8.f) * mesh_light_intensity * frame_intensity);
//
//                 FeignRenderer::fr_emitter(emitter, "mesh", &mesh_emitter_params);
//             }
//
//             FeignRenderer::fr_clear_transform();
//
//             FeignRenderer::fr_translate(4.5f, -4.5f + float(j), float(i) - 0.5f);
//
//             FeignRenderer::fr_object("tile_" + std::to_string(index),
//                                      "tile_obj_" + std::to_string(index),
//                                      material,
//                                      emitter);
//
//             FeignRenderer::fr_clear_transform();
//
//             ObjMesh::Params params_4("../scenes/meshes/cube_tile.obj", "");
//
//             FeignRenderer::fr_mesh("tile_obj_" + std::to_string(index),
//                                    "triangle_mesh",
//                                    &params_4);
//
//             index++;
//
//             FeignRenderer::fr_clear_transform();
//         }
//     }
// }
//
// // notes of entire video
// // frames 0-32 - introduction of yellow lights into the scene
// // frames 33-56 - dim yellow lights
// // every 24 frame (1 second intervals brighten then dim yellow lights)
//
// void HallOfTiles::initialize_camera(int frame)
// {
//     // for the first 40 seconds, the camera should remain completely still
//     int initial_still_time = 40 * 24;
//     int actual_frame = std::max(frame - initial_still_time, 0);
//
//     Perspective::Params cam_params(Vector3f(0.0 + 0.45 * cos(0.04f * actual_frame),
//                                             -0.2 - 0.3 * std::abs(cos(0.04f * actual_frame)),
//                                             0.0 + 0.1 * float(actual_frame)),
//                                    Vector3f(0.0 + 0.45 * cos(0.04f * actual_frame),
//                                             -0.2 - 0.3 * std::abs(cos(0.04f * actual_frame)),
//                                             1.0 + 0.1 * float(actual_frame)),
//                                    Vector3f(0, 1, 0),
//                                    50.f,
//                                    1e-4f,
//                                    1e4f,
//                                    10.f,
//                                    0.f,
//                                    Vec2i(256, 256));
//
//     FeignRenderer::fr_camera("camera",
//                              "perspective",
//                              &cam_params);
// }
//
// void HallOfTiles::initialize_base_structs(std::string test_name,
//                                           int frame)
// {
//     char str[5];
//     snprintf(str, 5, "%04d", frame);
//     std::string name = test_name + "_" + std::string(str);
//
//     LOG("initializing");
//     FeignRenderer::initialize();
//
//     LOG("scene");
//     FeignRenderer::fr_scene(name,
//                             "integrator",
//                             "sampler",
//                             "camera",
//                             "env_medium",
//                             false);
//
//     LOG("light accel");
//     SpatialLightAccel::Params light_accel_params(1, 1, 10);
//
//     FeignRenderer::fr_accel("light_accel",
//                             "light_spatial",
//                             &light_accel_params);
//
//     LOG("media");
//     HomogeneousAbsorbingMedia::Params media_params(0.02);
//     FeignRenderer::fr_media("env_medium",
//                             "homo_abs",
//                             &media_params);
//
//     Integrator::Params int_params(512,
//                                   512,
//                                   test_name + "/");
//
//     LOG("integrator");
//     FeignRenderer::fr_integrator("integrator",
//                                  "volpath",
//                                  "default",
//                                  &int_params);
//
//     Independent::Params samp_params(256, 0x12345);
//
//     LOG("sampler");
//     FeignRenderer::fr_sampler("sampler",
//                               "independent",
//                               &samp_params);
// }
//
// void HallOfTiles::initialize_lighting(int frame)
// {
//     PointEmitter::Params emitter_params(Color3f(250.f, 250.f, 250.f),
//                                         Vector3f(0.0, 1.0, -2.0));
//
//     FeignRenderer::fr_emitter("point_emitter",
//                               "point",
//                               &emitter_params);
// }
//
// void HallOfTiles::flush_render()
// {
//     FeignRenderer::flush_renders();
//     FeignRenderer::clean_up();
// }
//
// void HallOfTiles::run()
// {
//     std::string test_name = "hall_of_tiles";
//
//     std::string rm_command = "rm -rf " + test_name + "/";
//     std::string mkdir_command = "mkdir " + test_name + "/";
//     std::string publish_command = "ffmpeg -r 24 -f image2 -i " + test_name + "/" + test_name + "_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M " + test_name + "/" + test_name + ".mp4";
//
//     // system(rm_command.c_str());
//     system(mkdir_command.c_str());
//
//     int start_frame = 42;
//     int end_frame = 9*24;
//
//     for (int frame = start_frame; frame < end_frame; frame++)
//     {
//         LOG("Rendering Frame: " + std::to_string(frame));
//         // float degree = (M_PI * float(frame + 180) / 180.f) / 2.f;
//         // Vector3f origin = Vector3f(5.0 * cos(degree), 0.0, 5.0 * sin(degree));
//
//         LOG("initializing base structs");
//         initialize_base_structs(test_name, frame);
//
//         LOG("initislizing camera");
//         initialize_camera(frame);
//
//         LOG("initializing hallway");
//         initialize_hallway(frame);
//
//         LOG("initializing the materials");
//         initialize_materials(frame);
//
//         LOG("initializing lighting");
//         initialize_lighting(frame);
//
//         LOG("rendering frame: " + std::to_string(frame));
//
//         flush_render();
//     }
//
//     system(publish_command.c_str());
// }
//
// FEIGN_END()
