/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

#include <feign/parser/json_parser.h>
#include <feign/core/api.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>

#include <cstdio>
#include <fstream>

void JsonParser::parse(std::string filename)
{
    FILE* file = fopen(filename.c_str(), "r");
    char read_buffer[65536];
    rapidjson::FileReadStream input_stream(file, read_buffer, sizeof(read_buffer));

    rapidjson::Document document;
    document.ParseStream(input_stream);
    fclose(file);

    // TODO: these all can be parsed in parallel since they are processed
    // individually first

    // still need to figure out what is the best way of building the scene
    for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
         itr != document.MemberEnd(); ++itr)
    {
        if (strcmp(itr->name.GetString(), "scene") == 0)
        {
            std::string name = "default";
            std::string integrator = "default";
            std::string sampler = "default";
            std::string camera = "default";
            std::string medium = "default";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = (value["name"].GetString());
            }
            if (value.HasMember("integrator"))
            {
                integrator = (value["integrator"].GetString());
            }
            if (value.HasMember("sampler"))
            {
                sampler = (value["sampler"].GetString());
            }
            if (value.HasMember("camera"))
            {
                camera = (value["camera"].GetString());
            }
            if (value.HasMember("medium"))
            {
                medium = (value["medium"].GetString());
            }

            FeignRenderer::fr_scene(name,
                                    integrator,
                                    sampler,
                                    camera,
                                    medium);
        }
        else if (strcmp(itr->name.GetString(), "integrator") == 0)
        {
            std::string name = "integrator";
            std::string type = "normal";
            long max_time = -1;
            long max_heuristic = -1;

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }
            if (value.HasMember("max_time"))
            {
                max_time = value["max_time"].GetInt();
            }
            if (value.HasMember("max_heuristic"))
            {
                max_heuristic = value["max_heuristic"].GetInt();
            }

            FeignRenderer::fr_integrator(name,
                                         type,
                                         max_time,
                                         max_heuristic);
        }
        else if (strcmp(itr->name.GetString(), "sampler") == 0)
        {
            std::string name = "sampler";
            std::string type = "independent";
            int spp = 8;
            long seed = 0x12345678;
            long seed2 = 0x968574132;

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }
            if (value.HasMember("max_time"))
            {
                spp = value["spp"].GetInt();
            }
            if (value.HasMember("max_heuristic"))
            {
                seed = value["seed"].GetInt();
            }
            if (value.HasMember("max_heuristic"))
            {
                seed2 = value["seed2"].GetInt();
            }

            FeignRenderer::fr_sampler(name,
                                      type,
                                      spp,
                                      seed,
                                      seed2);
        }
        else if (strcmp(itr->name.GetString(), "camera") == 0)
        {
            std::string name = "camera";
            std::string type = "perspective";
            Vector3f origin = Vector3f(0.f, 0.f, 1.f);
            Vector3f target = Vector3f(0.f);
            Vector3f up = Vector3f(0.f, 1.f, 0.f);
            Vec2i image_res = Vec2i(512, 512);
            float fov = 45.f;

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }
            if (value.HasMember("origin"))
            {
                // TODO
            }
            if (value.HasMember("target"))
            {
                // TODO
            }
            if (value.HasMember("up"))
            {
                // TODO
            }
            if (value.HasMember("fov"))
            {
                fov = value["fov"].GetFloat();
            }
            if (value.HasMember("width"))
            {
                image_res[0] = value["width"].GetInt();
            }
            if (value.HasMember("height"))
            {
                image_res[1] = value["height"].GetInt();
            }

            FeignRenderer::fr_camera(name,
                                     type,
                                     origin,
                                     target,
                                     up,
                                     fov,
                                     image_res);
        }
        else if (strcmp(itr->name.GetString(), "object") == 0)
        {
            std::string name = "obj";
            std::string mesh = "default";
            std::string material = "default";

            const rapidjson::Value& value = itr->value;

            FeignRenderer::fr_clear_transform();

            for (rapidjson::Value::ConstMemberIterator itr_2 = value.MemberBegin();
                 itr_2 != document.MemberEnd(); ++itr_2)
            {
                const rapidjson::Value& value_2 = itr_2->value;

                if (strcmp(itr_2->name.GetString(), "name"))
                {
                    name = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "mesh"))
                {
                    mesh = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "material"))
                {
                    material = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "scale"))
                {
                    FeignRenderer::fr_scale(itr_2->value[0].GetFloat(),
                                            itr_2->value[1].GetFloat(),
                                            itr_2->value[2].GetFloat());
                }
                else if (strcmp(itr_2->name.GetString(), "translate"))
                {
                    FeignRenderer::fr_translate(itr_2->value[0].GetFloat(),
                                                itr_2->value[1].GetFloat(),
                                                itr_2->value[2].GetFloat());
                }
                else if (strcmp(itr_2->name.GetString(), "rotate"))
                {
                    FeignRenderer::fr_rotate(itr_2->value[0].GetFloat(),
                                             itr_2->value[1].GetFloat(),
                                             itr_2->value[2].GetFloat(),
                                             itr_2->value[3].GetFloat());
                }
            }

            FeignRenderer::fr_object(name,
                                     mesh,
                                     material);

            FeignRenderer::fr_clear_transform();
        }
        else if (strcmp(itr->name.GetString(), "mesh") == 0)
        {
            std::string name = "mesh";
            std::string type = "default";
            std::string filename = "default";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }
            if (value.HasMember("filename"))
            {
                filename = value["filename"].GetString();
            }

            FeignRenderer::fr_mesh(name, type, filename);
        }
        else if (strcmp(itr->name.GetString(), "emitter") == 0)
        {
            std::string name = "emitter";
            std::string type = "point";
            std::string mesh = "default";
            std::string material = "default";
            Vector3f pos = Vector3f(0.f);
            Color3f intensity = Color3f(1.f);

            const rapidjson::Value& value = itr->value;

            FeignRenderer::fr_clear_transform();

            for (rapidjson::Value::ConstMemberIterator itr_2 = value.MemberBegin();
                 itr_2 != document.MemberEnd(); ++itr_2)
            {
                const rapidjson::Value& value_2 = itr_2->value;

                if (strcmp(itr_2->name.GetString(), "name"))
                {
                    name = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "type"))
                {
                    type = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "mesh"))
                {
                    mesh = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "material"))
                {
                    material = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "pos"))
                {
                    pos[0] = itr_2->value[0].GetFloat();
                    pos[1] = itr_2->value[1].GetFloat();
                    pos[2] = itr_2->value[2].GetFloat();
                }
                else if (strcmp(itr_2->name.GetString(), "intensity"))
                {
                    intensity[0] = itr_2->value[0].GetFloat();
                    intensity[1] = itr_2->value[1].GetFloat();
                    intensity[2] = itr_2->value[2].GetFloat();
                }
                else if (strcmp(itr_2->name.GetString(), "scale"))
                {
                    FeignRenderer::fr_scale(itr_2->value[0].GetFloat(),
                                            itr_2->value[1].GetFloat(),
                                            itr_2->value[2].GetFloat());
                }
                else if (strcmp(itr_2->name.GetString(), "translate"))
                {
                    FeignRenderer::fr_translate(itr_2->value[0].GetFloat(),
                                                itr_2->value[1].GetFloat(),
                                                itr_2->value[2].GetFloat());
                }
                else if (strcmp(itr_2->name.GetString(), "rotate"))
                {
                    FeignRenderer::fr_rotate(itr_2->value[0].GetFloat(),
                                             itr_2->value[1].GetFloat(),
                                             itr_2->value[2].GetFloat(),
                                             itr_2->value[3].GetFloat());
                }
            }

            FeignRenderer::fr_emitter(name,
                                      type,
                                      mesh,
                                      material,
                                      pos,
                                      intensity);

            FeignRenderer::fr_clear_transform();
        }
        else if (strcmp(itr->name.GetString(), "material") == 0)
        {
            std::string name = "material";
            std::string bsdf = "default";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("bsdf"))
            {
                bsdf = value["bsdf"].GetString();
            }

            FeignRenderer::fr_material(name, bsdf);
        }
        else if (strcmp(itr->name.GetString(), "bsdf") == 0)
        {
            std::string name = "bsdf";
            std::string type = "diffuse";
            Color3f albedo = Color3f(1.f);

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }
            if (value.HasMember("albedo"))
            {
                albedo[0] = value["albedo"][0].GetFloat();
                albedo[1] = value["albedo"][1].GetFloat();
                albedo[2] = value["albedo"][2].GetFloat();
            }

            FeignRenderer::fr_bsdf(name, type, albedo);
        }
        else
        {
            assert(false);
            // error
        }
    }

    FeignRenderer::flush_renders();
}
