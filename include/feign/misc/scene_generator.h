#pragma once

// This class contains a collection of methods to automatically create the test
// scenes for this renderer

#include <feign/common.h>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>

class SceneGenerator
{
public:
    static void create_all_scenes()
    {
        create_ajax_normals_scene();
        create_ajax_diffuse_scene();
    }

    static void create_ajax_normals_scene()
    {
        rapidjson::Document document;
        document.SetObject();

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        rapidjson::Value scene(rapidjson::kObjectType);
        {
            scene.AddMember("name", "ajax_normals", allocator);

            rapidjson::Value sampler(rapidjson::kObjectType);
            {
                sampler.AddMember("type", "independent", allocator);
                sampler.AddMember("sample_count", 256, allocator);
                scene.AddMember("sampler", sampler, allocator);
            }

            rapidjson::Value integrator(rapidjson::kObjectType);
            {
                integrator.AddMember("type", "normals", allocator);
                scene.AddMember("integrator", integrator, allocator);
            }

            rapidjson::Value camera(rapidjson::kObjectType);
            {
                camera.AddMember("type", "perspective", allocator);
                {
                    rapidjson::Value array(rapidjson::kArrayType);
                    array.PushBack(-65.6055, allocator).PushBack(47.5762, allocator).PushBack(24.3583, allocator);
                    array.PushBack(-64.8161, allocator).PushBack(47.2211, allocator).PushBack(23.8576, allocator);
                    array.PushBack(0.299858, allocator).PushBack(0.934836, allocator).PushBack(-0.190177, allocator);
                    camera.AddMember("lookat", array, allocator);
                }
                camera.AddMember("fov", 30.0, allocator);
                camera.AddMember("width", 768, allocator);
                camera.AddMember("height", 768, allocator);
                scene.AddMember("camera", camera, allocator);
            }

            rapidjson::Value floor(rapidjson::kObjectType);
            {
                floor.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                floor.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    floor.AddMember("bsdf", bsdf, allocator);
                }
                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    scale_array.PushBack(100.0, allocator).PushBack(1.0, allocator).PushBack(100.0, allocator);
                    transform.AddMember("scale", scale_array, allocator);
                    floor.AddMember("transform", transform, allocator);
                }
                scene.AddMember("object", floor, allocator);
            }

            rapidjson::Value ajax(rapidjson::kObjectType);
            {
                ajax.AddMember("filename", "../scenes/meshes/ajax.obj", allocator);
                ajax.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    ajax.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", ajax, allocator);
            }

            document.AddMember("scene", scene, allocator);
        }

        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        document.Accept(writer);

        std::string str = sb.GetString();

        std::ofstream output;
        output.open("../scenes/generated/ajax-normals.json");
        output << str << std::endl;
        output.close();
    }

    static void create_ajax_diffuse_scene()
    {
        rapidjson::Document document;
        document.SetObject();

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        rapidjson::Value scene(rapidjson::kObjectType);
        {
            scene.AddMember("name", "ajax_diffuse", allocator);

            rapidjson::Value sampler(rapidjson::kObjectType);
            {
                sampler.AddMember("type", "independent", allocator);
                sampler.AddMember("sample_count", 256, allocator);
                scene.AddMember("sampler", sampler, allocator);
            }

            rapidjson::Value integrator(rapidjson::kObjectType);
            {
                integrator.AddMember("type", "whitted", allocator);
                scene.AddMember("integrator", integrator, allocator);
            }

            rapidjson::Value camera(rapidjson::kObjectType);
            {
                camera.AddMember("type", "perspective", allocator);
                {
                    rapidjson::Value array(rapidjson::kArrayType);
                    array.PushBack(-65.6055, allocator).PushBack(47.5762, allocator).PushBack(24.3583, allocator);
                    array.PushBack(-64.8161, allocator).PushBack(47.2211, allocator).PushBack(23.8576, allocator);
                    array.PushBack(0.299858, allocator).PushBack(0.934836, allocator).PushBack(-0.190177, allocator);
                    camera.AddMember("lookat", array, allocator);
                }
                camera.AddMember("fov", 30.0, allocator);
                camera.AddMember("width", 768, allocator);
                camera.AddMember("height", 768, allocator);
                scene.AddMember("camera", camera, allocator);
            }

            rapidjson::Value floor(rapidjson::kObjectType);
            {
                floor.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                floor.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    floor.AddMember("bsdf", bsdf, allocator);
                }
                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    scale_array.PushBack(100.0, allocator).PushBack(1.0, allocator).PushBack(100.0, allocator);
                    transform.AddMember("scale", scale_array, allocator);
                    floor.AddMember("transform", transform, allocator);
                }
                scene.AddMember("object", floor, allocator);
            }

            rapidjson::Value ajax(rapidjson::kObjectType);
            {
                ajax.AddMember("filename", "../scenes/meshes/ajax.obj", allocator);
                ajax.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    ajax.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", ajax, allocator);
            }

            rapidjson::Value point_one(rapidjson::kObjectType);
            {
                point_one.AddMember("type", "point", allocator);
                rapidjson::Value I(rapidjson::kArrayType);
                {
                    I.PushBack(2.25e4, allocator).PushBack(3e4, allocator).PushBack(3.75e4, allocator);
                }
                point_one.AddMember("I", I, allocator);
                rapidjson::Value pos(rapidjson::kArrayType);
                {
                    pos.PushBack(-20, allocator).PushBack(40, allocator).PushBack(20, allocator);
                }
                point_one.AddMember("pos", pos, allocator);
                scene.AddMember("emitter", point_one, allocator);
            }

            rapidjson::Value point_two(rapidjson::kObjectType);
            {
                point_two.AddMember("type", "point", allocator);
                rapidjson::Value I(rapidjson::kArrayType);
                {
                    I.PushBack(4e4, allocator).PushBack(3e4, allocator).PushBack(2e4, allocator);
                }
                point_two.AddMember("I", I, allocator);
                rapidjson::Value pos(rapidjson::kArrayType);
                {
                    pos.PushBack(-80, allocator).PushBack(80, allocator).PushBack(-10, allocator);
                }
                point_two.AddMember("pos", pos, allocator);
                scene.AddMember("emitter", point_two, allocator);
            }

            document.AddMember("scene", scene, allocator);
        }

        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        document.Accept(writer);

        std::string str = sb.GetString();

        std::ofstream output;
        output.open("../scenes/generated/ajax-diffuse.json");
        output << str << std::endl;
        output.close();
    }
};
