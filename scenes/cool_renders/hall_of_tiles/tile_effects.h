#define DIFFUSE 0
#define MIRROR 1
#define HEAD_LIGHT 2
#define ACCENT_LIGHT 3

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

    HOT_TileEffect(bool top,
                   bool bot,
                   bool left,
                   bool right)
        : applies_to_top(top),
          applies_to_bottom(bot),
          applies_to_right(right),
          applies_to_left(left),
          start_frame(0),
          end_frame(4920) { }

    HOT_TileEffect(bool top,
                   bool bot,
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
        : HOT_TileEffect(start_frame, end_frame),
          fall_off_dist(fall_off_dist)
    {
        start_z = -100.f;
        end_z = 520.f;
    }

    void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const
    {
        if (!is_active(frame)) return;

        Float proxy = Float(frame - start_frame) / Float(end_frame - start_frame);
        // std::cout << "what" << std::endl;
        // std::cout << proxy << std::endl;
        Float zpos = proxy * end_z + (1.0 - proxy) * start_z;

        // TODO: maybe implement different kinds of fall off, currently just using
        //       linear falloff
        for (int i = 0; i < tiles.size(); ++i)
        {
            if (tiles[i].base_object_type == HEAD_LIGHT)
            {
                Float zdist = std::abs(tiles[i].pos(2) - zpos);

                zdist = std::max(1.0 - zdist / fall_off_dist, 0.0);
                // std::cout << zdist << std::endl;

                tiles[i].light_scale = std::max(tiles[i].light_scale, zdist);
            }
        }
    }

    Float start_z;
    Float end_z;
    Float fall_off_dist;
};

struct HOT_TileEffect_AccentLightWave : public HOT_TileEffect
{
    HOT_TileEffect_AccentLightWave(int start_frame,
                                   int end_frame,
                                   Float fall_off_dist)
        : HOT_TileEffect(start_frame, end_frame),
          fall_off_dist(fall_off_dist)
    {
        start_z = -50.f;
        end_z = 470.f;
    }

    HOT_TileEffect_AccentLightWave(int start_frame,
                                   int end_frame,
                                   Float fall_off_dist,
                                   bool applies_top,
                                   bool applies_bot,
                                   bool applies_left,
                                   bool applies_right)
        : HOT_TileEffect(applies_top,
                         applies_bot,
                         applies_left,
                         applies_right,
                         start_frame,
                         end_frame),
          fall_off_dist(fall_off_dist)
    {
        start_z = -50.f;
        end_z = 470.f;
    }

    void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const
    {
        if (!is_active(frame)) return;

        Float proxy = Float(frame - start_frame) / Float(end_frame - start_frame);
        // std::cout << "what" << std::endl;
        // std::cout << proxy << std::endl;
        Float zpos = proxy * end_z + (1.0 - proxy) * start_z;

        // TODO: maybe implement different kinds of fall off, currently just using
        //       linear falloff
        for (int i = 0; i < tiles.size(); ++i)
        {
            if (tiles[i].base_object_type != MIRROR &&
                tiles[i].base_object_type != HEAD_LIGHT)
            {
                Float zdist = std::abs(tiles[i].pos(2) - zpos);

                zdist = std::max(1.0 - zdist / fall_off_dist, 0.0);

                if (zdist > 0.0)
                {
                    tiles[i].override_object_type = ACCENT_LIGHT;
                }

                tiles[i].light_scale = std::max(tiles[i].light_scale, zdist);
            }
            // if (tiles[i].base_object_type == HEAD_LIGHT)
            // {
            //     Float zdist = std::abs(tiles[i].pos(2) - zpos);
            //
            //     zdist = std::max(1.0 - zdist / fall_off_dist, 0.0);
            //     // std::cout << zdist << std::endl;
            //
            //     tiles[i].light_scale = std::max(tiles[i].light_scale, zdist);
            // }
        }
    }

    Float start_z;
    Float end_z;
    Float fall_off_dist;
};
