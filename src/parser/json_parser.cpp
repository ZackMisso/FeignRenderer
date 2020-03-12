/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/parser/json_parser.h>
#include <feign/core/api.h>

#include <feign/shapes/objmesh.h>
#include <feign/shapes/grid_obj.h>

#include <feign/stats/clocker.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>

#include <cstdio>
#include <fstream>

void JsonParser::parse(std::string filename, Imagef& image)
{
    
}

void JsonParser::parse(std::string filename)
{
    #if CLOCKING
        Clocker::startClock("parse");
    #endif

    FILE* file = fopen(filename.c_str(), "r");
    char read_buffer[65536];
    rapidjson::FileReadStream input_stream(file, read_buffer, sizeof(read_buffer));

    LOG(filename);
    rapidjson::Document document;
    document.ParseStream(input_stream);
    fclose(file);

    // TODO: parallelize this

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
            std::string filter = "default";
            std::string location = "";
            long max_time = -1;
            long max_heuristic = -1;

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }
            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
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
            if (value.HasMember("location"))
            {
                location = value["location"].GetString();
            }

            // all integrators share the same set of params unless there are
            // future exceptions
            Integrator::Params params(max_time, max_heuristic, location);

            FeignRenderer::fr_integrator(name,
                                         type,
                                         filter,
                                         &params);
        }
        else if (strcmp(itr->name.GetString(), "sampler") == 0)
        {
            std::string name = "sampler";
            std::string type = "independent";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }

            if (type == "independent")
            {
                int spp = 8;
                long seed = 0x12345678;

                if (value.HasMember("sample_count"))
                {
                    spp = value["sample_count"].GetInt();
                }
                if (value.HasMember("seed"))
                {
                    seed = value["seed"].GetInt();
                }

                Independent::Params params(spp, seed);

                FeignRenderer::fr_sampler(name, type, &params);
            }
            else
            {
                throw new FeignRendererException("parsing for any sampling routine other than independent not supported in json");
            }
        }
        else if (strcmp(itr->name.GetString(), "camera") == 0)
        {
            LOG("parsing camera");

            std::string name = "camera";
            std::string type = "perspective";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }

            if (type == "perspective")
            {
                Vector3f origin = Vector3f(0.f, 1.f, 0.f);
                Vector3f target = Vector3f(0.f);
                Vector3f up = Vector3f(0.f, 1.f, 0.f);
                Vec2i image_res = Vec2i(512, 512);
                Float fov = 30.f;
                Float app_radius = 0.f;
                Float near = 1e-4f;
                Float far = 1e4f;
                Float focal_dist = 10.f;

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

                Perspective::Params params(origin,
                                           target,
                                           up,
                                           fov,
                                           near,
                                           far,
                                           focal_dist,
                                           app_radius,
                                           image_res);

                FeignRenderer::fr_camera(name,
                                         type,
                                         &params);
            }
            else
            {
                throw new FeignRendererException(type + " can not be parsed through json");
            }
        }
        else if (strcmp(itr->name.GetString(), "object") == 0)
        {
            LOG("parsing object");
            std::string name = "obj";
            std::string mesh = "default";
            std::string material_shader = "null";
            std::string emitter = "null";

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
                else if (strcmp(itr_2->name.GetString(), "material_shader") == 0)
                {
                    material_shader = value_2.GetString();
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
                else if (strcmp(itr_2->name.GetString(), "emitter") == 0)
                {
                    // assert(false);
                    emitter = value_2.GetString();
                }
            }

            FeignRenderer::fr_object(name,
                                     mesh,
                                     material_shader,
                                     emitter);

            FeignRenderer::fr_clear_transform();
        }
        else if (strcmp(itr->name.GetString(), "mesh") == 0)
        {
            LOG("parsing mesh");
            std::string name = "mesh";
            std::string type = "default";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }

            if (type == "triangle_mesh")
            {
                std::string filename = "default";
                std::string geom_shader = "default";
                bool flip_norms = false;

                if (value.HasMember("filename"))
                {
                    filename = value["filename"].GetString();
                }
                if (value.HasMember("geom_shader"))
                {
                    geom_shader = value["geom_shader"].GetString();
                }
                if (value.HasMember("flip_norms"))
                {
                    flip_norms = value["flip_norms"].GetBool();
                }

                ObjMesh::Params params(filename, geom_shader, flip_norms);

                FeignRenderer::fr_mesh(name, type, &params);
            }
            else if (type == "grid")
            {
                std::string texture = "default";
                std::string geom_shader = "default";
                Vec2i resolution = Vec2i(2, 2);

                if (value.HasMember("texture"))
                {
                    texture = value["texture"].GetString();
                }
                if (value.HasMember("geom_shader"))
                {
                    geom_shader = value["geom_shader"].GetString();
                }
                if (value.HasMember("resolution"))
                {
                    resolution = Vec2i(value["resolution"][0].GetInt(),
                                       value["resolution"][1].GetInt());
                }

                GridObj::Params params(texture,
                                       geom_shader,
                                       resolution);

                FeignRenderer::fr_mesh(name, type, &params);
            }
            else
            {
                throw new FeignRendererException(type + " mesh not recognized");
            }
        }
        else if (strcmp(itr->name.GetString(), "emitter") == 0)
        {
            // assert(false);
            LOG("parsing emitter");
            std::string name = "emitter";
            std::string type = "point";
            std::string mesh = "null";
            std::string material = "null";
            // Vector3f pos = Vector3f(0.f);
            // Color3f intensity = Color3f(1.f);

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }

            FeignRenderer::fr_clear_transform();

            for (rapidjson::Value::ConstMemberIterator itr_2 = value.MemberBegin();
                 itr_2 != value.MemberEnd(); ++itr_2)
            {
                const rapidjson::Value& value_2 = itr_2->value;

                if (strcmp(itr_2->name.GetString(), "scale") == 0)
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

            if (type == "point")
            {
                Vector3f pos = Vector3f(0.f);
                Color3f intensity = Color3f(1.f);

                if (value.HasMember("position"))
                {
                    pos[0] = value["position"][0].GetFloat();
                    pos[1] = value["position"][1].GetFloat();
                    pos[2] = value["position"][2].GetFloat();
                }
                if (value.HasMember("intensity"))
                {
                    intensity[0] = value["intensity"][0].GetFloat();
                    intensity[1] = value["intensity"][1].GetFloat();
                    intensity[2] = value["intensity"][2].GetFloat();
                }

                PointEmitter::Params params(intensity, pos);

                FeignRenderer::fr_emitter(name, type, &params);
            }
            else if (type == "mesh")
            {
                Color3f intensity = Color3f(1.f);

                if (value.HasMember("intensity"))
                {
                    intensity[0] = value["intensity"][0].GetFloat();
                    intensity[1] = value["intensity"][1].GetFloat();
                    intensity[2] = value["intensity"][2].GetFloat();
                }

                MeshEmitter::Params params(intensity);

                FeignRenderer::fr_emitter(name, type, &params);
            }
            else if (type == "directional")
            {
                Vector3f light_dir = Color3f(1.f, 0.f, 0.f);
                Color3f radiance = Color3f(1.f);

                if (value.HasMember("light_dir"))
                {
                    light_dir[0] = value["light_dir"][0].GetFloat();
                    light_dir[1] = value["light_dir"][1].GetFloat();
                    light_dir[2] = value["light_dir"][2].GetFloat();
                }
                if (value.HasMember("radiance"))
                {
                    radiance[0] = value["radiance"][0].GetFloat();
                    radiance[1] = value["radiance"][1].GetFloat();
                    radiance[2] = value["radiance"][2].GetFloat();
                }

                DirectionalEmitter::Params params(light_dir, radiance);

                FeignRenderer::fr_emitter(name, type, &params);
            }
            else if (type == "spot")
            {
                Point3f light_pos = Color3f(0.f);
                Vector3f light_dir = Color3f(1.f, 0.f, 0.f);
                Color3f radiance = Color3f(1.f);
                Float light_angle = 25.f;

                if (value.HasMember("light_pos"))
                {
                    light_pos[0] = value["light_pos"][0].GetFloat();
                    light_pos[1] = value["light_pos"][1].GetFloat();
                    light_pos[2] = value["light_pos"][2].GetFloat();
                }
                if (value.HasMember("light_dir"))
                {
                    light_dir[0] = value["light_dir"][0].GetFloat();
                    light_dir[1] = value["light_dir"][1].GetFloat();
                    light_dir[2] = value["light_dir"][2].GetFloat();
                }
                if (value.HasMember("radiance"))
                {
                    radiance[0] = value["radiance"][0].GetFloat();
                    radiance[1] = value["radiance"][1].GetFloat();
                    radiance[2] = value["radiance"][2].GetFloat();
                }
                if (value.HasMember("light_angle"))
                {
                    light_angle = value["light_angle"].GetFloat();
                }

                SpotLightEmitter::Params params(light_pos,
                                                light_dir,
                                                radiance,
                                                light_angle);

                FeignRenderer::fr_emitter(name, type, &params);
            }
            else
            {
                throw new FeignRendererException(type + " emitter not parsable yet");
            }

            FeignRenderer::fr_clear_transform();
        }
        else if (strcmp(itr->name.GetString(), "material") == 0)
        {
            LOG("parsing material");
            std::string name = "material";
            std::string type = "simple";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }

            if (type == "simple")
            {
                std::string bsdf = "default";

                if (value.HasMember("bsdf"))
                {
                    bsdf = value["bsdf"].GetString();
                }

                SimpleMaterial::Params params(bsdf);

                FeignRenderer::fr_material(name, type, &params);
            }
            else
            {
                throw new FeignRendererException(type + " material is not parsable yet");
            }
        }
        else if (strcmp(itr->name.GetString(), "bsdf") == 0)
        {
            LOG("parsing bsdf");
            std::string name = "bsdf";
            std::string type = "diffuse";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }

            if (type == "diffuse")
            {
                Color3f albedo = Color3f(1.0f);

                if (value.HasMember("albedo"))
                {
                    albedo[0] = value["albedo"][0].GetFloat();
                    albedo[1] = value["albedo"][1].GetFloat();
                    albedo[2] = value["albedo"][2].GetFloat();
                }

                Diffuse::Params params(albedo);

                FeignRenderer::fr_bsdf(name, type, &params);
            }
            else if (type == "mirror")
            {
                Color3f albedo = Color3f(1.f);

                if (value.HasMember("albedo"))
                {
                    albedo[0] = value["albedo"][0].GetFloat();
                    albedo[1] = value["albedo"][1].GetFloat();
                    albedo[2] = value["albedo"][2].GetFloat();
                }

                Mirror::Params params(albedo);

                FeignRenderer::fr_bsdf(name, type, &params);
            }
            else if (type == "dielectric")
            {
                Float i_ior = 1.0;
                Float e_ior = 1.0;

                if (value.HasMember("int_ior"))
                {
                    i_ior = value["int_ior"].GetFloat();
                }
                if (value.HasMember("ext_ior"))
                {
                    e_ior = value["ext_ior"].GetFloat();
                }

                Dielectric::Params params(i_ior, e_ior);

                FeignRenderer::fr_bsdf(name, type, &params);
            }
            else if (type == "phong")
            {
                Color3f kd = Color3f(0.5f, 0.5f, 0.5f);
                Float ks = 0.5f;
                Float exponent = 2.f;

                if (value.HasMember("kd"))
                {
                    kd[0] = value["kd"][0].GetFloat();
                    kd[1] = value["kd"][1].GetFloat();
                    kd[2] = value["kd"][2].GetFloat();
                }
                if (value.HasMember("ks"))
                {
                    ks = value["ks"].GetFloat();
                }
                if (value.HasMember("exponent"))
                {
                    exponent = value["exponent"].GetFloat();
                }

                Phong::Params params(kd, ks, exponent);

                FeignRenderer::fr_bsdf(name, type, &params);
            }
            else
            {
                throw new FeignRendererException(type + " bsdf is not parsable yet");
            }
        }
        else if (strcmp(itr->name.GetString(), "shader") == 0)
        {
            LOG("parsing shader");
            std::string name = "shader";
            std::string type = "none";

            const rapidjson::Value& value = itr->value;

            if (value.HasMember("name"))
            {
                name = value["name"].GetString();
            }
            if (value.HasMember("type"))
            {
                type = value["type"].GetString();
            }

            if (type == "interp_verts_to_sphere")
            {
                float prop = 0.f;
                float proxy = 0.f;

                if (value.HasMember("prop_of_minor_axis"))
                {
                    prop = value["prop_of_minor_axis"].GetFloat();
                }
                if (value.HasMember("proxy"))
                {
                    proxy = value["proxy"].GetFloat();
                }

                InterpVertsToSphereShader::Params params(prop, proxy);

                FeignRenderer::fr_shader(name, type, &params);
            }
            else if (type == "simple_material")
            {
                std::string material = "defualt";

                if (value.HasMember("material"))
                {
                    material = value["material"].GetString();
                }

                SimpleMaterial::Params params(material);

                FeignRenderer::fr_shader(name, type, &params);
            }
            else
            {
                throw new FeignRendererException(type + " shader is not parsable yet");
            }
        }
        else
        {
            assert(false);
        }
    }

    #if CLOCKING
        Clocker::endClock("parse");
    #endif

    FeignRenderer::flush_renders();
}
