#include <feign/parser/json_parser.h>
#include <feign/api/api.h>

#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/filereadstream.h>

#include <cstdio>
#include <fstream>

// TODO: rewrite scene description document

void JsonParser::parseTransform(const rapidjson::Value& value)
{
    for (rapidjson::Value::ConstMemberIterator itr = value.MemberBegin();
         itr != value.MemberEnd(); ++itr)
    {
        if (strcmp(itr->name.GetString(), "scale") == 0)
        {
            Vec3f scale = Vec3f(itr->value[0].GetFloat(),
                                itr->value[1].GetFloat(),
                                itr->value[2].GetFloat());

            FeignRenderer::transform_scale(scale);
        }
        else if (strcmp(itr->name.GetString(), "rot") == 0)
        {
            Float degree = itr->value[0].GetFloat();

            Vec3f axis = Vec3f(itr->value[1].GetFloat(),
                               itr->value[2].GetFloat(),
                               itr->value[3].GetFloat());

            FeignRenderer::transform_rot(degree, axis);
        }
        else if (strcmp(itr->name.GetString(), "trans") == 0)
        {
            Vec3f translate(itr->value[0].GetFloat(),
                            itr->value[1].GetFloat(),
                            itr->value[2].GetFloat());

            FeignRenderer::transform_trans(translate);
        }
        else if (strcmp(itr->name.GetString(), "matrix") == 0)
        {
            FeignRenderer::transform_matrix(itr->value[0].GetFloat(),
                                            itr->value[1].GetFloat(),
                                            itr->value[2].GetFloat(),
                                            itr->value[3].GetFloat(),
                                            itr->value[4].GetFloat(),
                                            itr->value[5].GetFloat(),
                                            itr->value[6].GetFloat(),
                                            itr->value[7].GetFloat(),
                                            itr->value[8].GetFloat(),
                                            itr->value[9].GetFloat(),
                                            itr->value[10].GetFloat(),
                                            itr->value[11].GetFloat(),
                                            itr->value[12].GetFloat(),
                                            itr->value[13].GetFloat(),
                                            itr->value[14].GetFloat(),
                                            itr->value[15].GetFloat());
        }
        else if (strcmp(itr->name.GetString(), "lookat") == 0)
        {
            // TODO: decide later if this should be supported here or be directly
            // a parameter of the camera

            Vec3f eye = Vec3f(itr->value[0].GetFloat(),
                              itr->value[1].GetFloat(),
                              itr->value[2].GetFloat());

            Vec3f tar = Vec3f(itr->value[0].GetFloat(),
                              itr->value[1].GetFloat(),
                              itr->value[2].GetFloat());

            Vec3f up = Vec3f(itr->value[0].GetFloat(),
                             itr->value[1].GetFloat(),
                             itr->value[2].GetFloat());

            FeignRenderer::transform_lookat(eye, tar, up);
        }
        else if (strcmp(itr->name.GetString(), "name") == 0)
        {
            // TODO: decide if local transforms should be cached, if so,
            // implement the necessary functionality to do so
        }
        else
        {
            std::cout << "unrecognized transform operation: " << itr->name.GetString() << std::endl;
        }
    }
}

void JsonParser::parseBSDF(const rapidjson::Value& value)
{
    const char* type = value["type"].GetString();

    if (strcmp(type, "diffuse") == 0)
    {
        if (value.HasMember("albedo"))
        {
            Color3f albedo = Color3f(value["albedo"][0].GetFloat(),
                                     value["albedo"][1].GetFloat(),
                                     value["albedo"][2].GetFloat());

            FeignRenderer::bsdf_diffuse(albedo);
        }
        else
        {
            FeignRenderer::bsdf_diffuse();
        }
    }
    else
    {
        std::cout << "unrecognized bsdf: " << type << std::endl;
    }
}

void JsonParser::parseSampler(const rapidjson::Value& value)
{
    const char* type = value["type"].GetString();
    int sample_cnt = 16;
    uint32_t seed = 0x13579bd;

    if (value.HasMember("sample_count"))
    {
        sample_cnt = value["sample_count"].GetInt();
    }
    if (value.HasMember("seed"))
    {
        seed = (uint32_t)value["seed"].GetInt();
    }

    // std::cout << "moo" << std::endl;

    if (strcmp(type, "independent") == 0)
    {
        // std::cout << "before api call" << std::endl;
        FeignRenderer::sampler_independent(sample_cnt, seed);
        // std::cout << "after api call" << std::endl;
    }
    else if (strcmp(type, "halton") == 0)
    {
        FeignRenderer::sampler_halton(sample_cnt, seed);
    }
    else if (strcmp(type, "latin") == 0)
    {
        FeignRenderer::sampler_latin(sample_cnt, seed);
    }
    else
    {
        std::cout << "unrecognized sampler: " << type << std::endl;
    }
}

void JsonParser::parseIntegrator(const rapidjson::Value& value)
{
    const char* type = value["type"].GetString();

    if (strcmp(type, "whitted") == 0)
    {
        FeignRenderer::integrator_whitted();
    }
    else if (strcmp(type, "path") == 0)
    {
        FeignRenderer::integrator_path_unidir();
    }
    else if (strcmp(type, "bidir") == 0)
    {
        FeignRenderer::integrator_path_bidir();
    }
    else if (strcmp(type, "light") == 0)
    {
        FeignRenderer::integrator_light_unidir();
    }
    else if (strcmp(type, "normals") == 0)
    {
        FeignRenderer::integrator_normal();
    }
    else
    {
        std::cout << "unrecognized integrator: " << type << std::endl;
    }
}

void JsonParser::parseEmitter(const rapidjson::Value& value)
{
    const char* type = value["type"].GetString();

    if (strcmp(type, "point") == 0)
    {
        Color3f I = Color3f(1.0);
        Point3f pos = Point3f(0.0);

        if (value.HasMember("I"))
        {
            I[0] = value["I"][0].GetFloat();
            I[1] = value["I"][1].GetFloat();
            I[2] = value["I"][2].GetFloat();
        }
        if (value.HasMember("pos"))
        {
            pos[0] = value["pos"][0].GetFloat();
            pos[1] = value["pos"][1].GetFloat();
            pos[2] = value["pos"][2].GetFloat();
        }

        FeignRenderer::emitter_point(I, pos);
    }
    else
    {
        std::cout << "unrecognized emitter: " << type << std::endl;
    }
}

void JsonParser::parseCamera(const rapidjson::Value& value)
{
    const char* type = value["type"].GetString();

    if (strcmp(type, "orthographic") == 0)
    {
        throw new NotImplementedException("api parseCamera ortho");
    }
    else if (strcmp(type, "perspective") == 0)
    {
        Float aperatureRadius = 0.0;
        Float focalDistance = 10.0;
        Float fov = 30.0;
        Float near = 1e-4;
        Float far = 1e4;
        uint32_t width = 1280;
        uint32_t height = 720;

        Vec3f eye = Vec3f(0.0, 0.0, -10.0);
        Vec3f at = Vec3f(0.0, 0.0, 0.0);
        Vec3f up = Vec3f(0.0, 1.0, 0.0);

        if (value.HasMember("lookat"))
        {
            eye = Vec3f(value["lookat"][0].GetFloat(),
                        value["lookat"][1].GetFloat(),
                        value["lookat"][2].GetFloat());

            at = Vec3f(value["lookat"][3].GetFloat(),
                       value["lookat"][4].GetFloat(),
                       value["lookat"][5].GetFloat());

            up = Vec3f(value["lookat"][6].GetFloat(),
                       value["lookat"][7].GetFloat(),
                       value["lookat"][8].GetFloat());

            // FeignRenderer::transform_lookat(eye, at, up);
        }
        if (value.HasMember("aperature_radius"))
        {
            aperatureRadius = value["aperature_radius"].GetFloat();
        }
        if (value.HasMember("focal_distance"))
        {
            focalDistance = value["focal_distance"].GetFloat();
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
        if (value.HasMember("width"))
        {
            width = (uint32_t)value["width"].GetInt();
        }
        if (value.HasMember("height"))
        {
            height = (uint32_t)value["height"].GetInt();
        }

        FeignRenderer::camera_perspective(eye,
                                          at,
                                          up,
                                          aperatureRadius,
                                          focalDistance,
                                          fov,
                                          near,
                                          far,
                                          width,
                                          height);
    }
    else
    {
        std::cout << "unrecognized camera: " << type << std::endl;
    }
}

void JsonParser::parseSceneObject(const rapidjson::Value& value)
{
    const char* type = value["type"].GetString();

    FeignRenderer::transform_push();

    // specific object logic
    if (strcmp(type, "mesh") == 0)
    {
        // TODO: add default obj file for the renderer
        std::string filename = "../scenes/meshes/cube.obj";

        if (value.HasMember("filename"))
        {
            filename = std::string(value["filename"].GetString());
        }

        bool flip_norms = false;

        if (value.HasMember("flip_norms"))
        {
            flip_norms = value["flip_norms"].GetBool();
        }

        // TODO: This current implementation still stores all vertex data separately
        // for each instance of every mesh.... not sure if this is the optimal choice.
        // it is computationally less expensive, but more expensive memory-wise.
        if (value.HasMember("transform"))
        {
            parseTransform(value["transform"]);
        }

        FeignRenderer::begin_obj(filename, flip_norms);

        if (value.HasMember("bsdf"))
        {
            parseBSDF(value["bsdf"]);
        }
    }
    else if (strcmp(type, "empty") == 0)
    {
        throw new NotImplementedException("parseSceneObject empty nodes");
    }
    else
    {
        std::cout << "unrecognized scene node type: " << type << std::endl;
        return;
    }

    // recurse logic
    // Note: currently only objects, scenes, or worlds may have children.
    for (rapidjson::Value::ConstMemberIterator itr = value.MemberBegin();
         itr != value.MemberEnd(); ++itr)
    {
        if (strcmp(itr->name.GetString(), "object") == 0)
        {
            parseSceneObject(itr->value);
        }
    }

    // ending logic
    if (strcmp(type, "mesh") == 0)
    {
        FeignRenderer::end_obj();
    }
    else if (strcmp(type, "empty") == 0)
    {
        FeignRenderer::end_node();
    }

    FeignRenderer::transform_pop();
}

void JsonParser::parseScene(const rapidjson::Value& value)
{
    std::string name = "test";
    if (value.HasMember("name"))
    {
        name = (value["name"].GetString());
    }
    FeignRenderer::begin_scene(name);

    // transform should be processed first
    if (value.HasMember("transform"))
    {
        parseTransform(value);
    }

    for (rapidjson::Value::ConstMemberIterator itr = value.MemberBegin();
         itr != value.MemberEnd(); ++itr)
    {
        // sampler
        if (strcmp(itr->name.GetString(), "sampler") == 0)
        {
            // std::cout << "parsing sampler" << std::endl;
            parseSampler(itr->value);
            // std::cout << "outside sampler" << std::endl;
        }
        // integrator
        else if (strcmp(itr->name.GetString(), "integrator") == 0)
        {
            // std::cout << "parsing integrator" << std::endl;
            parseIntegrator(itr->value);
        }
        // camera
        else if (strcmp(itr->name.GetString(), "camera") == 0)
        {
            // std::cout << "parsing camera" << std::endl;
            parseCamera(itr->value);
        }
        // object
        else if (strcmp(itr->name.GetString(), "object") == 0)
        {
            // std::cout << "parsing object" << std::endl;
            parseSceneObject(itr->value);
        }
        // emitter
        else if (strcmp(itr->name.GetString(), "emitter") == 0)
        {
            // std::cout << "parsing emitter" << std::endl;
            parseEmitter(itr->value);
        }
        else if (strcmp(itr->name.GetString(), "name") == 0)
        {
            // does nothing, but is an acceptable property
        }
        else
        {
            std::cout << "unknown scene property: " << itr->name.GetString() << std::endl;
        }
    }

    FeignRenderer::end_scene();
}

WorldNode* JsonParser::parse(std::string filename)
{
    FILE* file = fopen(filename.c_str(), "r");
    char read_buffer[65536];
    rapidjson::FileReadStream input_stream(file, read_buffer, sizeof(read_buffer));
    rapidjson::Document document;
    document.ParseStream(input_stream);
    fclose(file);

    FeignRenderer::begin_world();

    for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
         itr != document.MemberEnd(); ++itr)
    {
        if (strcmp(itr->name.GetString(), "scene") == 0)
        {
            parseScene(itr->value);
        }
        else
        {
            throw new UnrecognizedTokenException(std::string(itr->name.GetString()));
        }
    }

    return FeignRenderer::end_world();
}
