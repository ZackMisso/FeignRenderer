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
    // LOG("parsing: " + filename);
    FILE* file = fopen(filename.c_str(), "r");
    char read_buffer[65536];
    rapidjson::FileReadStream input_stream(file, read_buffer, sizeof(read_buffer));

    LOG(filename);
    rapidjson::Document document;
    document.ParseStream(input_stream);
    fclose(file);

    LOG("start full parse");

    // TODO: these all can be parsed in parallel since they are processed
    // individually first

    // still need to figure out what is the best way of building the scene
    for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
         itr != document.MemberEnd(); ++itr)
    {
        LOG("parse: ");
        if (strcmp(itr->name.GetString(), "scene") == 0)
        {
            LOG("parsing scene");

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

            // LOG("setting scene");
            // LOG("name: " + name);
            // LOG("integrator: " + integrator);
            // LOG("sampler: " + sampler);
            // LOG("camera: " + camera);
            // LOG("name: " + name);

            FeignRenderer::fr_scene(name,
                                    integrator,
                                    sampler,
                                    camera,
                                    medium);
        }
        else if (strcmp(itr->name.GetString(), "integrator") == 0)
        {
            LOG("parsing integrator");

            std::string name = "integrator";
            std::string type = "normal";
            std::string filter = "default";
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
            if (value.HasMember("filter"))
            {
                filter = value["filter"].GetString();
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
                                         filter,
                                         max_time,
                                         max_heuristic);
        }
        else if (strcmp(itr->name.GetString(), "sampler") == 0)
        {
            LOG("parsing sampler");

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
            if (value.HasMember("sample_count"))
            {
                spp = value["sample_count"].GetInt();
            }
            if (value.HasMember("seed"))
            {
                seed = value["seed"].GetInt();
            }
            if (value.HasMember("seed2"))
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
            LOG("parsing camera");

            std::string name = "camera";
            std::string type = "perspective";
            Vector3f origin = Vector3f(0.f, 1.f, 0.f);
            Vector3f target = Vector3f(0.f);
            Vector3f up = Vector3f(0.f, 1.f, 0.f);
            Vec2i image_res = Vec2i(512, 512);
            Float fov = 30.f;
            Float app_radius = 0.f;
            Float near = 1e-4f;
            Float far = 1e4f;
            Float focal_dist = 10.f;

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
                throw NotImplementedException("origin vector json_parser");
            }
            if (value.HasMember("target"))
            {
                throw NotImplementedException("target vector json_parser");
            }
            if (value.HasMember("up"))
            {
                throw NotImplementedException("up vector json_parser");
            }
            if (value.HasMember("fov"))
            {
                fov = value["fov"].GetFloat();
            }
            if (value.HasMember("near"))
            {
                near = value["near"].GetFloat();
            }
            if (value.HasMember("far"))
            {
                far = value["far"].GetFloat();
            }
            if (value.HasMember("app_radius"))
            {
                app_radius = value["app_radius"].GetFloat();
            }
            if (value.HasMember("focal_dist"))
            {
                focal_dist = value["focal_dist"].GetFloat();
            }
            if (value.HasMember("width"))
            {
                image_res[0] = value["width"].GetInt();
            }
            if (value.HasMember("height"))
            {
                image_res[1] = value["height"].GetInt();
            }
            if (value.HasMember("lookat"))
            {
                origin = Vector3f(value["lookat"][0].GetFloat(),
                                  value["lookat"][1].GetFloat(),
                                  value["lookat"][2].GetFloat());

                target = Vector3f(value["lookat"][3].GetFloat(),
                                  value["lookat"][4].GetFloat(),
                                  value["lookat"][5].GetFloat());

                up = Vector3f(value["lookat"][6].GetFloat(),
                              value["lookat"][7].GetFloat(),
                              value["lookat"][8].GetFloat());
            }

            FeignRenderer::fr_camera(name,
                                     type,
                                     origin,
                                     target,
                                     up,
                                     fov,
                                     near,
                                     far,
                                     focal_dist,
                                     app_radius,
                                     image_res);
        }
        else if (strcmp(itr->name.GetString(), "object") == 0)
        {
            std::string name = "obj";
            std::string mesh = "default";
            std::string material = "null";

            const rapidjson::Value& value = itr->value;

            FeignRenderer::fr_clear_transform();

            for (rapidjson::Value::ConstMemberIterator itr_2 = value.MemberBegin();
                 itr_2 != value.MemberEnd(); ++itr_2)
            {
                const rapidjson::Value& value_2 = itr_2->value;

                if (strcmp(itr_2->name.GetString(), "name") == 0)
                {
                    name = value_2.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "mesh") == 0)
                {
                    mesh = value_2.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "material") == 0)
                {
                    material = value_2.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "scale") == 0)
                {
                    FeignRenderer::fr_scale(value_2[0].GetFloat(),
                                            value_2[1].GetFloat(),
                                            value_2[2].GetFloat());
                }
                else if (strcmp(itr_2->name.GetString(), "translate") == 0)
                {
                    FeignRenderer::fr_translate(value_2[0].GetFloat(),
                                                value_2[1].GetFloat(),
                                                value_2[2].GetFloat());
                }
                else if (strcmp(itr_2->name.GetString(), "rotate") == 0)
                {
                    FeignRenderer::fr_rotate(value_2[0].GetFloat(),
                                             value_2[1].GetFloat(),
                                             value_2[2].GetFloat(),
                                             value_2[3].GetFloat());
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
            std::string geom_shader = "default";

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
            if (value.HasMember("geom_shader"))
            {
                geom_shader = value["geom_shader"].GetString();
            }

            FeignRenderer::fr_mesh(name, type, filename, geom_shader);
        }
        else if (strcmp(itr->name.GetString(), "emitter") == 0)
        {
            // LOG("emitter");
            std::string name = "emitter";
            std::string type = "point";
            std::string mesh = "null";
            std::string material = "null";
            Vector3f pos = Vector3f(0.f);
            Color3f intensity = Color3f(1.f);

            const rapidjson::Value& value = itr->value;

            FeignRenderer::fr_clear_transform();

            for (rapidjson::Value::ConstMemberIterator itr_2 = value.MemberBegin();
                 itr_2 != value.MemberEnd(); ++itr_2)
            {
                // LOG("emitter iter");
                const rapidjson::Value& value_2 = itr_2->value;

                if (strcmp(itr_2->name.GetString(), "name") == 0)
                {
                    name = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "type") == 0)
                {
                    type = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "mesh") == 0)
                {
                    mesh = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "material") == 0)
                {
                    material = itr_2->value.GetString();
                }
                else if (strcmp(itr_2->name.GetString(), "position") == 0)
                {
                    pos[0] = itr_2->value[0].GetFloat();
                    pos[1] = itr_2->value[1].GetFloat();
                    pos[2] = itr_2->value[2].GetFloat();
                }
                else if (strcmp(itr_2->name.GetString(), "intensity") == 0)
                {
                    // LOG("intensity");
                    intensity[0] = itr_2->value[0].GetFloat();
                    intensity[1] = itr_2->value[1].GetFloat();
                    intensity[2] = itr_2->value[2].GetFloat();
                }
                else if (strcmp(itr_2->name.GetString(), "scale") == 0)
                {
                    FeignRenderer::fr_scale(itr_2->value[0].GetFloat(),
                                            itr_2->value[1].GetFloat(),
                                            itr_2->value[2].GetFloat());
                }
                else if (strcmp(itr_2->name.GetString(), "translate") == 0)
                {
                    FeignRenderer::fr_translate(itr_2->value[0].GetFloat(),
                                                itr_2->value[1].GetFloat(),
                                                itr_2->value[2].GetFloat());
                }
                else if (strcmp(itr_2->name.GetString(), "rotate") == 0)
                {
                    FeignRenderer::fr_rotate(itr_2->value[0].GetFloat(),
                                             itr_2->value[1].GetFloat(),
                                             itr_2->value[2].GetFloat(),
                                             itr_2->value[3].GetFloat());
                }
            }

            LOG("fr_emitter");

            FeignRenderer::fr_emitter(name,
                                      type,
                                      mesh,
                                      material,
                                      pos,
                                      intensity);

            LOG("post emitter");

            FeignRenderer::fr_clear_transform();
        }
        else if (strcmp(itr->name.GetString(), "material") == 0)
        {
            LOG("material");
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
        else if (strcmp(itr->name.GetString(), "shader") == 0)
        {
            std::string name = "shader";
            std::string type = "none";
            float test_val = 0.f;

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }
            if (value.HasMember("test_value"))
            {
                test_val = value["test_value"].GetFloat();
            }

            FeignRenderer::fr_shader(name, type, test_val);
        }
        else
        {
            assert(false);
            // error
        }
    }

    FeignRenderer::flush_renders();
}
