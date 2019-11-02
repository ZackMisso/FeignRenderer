/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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
        // create_ajax_normals_scene();
        create_ajax_normals_scene_2();
        // create_ajax_diffuse_scene();
        // create_ajax_amb_occ_scene();
        // create_cbox_whitted_scene();
    }

    static void create_ajax_normals_scene_2()
    {
        rapidjson::Document document;
        document.SetObject();

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        rapidjson::Value scene(rapidjson::kObjectType);
        {
            scene.AddMember("name", "ajax_normals", allocator);
            scene.AddMember("integrator", "integrator", allocator);
            scene.AddMember("sampler", "sampler", allocator);
            scene.AddMember("camera", "camera", allocator);
            document.AddMember("scene", scene, allocator);
        }

        rapidjson::Value integrator(rapidjson::kObjectType);
        {
            integrator.AddMember("name", "integrator", allocator);
            integrator.AddMember("type", "normals", allocator);
            document.AddMember("integrator", integrator, allocator);
        }

        rapidjson::Value sampler(rapidjson::kObjectType);
        {
            sampler.AddMember("name", "sampler", allocator);
            sampler.AddMember("type", "independent", allocator);
            sampler.AddMember("sample_count", 16, allocator);
            document.AddMember("sampler", sampler, allocator);
        }

        rapidjson::Value camera(rapidjson::kObjectType);
        {
            camera.AddMember("name", "camera", allocator);
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
            document.AddMember("camera", camera, allocator);
        }

        rapidjson::Value floor(rapidjson::kObjectType);
        {
            floor.AddMember("name", "floor", allocator);
            floor.AddMember("mesh", "plane", allocator);
            floor.AddMember("material", "floor_material", allocator);
            rapidjson::Value scale_array(rapidjson::kArrayType);
            scale_array.PushBack(100.0, allocator).PushBack(1.0, allocator).PushBack(100.0, allocator);
            floor.AddMember("scale", scale_array, allocator);
            document.AddMember("object", floor, allocator);
        }

        rapidjson::Value floor_mesh(rapidjson::kObjectType);
        {
            floor_mesh.AddMember("name", "plane", allocator);
            floor_mesh.AddMember("type", "triangle_mesh", allocator);
            floor_mesh.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
            document.AddMember("mesh", floor_mesh, allocator);
        }

        rapidjson::Value ajax(rapidjson::kObjectType);
        {
            ajax.AddMember("name", "ajax", allocator);
            ajax.AddMember("mesh", "ajax_data", allocator);
            document.AddMember("object", ajax, allocator);
        }

        rapidjson::Value ajax_obj(rapidjson::kObjectType);
        {
            ajax_obj.AddMember("name", "ajax_obj", allocator);
            ajax_obj.AddMember("type", "triangle_mesh", allocator);
            ajax_obj.AddMember("filename", "../scenes/meshes/ajax.obj", allocator);
            document.AddMember("mesh", ajax_obj, allocator);
        }

        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        document.Accept(writer);

        std::string str = sb.GetString();

        std::ofstream output;
        output.open("../scenes/generated/ajax-normals-new-format.json");
        output << str << std::endl;
        output.close();
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

    static void create_ajax_amb_occ_scene()
    {
        rapidjson::Document document;
        document.SetObject();

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        rapidjson::Value scene(rapidjson::kObjectType);
        {
            scene.AddMember("name", "ajax_amb_occ", allocator);

            rapidjson::Value sampler(rapidjson::kObjectType);
            {
                sampler.AddMember("type", "independent", allocator);
                sampler.AddMember("sample_count", 1024, allocator);
                scene.AddMember("sampler", sampler, allocator);
            }

            rapidjson::Value integrator(rapidjson::kObjectType);
            {
                integrator.AddMember("type", "amb_occ", allocator);
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
        output.open("../scenes/generated/ajax-amb_occ.json");
        output << str << std::endl;
        output.close();
    }

    static void create_cbox_whitted_scene()
    {
        rapidjson::Document document;
        document.SetObject();

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        rapidjson::Value scene(rapidjson::kObjectType);
        {
            scene.AddMember("name", "cbox_whitted", allocator);

            rapidjson::Value sampler(rapidjson::kObjectType);
            {
                sampler.AddMember("type", "independent", allocator);
                sampler.AddMember("sample_count", 512, allocator);
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
                    array.PushBack(0.0, allocator).PushBack(0.919769, allocator).PushBack(5.41159, allocator);
                    array.PushBack(0.0, allocator).PushBack(0.893051, allocator).PushBack(4.41198, allocator);
                    array.PushBack(0.0, allocator).PushBack(1.0, allocator).PushBack(0.0, allocator);
                    camera.AddMember("lookat", array, allocator);
                }
                camera.AddMember("fov", 27.7856, allocator);
                camera.AddMember("width", 600, allocator);
                camera.AddMember("height", 800, allocator);
                scene.AddMember("camera", camera, allocator);
            }

            rapidjson::Value walls(rapidjson::kObjectType);
            {
                walls.AddMember("filename", "../scenes/meshes/walls.obj", allocator);
                walls.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    {
                        rapidjson::Value albedo(rapidjson::kArrayType);
                        albedo.PushBack(0.725, allocator).PushBack(0.71, allocator).PushBack(0.68, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }
                    walls.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", walls, allocator);
            }

            rapidjson::Value right_wall(rapidjson::kObjectType);
            {
                right_wall.AddMember("filename", "../scenes/meshes/rightwall.obj", allocator);
                right_wall.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    {
                        rapidjson::Value albedo(rapidjson::kArrayType);
                        albedo.PushBack(0.161, allocator).PushBack(0.133, allocator).PushBack(0.427, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }
                    right_wall.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", right_wall, allocator);
            }

            rapidjson::Value left_wall(rapidjson::kObjectType);
            {
                left_wall.AddMember("filename", "../scenes/meshes/leftwall.obj", allocator);
                left_wall.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    {
                        rapidjson::Value albedo(rapidjson::kArrayType);
                        albedo.PushBack(0.630, allocator).PushBack(0.065, allocator).PushBack(0.050, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }
                    left_wall.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", left_wall, allocator);
            }

            rapidjson::Value sphere_one(rapidjson::kObjectType);
            {
                sphere_one.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                sphere_one.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "mirror", allocator);
                    sphere_one.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", sphere_one, allocator);
            }

            rapidjson::Value sphere_two(rapidjson::kObjectType);
            {
                sphere_two.AddMember("filename", "../scenes/meshes/sphere2.obj", allocator);
                sphere_two.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "mirror", allocator);
                    sphere_two.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", sphere_two, allocator);
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
                    I.PushBack(20.0, allocator).PushBack(20.0, allocator).PushBack(20.0, allocator);
                }
                point_one.AddMember("I", I, allocator);
                rapidjson::Value pos(rapidjson::kArrayType);
                {
                    pos.PushBack(0.0, allocator).PushBack(1.0, allocator).PushBack(1.0, allocator);
                }
                point_one.AddMember("pos", pos, allocator);
                scene.AddMember("emitter", point_one, allocator);
            }

            document.AddMember("scene", scene, allocator);
        }

        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        document.Accept(writer);

        std::string str = sb.GetString();

        std::ofstream output;
        output.open("../scenes/generated/cbox-whitted.json");
        output << str << std::endl;
        output.close();
    }

    static void create_sphere_direct()
    {
        rapidjson::Document document;
        document.SetObject();

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        rapidjson::Value scene(rapidjson::kObjectType);
        {
            scene.AddMember("name", "sphere_direct", allocator);

            rapidjson::Value sampler(rapidjson::kObjectType);
            {
                sampler.AddMember("type", "independent", allocator);
                sampler.AddMember("sample_count", 256, allocator);
                scene.AddMember("sampler", sampler, allocator);
            }

            rapidjson::Value integrator(rapidjson::kObjectType);
            {
                integrator.AddMember("type", "direct", allocator);
                scene.AddMember("integrator", integrator, allocator);
            }

            rapidjson::Value emitter(rapidjson::kObjectType);
            {
                emitter.AddMember("type", "distant", allocator);
                rapidjson::Value radiance(rapidjson::kArrayType);
                {
                    radiance.PushBack(20.0, allocator).PushBack(20.0, allocator).PushBack(20.0, allocator);
                }
                emitter.AddMember("radiance", radiance, allocator);
                emitter.AddMember("thetaA", 180, allocator);
                scene.AddMember("emitter", emitter, allocator);
            }

            rapidjson::Value camera(rapidjson::kObjectType);
            {
                camera.AddMember("type", "perspective", allocator);

                rapidjson::Value array(rapidjson::kArrayType);
                array.PushBack(5.0, allocator).PushBack(5.0, allocator).PushBack(1.0, allocator);
                array.PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(0.0, allocator);
                array.PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(1.0, allocator);
                camera.AddMember("lookat", array, allocator);

                camera.AddMember("fov", 40.0, allocator);
                camera.AddMember("width", 512, allocator);
                camera.AddMember("height", 512, allocator);
                scene.AddMember("camera", camera, allocator);
            }

            rapidjson::Value sphere(rapidjson::kObjectType);
            {
                sphere.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                sphere.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(1.0, allocator).PushBack(1.0, allocator).PushBack(1.0, allocator);
                    }
                    bsdf.AddMember("albedo", albedo, allocator);
                    sphere.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", sphere, allocator);
            }

            rapidjson::Value floor(rapidjson::kObjectType);
            {
                floor.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                floor.AddMember("type", "mesh", allocator);

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);
                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(1.0, allocator).PushBack(1.0, allocator).PushBack(1.0, allocator);
                    }
                    bsdf.AddMember("albedo", albedo, allocator);
                    floor.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    scale_array.PushBack(10.0, allocator).PushBack(1.0, allocator).PushBack(10.0, allocator);
                    transform.AddMember("scale", scale_array, allocator);
                    rapidjson::Value rotate_array(rapidjson::kArrayType);
                    rotate_array.PushBack(90.0, allocator).PushBack(1.0, allocator).PushBack(0.0, allocator).PushBack(0.0, allocator);
                    transform.AddMember("rotate", rotate_array, allocator);
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    translate_array.PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(-1.0, allocator);
                    transform.AddMember("translate", translate_array, allocator);
                    floor.AddMember("transform", transform, allocator);
                }

                scene.AddMember("object", floor, allocator);
            }

            document.AddMember("scene", scene, allocator);
        }

        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        document.Accept(writer);

        std::string str = sb.GetString();

        std::ofstream output;
        output.open("../scenes/generated/sphere_direct.json");
        output << str << std::endl;
        output.close();
    }

    static void create_spherical()
    {
        rapidjson::Document document;
        document.SetObject();

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        rapidjson::Value scene(rapidjson::kObjectType);
        {
            scene.AddMember("name", "spherical", allocator);

            rapidjson::Value sampler(rapidjson::kObjectType);
            {
                sampler.AddMember("type", "independent", allocator);
                sampler.AddMember("sampler_count", 256, allocator);
                scene.AddMember("sampler", sampler, allocator);
            }

            rapidjson::Value integrator(rapidjson::kObjectType);
            {
                integrator.AddMember("type", "path", allocator);
                scene.AddMember("integrator", integrator, allocator);
            }

            rapidjson::Value camera(rapidjson::kObjectType);
            {
                camera.AddMember("type", "perspective", allocator);

                rapidjson::Value array(rapidjson::kArrayType);
                array.PushBack(8.0, allocator).PushBack(8.0, allocator).PushBack(0.4, allocator); // origin
                array.PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(1.0, allocator); // target
                array.PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(1.0, allocator); // up
                camera.AddMember("lookat", array, allocator);

                camera.AddMember("fov", 70.0, allocator);
                camera.AddMember("width", 1920, allocator);
                camera.AddMember("height", 1200, allocator);
                scene.AddMember("camera", camera, allocator);
            }

            rapidjson::Value sphere_1(rapidjson::kObjectType);
            {
                sphere_1.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                sphere_1.AddMember("type", "mesh", allocator);
                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "dielectric", allocator);
                    sphere_1.AddMember("bsdf", bsdf, allocator);
                }
                scene.AddMember("object", sphere_1, allocator);
            }

            rapidjson::Value sphere_2(rapidjson::kObjectType);
            {
                sphere_2.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                sphere_2.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(1.0, allocator).PushBack(1.0, allocator).PushBack(1.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(3.75, allocator).PushBack(0.0, allocator).PushBack(0.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    sphere_2.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "blinn", allocator);
                    bsdf.AddMember("ks", 0.3, allocator);
                    rapidjson::Value kd(rapidjson::kArrayType);
                    {
                        kd.PushBack(0.175, allocator).PushBack(0.225, allocator).PushBack(0.325, allocator);
                        bsdf.AddMember("kd", kd, allocator);
                    }
                    bsdf.AddMember("e", 100.0, allocator);
                    sphere_2.AddMember("bsdf", bsdf, allocator);
                }

                scene.AddMember("object", sphere_2, allocator);
            }

            rapidjson::Value sphere_3(rapidjson::kObjectType);
            {
                sphere_3.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                sphere_3.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(1.0, allocator).PushBack(1.0, allocator).PushBack(1.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(0.0, allocator).PushBack(3.75, allocator).PushBack(0.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    sphere_3.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "blinn", allocator);
                    bsdf.AddMember("ks", 0.3, allocator);
                    rapidjson::Value kd(rapidjson::kArrayType);
                    {
                        kd.PushBack(0.35, allocator).PushBack(0.4, allocator).PushBack(0.3, allocator);
                        bsdf.AddMember("kd", kd, allocator);
                    }
                    bsdf.AddMember("e", 100.0, allocator);
                    sphere_3.AddMember("bsdf", bsdf, allocator);
                }

                scene.AddMember("object", sphere_3, allocator);
            }

            rapidjson::Value sphere_4(rapidjson::kObjectType);
            {
                sphere_4.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                sphere_4.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(2.0, allocator).PushBack(2.0, allocator).PushBack(2.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(2.0, allocator).PushBack(-6.0, allocator).PushBack(1.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    sphere_4.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "dielectric", allocator);
                    sphere_4.AddMember("bsdf", bsdf, allocator);
                }

                scene.AddMember("object", sphere_4, allocator);
            }

            rapidjson::Value sphere_5(rapidjson::kObjectType);
            {
                sphere_5.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                sphere_5.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(2.0, allocator).PushBack(2.0, allocator).PushBack(2.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(-6.0, allocator).PushBack(2.0, allocator).PushBack(1.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    sphere_5.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "dielectric", allocator);
                    sphere_5.AddMember("bsdf", bsdf, allocator);
                }

                scene.AddMember("object", sphere_5, allocator);
            }

            rapidjson::Value floor(rapidjson::kObjectType);
            {
                floor.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                floor.AddMember("type", "mesh", allocator);

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "dielectric", allocator);
                    floor.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(10.0, allocator).PushBack(1.0, allocator).PushBack(10.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value rotate_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(90.0, allocator).PushBack(1.0, allocator).PushBack(0.0, allocator).PushBack(0.0, allocator);
                        transform.AddMember("rotate", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(-1.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    floor.AddMember("transform", transform, allocator);
                }

                scene.AddMember("object", floor, allocator);
            }

            rapidjson::Value ceiling(rapidjson::kObjectType);
            {
                ceiling.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                ceiling.AddMember("type", "mesh", allocator);

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "mirror", allocator);
                    ceiling.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(10.0, allocator).PushBack(1.0, allocator).PushBack(10.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value rotate_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(270.0, allocator).PushBack(1.0, allocator).PushBack(0.0, allocator).PushBack(0.0, allocator);
                        transform.AddMember("rotate", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(8.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    ceiling.AddMember("transform", transform, allocator);
                }

                scene.AddMember("object", ceiling, allocator);
            }

            rapidjson::Value wall_1(rapidjson::kObjectType);
            {
                wall_1.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                wall_1.AddMember("type", "mesh", allocator);

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "mirror", allocator);
                    wall_1.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(7.0, allocator).PushBack(1.0, allocator).PushBack(10.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value rotate_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(90.0, allocator).PushBack(0.0, allocator).PushBack(1.0, allocator).PushBack(0.0, allocator);
                        transform.AddMember("rotate", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(0.0, allocator).PushBack(-10.0, allocator).PushBack(6.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    wall_1.AddMember("transform", transform, allocator);
                }

                scene.AddMember("object", wall_1, allocator);
            }

            rapidjson::Value wall_2(rapidjson::kObjectType);
            {
                wall_2.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                wall_2.AddMember("type", "mesh", allocator);

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "mirror", allocator);
                    wall_2.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(7.0, allocator).PushBack(1.0, allocator).PushBack(10.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value rotate_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(90.0, allocator).PushBack(0.0, allocator).PushBack(1.0, allocator).PushBack(0.0, allocator);
                        transform.AddMember("rotate", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(0.0, allocator).PushBack(10.0, allocator).PushBack(6.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    wall_2.AddMember("transform", transform, allocator);
                }

                scene.AddMember("object", wall_2, allocator);
            }

            rapidjson::Value wall_3(rapidjson::kObjectType);
            {
                wall_3.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                wall_3.AddMember("type", "mesh", allocator);

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "mirror", allocator);
                    wall_3.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(7.0, allocator).PushBack(1.0, allocator).PushBack(10.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value rotate_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(-90.0, allocator).PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(-1.0, allocator);
                        transform.AddMember("rotate", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(10.0, allocator).PushBack(0.0, allocator).PushBack(6.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    wall_3.AddMember("transform", transform, allocator);
                }

                scene.AddMember("object", wall_3, allocator);
            }

            rapidjson::Value wall_4(rapidjson::kObjectType);
            {
                wall_4.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
                wall_4.AddMember("type", "mesh", allocator);

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "mirror", allocator);
                    wall_4.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(7.0, allocator).PushBack(1.0, allocator).PushBack(10.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value rotate_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(-90.0, allocator).PushBack(0.0, allocator).PushBack(0.0, allocator).PushBack(-1.0, allocator);
                        transform.AddMember("rotate", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(-10.0, allocator).PushBack(0.0, allocator).PushBack(6.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    wall_4.AddMember("transform", transform, allocator);
                }

                scene.AddMember("object", wall_4, allocator);
            }

            rapidjson::Value holder_1(rapidjson::kObjectType);
            {
                holder_1.AddMember("filename", "../scenes/meshes/cube.obj", allocator);
                holder_1.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(2.0, allocator).PushBack(2.0, allocator).PushBack(6.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(8.0, allocator).PushBack(-8.0, allocator).PushBack(2.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    holder_1.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);

                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(0.2, allocator).PushBack(0.2, allocator).PushBack(0.2, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }

                    holder_1.AddMember("bsdf", bsdf, allocator);
                }

                scene.AddMember("object", holder_1, allocator);
            }

            rapidjson::Value light_1(rapidjson::kObjectType);
            {
                light_1.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                light_1.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(1.0, allocator).PushBack(1.0, allocator).PushBack(1.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(8.0, allocator).PushBack(-8.0, allocator).PushBack(2.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    light_1.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);

                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(0.2, allocator).PushBack(0.2, allocator).PushBack(0.2, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }

                    light_1.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value emitter(rapidjson::kArrayType);
                {
                    emitter.AddMember("type", "area", allocator);

                    rapidjson::Value radiance(rapidjson::kArrayType);
                    {
                        radiance.PushBack(0.0, allocator).PushBack(4.0, allocator).PushBack(4.0, allocator);
                        emitter.AddMember("radiance", radiance, allocator);
                    }

                    light_1.AddMember("emitter", emitter, allocator);
                }

                scene.AddMember("object", light_1, allocator);
            }

            rapidjson::Value holder_2(rapidjson::kObjectType);
            {
                holder_2.AddMember("filename", "../scenes/meshes/cube.obj", allocator);
                holder_2.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(2.0, allocator).PushBack(2.0, allocator).PushBack(6.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(-8.0, allocator).PushBack(8.0, allocator).PushBack(2.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    holder_2.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);

                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(0.2, allocator).PushBack(0.2, allocator).PushBack(0.2, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }

                    holder_2.AddMember("bsdf", bsdf, allocator);
                }

                scene.AddMember("object", holder_2, allocator);
            }

            rapidjson::Value light_2(rapidjson::kObjectType);
            {
                light_2.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                light_2.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(1.0, allocator).PushBack(1.0, allocator).PushBack(1.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(-8.0, allocator).PushBack(8.0, allocator).PushBack(2.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    light_2.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);

                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(0.2, allocator).PushBack(0.2, allocator).PushBack(0.2, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }

                    light_2.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value emitter(rapidjson::kArrayType);
                {
                    emitter.AddMember("type", "area", allocator);

                    rapidjson::Value radiance(rapidjson::kArrayType);
                    {
                        radiance.PushBack(4.0, allocator).PushBack(4.0, allocator).PushBack(0.0, allocator);
                        emitter.AddMember("radiance", radiance, allocator);
                    }

                    light_2.AddMember("emitter", emitter, allocator);
                }

                scene.AddMember("object", light_2, allocator);
            }

            rapidjson::Value holder_3(rapidjson::kObjectType);
            {
                holder_3.AddMember("filename", "../scenes/meshes/cube.obj", allocator);
                holder_3.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(2.0, allocator).PushBack(2.0, allocator).PushBack(6.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(-8.0, allocator).PushBack(-8.0, allocator).PushBack(2.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    holder_3.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);

                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(0.2, allocator).PushBack(0.2, allocator).PushBack(0.2, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }

                    holder_3.AddMember("bsdf", bsdf, allocator);
                }

                scene.AddMember("object", holder_3, allocator);
            }

            rapidjson::Value light_3(rapidjson::kObjectType);
            {
                light_3.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                light_3.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(1.0, allocator).PushBack(1.0, allocator).PushBack(1.0, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(-8.0, allocator).PushBack(-8.0, allocator).PushBack(2.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    light_3.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);

                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(0.2, allocator).PushBack(0.2, allocator).PushBack(0.2, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }

                    light_3.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value emitter(rapidjson::kArrayType);
                {
                    emitter.AddMember("type", "area", allocator);

                    rapidjson::Value radiance(rapidjson::kArrayType);
                    {
                        radiance.PushBack(4.0, allocator).PushBack(4.0, allocator).PushBack(4.0, allocator);
                        emitter.AddMember("radiance", radiance, allocator);
                    }

                    light_3.AddMember("emitter", emitter, allocator);
                }

                scene.AddMember("object", light_3, allocator);
            }

            rapidjson::Value light_4(rapidjson::kObjectType);
            {
                light_4.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                light_4.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(5.0, allocator).PushBack(0.1, allocator).PushBack(0.1, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(3.0, allocator).PushBack(-8.6, allocator).PushBack(13.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    light_4.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);

                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(0.2, allocator).PushBack(0.2, allocator).PushBack(0.2, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }

                    light_4.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value emitter(rapidjson::kArrayType);
                {
                    emitter.AddMember("type", "area", allocator);

                    rapidjson::Value radiance(rapidjson::kArrayType);
                    {
                        radiance.PushBack(0.0, allocator).PushBack(2.0, allocator).PushBack(0.0, allocator);
                        emitter.AddMember("radiance", radiance, allocator);
                    }

                    light_4.AddMember("emitter", emitter, allocator);
                }

                scene.AddMember("object", light_4, allocator);
            }

            rapidjson::Value light_5(rapidjson::kObjectType);
            {
                light_5.AddMember("filename", "../scenes/meshes/sphere1.obj", allocator);
                light_5.AddMember("type", "mesh", allocator);

                rapidjson::Value transform(rapidjson::kObjectType);
                {
                    transform.AddMember("name", "toWorld", allocator);
                    rapidjson::Value scale_array(rapidjson::kArrayType);
                    {
                        scale_array.PushBack(0.1, allocator).PushBack(5.0, allocator).PushBack(0.1, allocator);
                        transform.AddMember("scale", scale_array, allocator);
                    }
                    rapidjson::Value translate_array(rapidjson::kArrayType);
                    {
                        translate_array.PushBack(-8.6, allocator).PushBack(-3.0, allocator).PushBack(13.0, allocator);
                        transform.AddMember("translate", translate_array, allocator);
                    }
                    light_5.AddMember("transform", transform, allocator);
                }

                rapidjson::Value bsdf(rapidjson::kObjectType);
                {
                    bsdf.AddMember("type", "diffuse", allocator);

                    rapidjson::Value albedo(rapidjson::kArrayType);
                    {
                        albedo.PushBack(0.2, allocator).PushBack(0.2, allocator).PushBack(0.2, allocator);
                        bsdf.AddMember("albedo", albedo, allocator);
                    }

                    light_5.AddMember("bsdf", bsdf, allocator);
                }

                rapidjson::Value emitter(rapidjson::kArrayType);
                {
                    emitter.AddMember("type", "area", allocator);

                    rapidjson::Value radiance(rapidjson::kArrayType);
                    {
                        radiance.PushBack(0.0, allocator).PushBack(2.0, allocator).PushBack(0.0, allocator);
                        emitter.AddMember("radiance", radiance, allocator);
                    }

                    light_5.AddMember("emitter", emitter, allocator);
                }

                scene.AddMember("object", light_5, allocator);
            }

            document.AddMember("scene", scene, allocator);
        }

        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        document.Accept(writer);

        std::string str = sb.GetString();

        std::ofstream output;
        output.open("../scenes/generated/spherical.json");
        output << str << std::endl;
        output.close();
    }
};
