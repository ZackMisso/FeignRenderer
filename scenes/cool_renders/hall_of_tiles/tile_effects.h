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
        z_rot = 0.f;
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
    Float light_scale;

    // the z rotation of the tile
    Float z_rot;

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
          fall_off_dist(fall_off_dist),
          additional_scale(1.f)
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
                                   bool applies_right,
                                   Float additional_scale=1.f)
        : HOT_TileEffect(applies_top,
                         applies_bot,
                         applies_left,
                         applies_right,
                         start_frame,
                         end_frame),
          fall_off_dist(fall_off_dist),
          additional_scale(additional_scale)
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

                tiles[i].light_scale = std::max(tiles[i].light_scale, zdist*additional_scale);
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
    Float additional_scale;
};

struct HOT_TileEffect_AccentSetLightBeam : public HOT_TileEffect
{
    HOT_TileEffect_AccentSetLightBeam(int start_frame,
                                      int end_frame,
                                      Float value)
        : HOT_TileEffect(start_frame, end_frame),
          value(value)
    {
        start_z = -50.f;
        end_z = 470.f;
    }

    HOT_TileEffect_AccentSetLightBeam(int start_frame,
                                      int end_frame,
                                      Float value,
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
          value(value)
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
                Float zdist = zpos - tiles[i].pos(2);

                if (zdist > 0.0)
                    tiles[i].light_scale = std::max(tiles[i].light_scale, value);
            }
        }
    }

    Float start_z;
    Float end_z;
    Float value;
};

struct HOT_TileEffect_RotationWave : public HOT_TileEffect
{
    HOT_TileEffect_RotationWave(int start_frame,
                                int end_frame,
                                Float fall_off_dist,
                                Float start_angle,
                                Float end_angle)
        : HOT_TileEffect(start_frame, end_frame),
          fall_off_dist(fall_off_dist),
          start_angle(start_angle),
          end_angle(end_angle)
    {
        start_z = -50.f;
        end_z = 470.f;
    }

    HOT_TileEffect_RotationWave(int start_frame,
                                int end_frame,
                                Float fall_off_dist,
                                Float start_angle,
                                Float end_angle,
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
          fall_off_dist(fall_off_dist),
          start_angle(start_angle),
          end_angle(end_angle)
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
            // if (tiles[i].base_object_type != MIRROR &&
            //     tiles[i].base_object_type != HEAD_LIGHT)
            // {
                Float zdist = (zpos + fall_off_dist) - tiles[i].pos(2);

                zdist = std::min(std::max(zdist / (2.0*fall_off_dist), 0.0), 1.0);

                tiles[i].z_rot = zdist * (end_angle - start_angle) + start_angle;


                // if (zdist > 0)
                //     std::cout << "angle: " << tiles[i].z_rot << std::endl;
            // }
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
    Float start_angle;
    Float end_angle;
};

struct HOT_TileEffect_JerkyRotate : public HOT_TileEffect
{
    HOT_TileEffect_JerkyRotate(int start_frame,
                               int end_frame,
                               Float min_rotate,
                               Float max_rotate,
                               Float min_min_length,
                               Float max_min_length,
                               Float min_max_length,
                               Float max_max_length,
                               Float transition_length,
                               pcg32& rng)
        : HOT_TileEffect(start_frame, end_frame)
    {
        min_rotations = std::vector<Float>();
        max_rotations = std::vector<Float>();
        min_lengths = std::vector<Float>();
        max_lengths = std::vector<Float>();
        transition_lengths = std::vector<Float>();

        for (int i = 0; i < 400; ++i)
        {
            min_rotations.push_back(-rng.nextDouble() * (max_rotate - min_rotate) + min_rotate);
            max_rotations.push_back(rng.nextDouble() * (max_rotate - min_rotate) + min_rotate);
            min_lengths.push_back(int(rng.nextDouble() * (max_min_length - min_min_length) + min_min_length));
            max_lengths.push_back(int(rng.nextDouble() * (max_max_length - min_max_length) + min_max_length));
            transition_lengths.push_back(transition_length);
        }
    }

    // HOT_TileEffect_JerkyRotate(int start_frame,
    //                            int end_frame,
    //                            Float fall_off_dist,
    //                            Float start_angle,
    //                            Float end_angle,
    //                            bool applies_top,
    //                            bool applies_bot,
    //                            bool applies_left,
    //                            bool applies_right)
    //     : HOT_TileEffect(applies_top,
    //                      applies_bot,
    //                      applies_left,
    //                      applies_right,
    //                      start_frame,
    //                      end_frame),
    //       fall_off_dist(fall_off_dist),
    //       start_angle(start_angle),
    //       end_angle(end_angle)
    // {
    //     start_z = -50.f;
    //     end_z = 470.f;
    // }

    int get_stage(int frame, int index, Float& proxy) const
    {
        // returns 0 - if min,
        //         1 - if max,
        //         2 - transitioning from min to max,
        //         3 - transitioning from max to min

        Float base_frame = Float(frame);
        proxy = 0.f;

        int total_dist = 2 * transition_lengths[index] +
                         max_lengths[index] + min_lengths[index];

        int loc = frame % total_dist;
        if (loc < min_lengths[index]) return 0;
        if (loc < min_lengths[index] + transition_lengths[index])
        {
            Float proxy = Float(loc - min_lengths[index]) / (transition_lengths[index]);
            return 2;
        }
        if (loc < min_lengths[index] + transition_lengths[index] + max_lengths[index])
            return 1;
        else
        {
            Float proxy = Float(loc - min_lengths[index] - transition_lengths[index] - max_lengths[index]) / (transition_lengths[index]);
            return 3;
        }

        // while (base_frame > 0)
        // {
        //     base_frame -= min_lengths[index];
        //     if (base_frame < 0) return 0;
        //     base_frame -= transition_lengths[index];
        //     if (base_frame < 0)
        //     {
        //         Float val = base_frame + Float(transition_lengths[index]);
        //         proxy = Float(val) / Float(transition_lengths[index]-1);
        //         return 2;
        //     }
        //     base_frame -= max_lengths[index];
        //     if (base_frame < 0) return 1;
        //     base_frame -= transition_lengths[index];
        //     if (base_frame < 0)
        //     {
        //         Float val = base_frame + (transition_lengths[index]);
        //         proxy = Float(val) / Float(transition_lengths[index]-1);
        //
        //         if (proxy < 0.0) LOG("FUCK");
        //         if (proxy > 1.0) LOG("FUCK");
        //         return 3;
        //     }
        // }
    }

    void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const
    {
        if (!is_active(frame)) return;

        for (int i = 0; i < tiles.size(); ++i)
        {
            int index = std::floor(tiles[i].pos[2]);
            if (index < 0) index = 0;
            if (index > 399) index = 399;

            Float proxy = 0.0;
            int stage = get_stage(frame, index, proxy);

            if (stage == 0)
            {
                tiles[i].z_rot = min_rotations[index];
            }
            if (stage == 1)
            {
                tiles[i].z_rot = max_rotations[index];
            }
            if (stage == 2)
            {
                tiles[i].z_rot = (1.0 - proxy) * min_rotations[index] +
                                 proxy * max_rotations[index];
            }
            if (stage == 3)
            {
                tiles[i].z_rot = (1.0 - proxy) * max_rotations[index] +
                                 proxy * min_rotations[index];
            }
        }
    }

    std::vector<Float> min_rotations;
    std::vector<Float> max_rotations;
    std::vector<Float> min_lengths;
    std::vector<Float> max_lengths;
    std::vector<Float> transition_lengths;
};

struct HOT_TileEffect_PulsatingAccent : public HOT_TileEffect
{
    HOT_TileEffect_PulsatingAccent(int start_frame,
                                   int end_frame,
                                   Float base_scale,
                                   Float scale_var,
                                   Float min_freq,
                                   Float max_freq,
                                   pcg32& rng)
        : HOT_TileEffect(start_frame, end_frame),
          base_scale(base_scale),
          scale_var(scale_var)
    {
        frequencies = std::vector<Float>();

        for (int i = 0; i < 400; ++i)
        {
            Float freq = rng.nextDouble() * (max_freq - min_freq) + min_freq;

            frequencies.push_back(freq);
        }
    }

    void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const
    {
        if (!is_active(frame)) return;

        for (int i = 0; i < tiles.size(); ++i)
        {
            int loc = std::floor(tiles[i].pos[2]);

            if (loc < 0) loc = 0;
            if (loc > 399) loc = 399;

            Float freq = frequencies[loc];

            Float scale = base_scale + scale_var * cos(freq * (frame - start_frame));

            if (tiles[i].base_object_type != MIRROR &&
                tiles[i].base_object_type != HEAD_LIGHT)
            {
                tiles[i].light_scale = scale;
            }
        }
    }

    std::vector<Float> frequencies;
    Float base_scale;
    Float scale_var;
};

struct HOT_TileEffect_SectionBeam : public HOT_TileEffect
{
    HOT_TileEffect_SectionBeam(int start_frame,
                               int end_frame,
                               Float value)
        : HOT_TileEffect(start_frame, end_frame),
          value(value)
    {
        start_z = 0.f;
        end_z = 400.f;
    }

    void apply_to_tiles(std::vector<HOT_Tile>& tiles, int frame) const
    {
        if (!is_active(frame)) return;

        Float proxy = Float(frame - start_frame) / Float(end_frame - start_frame);
        Float zpos = proxy * (end_z - start_z) + start_z;
        int index = std::floor(zpos / 80.f);

        if (index >= 0 || index <= 7)
        {
            for (int i = 0; i < tiles.size(); ++i)
            {
                if (tiles[i].base_object_type != MIRROR &&
                    tiles[i].base_object_type != HEAD_LIGHT)
                {
                    int tile_index = std::floor(tiles[i].pos(2) / 80.f);

                    if (tile_index == index)
                    {
                        tiles[i].override_object_type = ACCENT_LIGHT;
                        tiles[i].light_scale = std::max(tiles[i].light_scale, value);
                    }
                }
            }
        }
    }

    Float value;
    Float start_z;
    Float end_z;
};
