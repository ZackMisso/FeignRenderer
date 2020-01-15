/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <tests/json_test.h>

#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/filereadstream.h>

#include <cstdio>
#include <fstream>

using namespace rapidjson;

JsonTestData::JsonTestData()
{
    // TODO
}

void JsonTestData::logReport() const
{
    // TODO
}

JsonTest::JsonTest()
{
    testing = false;
}

bool JsonTest::evaluateTest(UnitTestData& testLog) const
{
    if (testing)
    {
        std::cout << "running rapidjson's tutorial script" << std::endl;
        jsonTutorial();
        return false;
    }

    if (!createBaseSceneTest())
    {
        std::cout << "json test: create base scene test failed" << std::endl;
        return false;
    }

    if (!readBaseSceneTest())
    {
        std::cout << "json test: read base scene test failed" << std::endl;
        return false;
    }

    return true;
}

void JsonTest::jsonTutorial() const
{
    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
    printf("Original JSON:\n %s\n", json);

    Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

#if 0
    // "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
    if (document.Parse(json).HasParseError())
        return;
#else
    // In-situ parsing, decode strings directly in the source string. Source must be string.
    char buffer[sizeof(json)];
    memcpy(buffer, json, sizeof(json));
    if (document.ParseInsitu(buffer).HasParseError())
        return;
#endif

    printf("\nParsing to document succeeded.\n");

    ////////////////////////////////////////////////////////////////////////////
    // 2. Access values in document.

    printf("\nAccess values in document:\n");
    assert(document.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.

    assert(document.HasMember("hello"));
    assert(document["hello"].IsString());
    printf("hello = %s\n", document["hello"].GetString());

    // Since version 0.2, you can use single lookup to check the existing of member and its value:
    Value::MemberIterator hello = document.FindMember("hello");
    assert(hello != document.MemberEnd());
    assert(hello->value.IsString());
    assert(strcmp("world", hello->value.GetString()) == 0);
    (void)hello;

    assert(document["t"].IsBool());     // JSON true/false are bool. Can also uses more specific function IsTrue().
    printf("t = %s\n", document["t"].GetBool() ? "true" : "false");

    assert(document["f"].IsBool());
    printf("f = %s\n", document["f"].GetBool() ? "true" : "false");

    printf("n = %s\n", document["n"].IsNull() ? "null" : "?");

    assert(document["i"].IsNumber());   // Number is a JSON type, but C++ needs more specific type.
    assert(document["i"].IsInt());      // In this case, IsUint()/IsInt64()/IsUInt64() also return true.
    printf("i = %d\n", document["i"].GetInt()); // Alternative (int)document["i"]

    assert(document["pi"].IsNumber());
    assert(document["pi"].IsDouble());
    printf("pi = %g\n", document["pi"].GetDouble());

    {
        const Value& a = document["a"]; // Using a reference for consecutive access is handy and faster.
        assert(a.IsArray());
        for (SizeType i = 0; i < a.Size(); i++) // rapidjson uses SizeType instead of size_t.
            printf("a[%d] = %d\n", i, a[i].GetInt());

        int y = a[0].GetInt();
        (void)y;

        // Iterating array with iterators
        printf("a = ");
        for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
            printf("%d ", itr->GetInt());
        printf("\n");
    }

    // Iterating object members
    static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
    for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
        printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);

    ////////////////////////////////////////////////////////////////////////////
    // 3. Modify values in document.

    // Change i to a bigger number
    {
        uint64_t f20 = 1;   // compute factorial of 20
        for (uint64_t j = 1; j <= 20; j++)
            f20 *= j;
        document["i"] = f20;    // Alternate form: document["i"].SetUint64(f20)
        assert(!document["i"].IsInt()); // No longer can be cast as int or uint.
    }

    // Adding values to array.
    {
        Value& a = document["a"];   // This time we uses non-const reference.
        Document::AllocatorType& allocator = document.GetAllocator();
        for (int i = 5; i <= 10; i++)
            a.PushBack(i, allocator);   // May look a bit strange, allocator is needed for potentially realloc. We normally uses the document's.

        // Fluent API
        a.PushBack("Lua", allocator).PushBack("Mio", allocator);
    }

    // Making string values.

    // This version of SetString() just store the pointer to the string.
    // So it is for literal and string that exists within value's life-cycle.
    {
        document["hello"] = "rapidjson";    // This will invoke strlen()
        // Faster version:
        // document["hello"].SetString("rapidjson", 9);
    }

    // This version of SetString() needs an allocator, which means it will allocate a new buffer and copy the the string into the buffer.
    Value author;
    {
        char buffer2[10];
        int len = sprintf(buffer2, "%s %s", "Milo", "Yip");  // synthetic example of dynamically created string.

        author.SetString(buffer2, static_cast<SizeType>(len), document.GetAllocator());
        // Shorter but slower version:
        // document["hello"].SetString(buffer, document.GetAllocator());

        // Constructor version:
        // Value author(buffer, len, document.GetAllocator());
        // Value author(buffer, document.GetAllocator());
        memset(buffer2, 0, sizeof(buffer2)); // For demonstration purpose.
    }
    // Variable 'buffer' is unusable now but 'author' has already made a copy.
    document.AddMember("author", author, document.GetAllocator());

    assert(author.IsNull());        // Move semantic for assignment. After this variable is assigned as a member, the variable becomes null.

    ////////////////////////////////////////////////////////////////////////////
    // 4. Stringify JSON

    printf("\nModified JSON with reformatting:\n");
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    document.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    puts(sb.GetString());
    std::cout << "Take 2" << std::endl;
    std::cout << sb.GetString() << std::endl;
}

// TODO: create autogeneratable scenes
bool JsonTest::createBaseSceneTest() const
{
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    rapidjson::Value scene(rapidjson::kObjectType);
    {
        scene.AddMember("name", "new_parser", allocator);

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
            // rapidjson::Value transform(rapidjson::kObjectType);
            // {
            //     transform.AddMember("name", "toWorld", allocator);
                rapidjson::Value array(rapidjson::kArrayType);
                array.PushBack(-65.6055, allocator).PushBack(47.5762, allocator).PushBack(24.3583, allocator);
                array.PushBack(-64.8161, allocator).PushBack(47.2211, allocator).PushBack(23.8576, allocator);
                array.PushBack(0.299858, allocator).PushBack(0.934836, allocator).PushBack(-0.190177, allocator);
                // transform.AddMember("lookat", array, allocator);
                camera.AddMember("lookat", array, allocator);
            // }
            camera.AddMember("fov", 30.0, allocator);
            camera.AddMember("width", 768, allocator);
            camera.AddMember("height", 768, allocator);
            scene.AddMember("camera", camera, allocator);
        }

        rapidjson::Value floor(rapidjson::kObjectType);
        {
            floor.AddMember("filename", "../scenes/meshes/plane.obj", allocator);
            floor.AddMember("type", "mesh", allocator);
            rapidjson::Value bsdf(kObjectType);
            {
                bsdf.AddMember("type", "diffuse", allocator);
                floor.AddMember("bsdf", bsdf, allocator);
            }
            rapidjson::Value transform(kObjectType);
            {
                transform.AddMember("name", "toWorld", allocator);
                rapidjson::Value scale_array(kArrayType);
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
            rapidjson::Value bsdf(kObjectType);
            {
                bsdf.AddMember("type", "diffuse", allocator);
                ajax.AddMember("bsdf", bsdf, allocator);
            }
            scene.AddMember("object", ajax, allocator);
        }

        document.AddMember("scene", scene, allocator);
    }

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    document.Accept(writer);

    std::string str = sb.GetString();

    std::ofstream output;
    output.open("../scenes/ajax-normals-generated.json");
    output << str << std::endl;
    output.close();

    return true;
}

///////////////// Dummy Structs for Testing //////////////////////
struct DummyIntegrator
{
    std::string type = "";
};

struct DummySampler
{
    std::string type = "";
    int sample_count = 0;
};

// struct DummyMatrix
// {
//
// };

struct DummyTransform
{
    std::string name;
    Float one = 0.0;
    Float two = 0.0;
    Float three = 0.0;
    Float four = 0.0;
    Float five = 0.0;
    Float six = 0.0;
    Float seven = 0.0;
    Float eight = 0.0;
    Float nine = 0.0;
};

struct DummyCamera
{
    ~DummyCamera()
    {
        delete transform;
    }
    DummyTransform* transform = nullptr;
    std::string type = "";
    Float fov = 0.0;
    int height = 0;
    int width = 0;
};

struct DummyBSDF
{
    std::string type = "";
};

struct DummyMesh
{
    ~DummyMesh()
    {
        delete transform;
        delete bsdf;
    }

    DummyTransform* transform = nullptr;
    DummyBSDF* bsdf = nullptr;
    std::string filename = "";
};

struct DummyScene
{
    ~DummyScene()
    {
        delete mesh_one;
        delete mesh_two;
        delete integrator;
        delete camera;
        delete sampler;
    }

    DummyMesh* mesh_one = nullptr;
    DummyMesh* mesh_two = nullptr;
    DummyIntegrator* integrator = nullptr;
    DummyCamera* camera = nullptr;
    DummySampler* sampler = nullptr;
};
//////////////////////////////////////////////////////////////////

bool JsonTest::readBaseSceneTest() const
{
    FILE* fp = fopen("../scenes/ajax-normals-generated.json", "rs"); // non-Windows use "r"
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document document;
    document.ParseStream(is);
    fclose(fp);

    DummyScene* scene = nullptr;

    for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
         itr != document.MemberEnd(); ++itr)
    {
        static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
        if (std::string(itr->name.GetString()) == "scene")
        {
            if (scene)
            {
                std::cout << "error: only one scene should exist" << std::endl;
                return false;
            }

            scene = new DummyScene();

            for (rapidjson::Value::ConstMemberIterator scene_itr = itr->value.MemberBegin();
                 scene_itr != itr->value.MemberEnd(); ++scene_itr)
            {
                if (std::string(scene_itr->name.GetString()) == "object" &&
                    std::string(scene_itr->value["type"].GetString()) == "mesh")
                {
                    if (scene->mesh_one && scene->mesh_two)
                    {
                        std::cout << "error: there should only be two meshes" << std::endl;
                        return false;
                    }

                    DummyMesh* mesh = nullptr;

                    if (!scene->mesh_one)
                    {
                        scene->mesh_one = new DummyMesh();
                        mesh = scene->mesh_one;
                    }
                    else
                    {
                        scene->mesh_two = new DummyMesh();
                        mesh = scene->mesh_two;
                    }

                    for (rapidjson::Value::ConstMemberIterator mesh_itr = scene_itr->value.MemberBegin();
                         mesh_itr != scene_itr->value.MemberEnd(); ++mesh_itr)
                    {
                        if (std::string(mesh_itr->name.GetString()) == "filename")
                        {
                            mesh->filename = std::string(mesh_itr->value.GetString());
                        }

                        else if (std::string(mesh_itr->name.GetString()) == "transform")
                        {
                            if (mesh->transform)
                            {
                                std::cout << "error: mesh is supposed to have only one transform" << std::endl;
                                return false;
                            }

                            mesh->transform = new DummyTransform();

                            for (rapidjson::Value::ConstMemberIterator trans_itr = mesh_itr->value.MemberBegin();
                                 trans_itr != mesh_itr->value.MemberEnd(); ++trans_itr)
                            {
                                if (std::string(trans_itr->name.GetString()) == "name")
                                {
                                    mesh->transform->name = trans_itr->value.GetString();
                                }

                                else if (std::string(trans_itr->name.GetString()) == "scale")
                                {
                                    const Value& array = trans_itr->value;
                                    assert(array.IsArray());
                                    assert(array.Size() == 3);

                                    mesh->transform->one = array[0].GetFloat();
                                    mesh->transform->two = 0.0;
                                    mesh->transform->three = 0.0;
                                    mesh->transform->four = 0.0;
                                    mesh->transform->five = array[1].GetFloat();
                                    mesh->transform->six = 0.0;
                                    mesh->transform->seven = 0.0;
                                    mesh->transform->eight = 0.0;
                                    mesh->transform->nine = array[2].GetFloat();
                                }

                                else
                                {
                                    std::cout << "error: transform is not supposed to have another parameter" << std::endl;
                                    return false;
                                }
                            }
                        }

                        else if (std::string(mesh_itr->name.GetString()) == "bsdf")
                        {
                            if (mesh->bsdf)
                            {
                                std::cout << "error: mesh is supposed to have only one bsdf" << std::endl;
                                return false;
                            }

                            mesh->bsdf = new DummyBSDF();

                            for (rapidjson::Value::ConstMemberIterator bsdf_itr = mesh_itr->value.MemberBegin();
                                 bsdf_itr != mesh_itr->value.MemberEnd(); ++bsdf_itr)
                            {
                                if (std::string(bsdf_itr->name.GetString()) == "type")
                                {
                                    mesh->bsdf->type = std::string(bsdf_itr->value.GetString());
                                }

                                else
                                {
                                    std::cout << "error: scene integrator should not have any other params" << std::endl;
                                    return false;
                                }
                            }
                        }
                        else if (std::string(mesh_itr->name.GetString()) == "type")
                        {
                            // does nothing
                        }
                        else
                        {
                            std::cout << "error: mesh should not have any other params" << std::endl;
                            return false;
                        }
                    }
                }

                else if (std::string(scene_itr->name.GetString()) == "integrator")
                {
                    if (scene->integrator)
                    {
                        std::cout << "error: scene should only have one integrator" << std::endl;
                        return false;
                    }

                    scene->integrator = new DummyIntegrator();

                    for (rapidjson::Value::ConstMemberIterator inte_itr = scene_itr->value.MemberBegin();
                         inte_itr != scene_itr->value.MemberEnd(); ++inte_itr)
                    {
                        if (std::string(inte_itr->name.GetString()) == "type")
                        {
                            scene->integrator->type = std::string(inte_itr->value.GetString());
                        }

                        else
                        {
                            std::cout << "error: scene integrator should not have any other params" << std::endl;
                            return false;
                        }
                    }
                }

                else if (std::string(scene_itr->name.GetString()) == "sampler")
                {
                    if (scene->sampler)
                    {
                        std::cout << "error: scene should only have one sampler" << std::endl;
                        return false;
                    }

                    scene->sampler = new DummySampler();

                    for (rapidjson::Value::ConstMemberIterator samp_itr = scene_itr->value.MemberBegin();
                         samp_itr != scene_itr->value.MemberEnd(); ++samp_itr)
                    {
                        if (std::string(samp_itr->name.GetString()) == "type")
                        {
                            scene->sampler->type = std::string(samp_itr->value.GetString());
                        }

                        else if (std::string(samp_itr->name.GetString()) == "sample_count")
                        {
                            scene->sampler->sample_count = samp_itr->value.GetInt();
                        }

                        else
                        {
                            std::cout << "error: scene sampler should not have any other params" << std::endl;
                            return false;
                        }
                    }
                }

                else if (std::string(scene_itr->name.GetString()) == "camera")
                {
                    if (scene->camera)
                    {
                        std::cout << "error: scene should only have one camera" << std::endl;
                        return false;
                    }

                    scene->camera = new DummyCamera();

                    for (rapidjson::Value::ConstMemberIterator cam_itr = scene_itr->value.MemberBegin();
                         cam_itr != scene_itr->value.MemberEnd(); ++cam_itr)
                    {
                        if (std::string(cam_itr->name.GetString()) == "type")
                        {
                            scene->camera->type = std::string(cam_itr->value.GetString());
                        }

                        else if (std::string(cam_itr->name.GetString()) == "fov")
                        {
                            scene->camera->fov = cam_itr->value.GetFloat();
                        }

                        else if (std::string(cam_itr->name.GetString()) == "width")
                        {
                            scene->camera->width = cam_itr->value.GetInt();
                        }

                        else if (std::string(cam_itr->name.GetString()) == "height")
                        {
                            scene->camera->height = cam_itr->value.GetInt();
                        }

                        else if (std::string(cam_itr->name.GetString()) == "lookat")
                        {
                            if (scene->camera->transform)
                            {
                                std::cout << "error: camera is supposed to have only one transform" << std::endl;
                                return false;
                            }

                            scene->camera->transform = new DummyTransform();

                            // for (rapidjson::Value::ConstMemberIterator trans_itr = cam_itr->value.MemberBegin();
                            //      trans_itr != cam_itr->value.MemberEnd(); ++trans_itr)
                            // {
                            //     if (std::string(trans_itr->name.GetString()) == "name")
                            //     {
                            //         scene->camera->transform->name = trans_itr->value.GetString();
                            //     }

                                // else if (std::string(trans_itr->name.GetString()) == "lookat")
                                // {
                            const Value& array = cam_itr->value;
                            assert(array.IsArray());
                            assert(array.Size() == 9);

                            scene->camera->transform->one = array[0].GetFloat();
                            scene->camera->transform->two = array[1].GetFloat();
                            scene->camera->transform->three = array[2].GetFloat();
                            scene->camera->transform->four = array[3].GetFloat();
                            scene->camera->transform->five = array[4].GetFloat();
                            scene->camera->transform->six = array[5].GetFloat();
                            scene->camera->transform->seven = array[6].GetFloat();
                            scene->camera->transform->eight = array[7].GetFloat();
                            scene->camera->transform->nine = array[8].GetFloat();
                            //     }
                            //
                            //     else
                            //     {
                            //         std::cout << "error: transform is not supposed to have another parameter" << std::endl;
                            //         return false;
                            //     }
                            // }
                        }

                        else
                        {
                            std::cout << "error: scene integrator should not have any other params" << std::endl;
                            return false;
                        }
                    }
                }
                else if (std::string(scene_itr->name.GetString()) == "name")
                {
                    // does nothing
                }
                else
                {
                    std::cout << "error: no other objects should exist in the scene" << std::endl;
                    return false;
                }
            }
        }
    }

    if (!scene->mesh_one)
    {
        std::cout << "mesh_one not initialized" << std::endl;
        return false;
    }
    if (scene->mesh_one->filename != "../scenes/meshes/plane.obj")
    {
        std::cout << "mesh_one filename" << std::endl;
        return false;
    }
    if (scene->mesh_one->bsdf->type != "diffuse")
    {
        std::cout << "mesh_one diffuse: " << scene->mesh_one->bsdf->type << std::endl;
        return false;
    }
    if (!scene->mesh_one->transform)
    {
        std::cout << "mesh_one transform not initialized" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->one != 100.0)
    {
        std::cout << "mesh_one one: " << scene->mesh_one->transform->one << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->two != 0.0)
    {
        std::cout << "mesh_one two" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->three != 0.0)
    {
        std::cout << "mesh_one three" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->four != 0.0)
    {
        std::cout << "mesh_one four" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->five != 1.0)
    {
        std::cout << "mesh_one five" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->six != 0.0)
    {
        std::cout << "mesh_one six" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->seven != 0.0)
    {
        std::cout << "mesh_one seven" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->eight != 0.0)
    {
        std::cout << "mesh_one eight" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->nine != 100.0)
    {
        std::cout << "mesh_one nine" << std::endl;
        return false;
    }
    if (scene->mesh_one->transform->name != "toWorld")
    {
        std::cout << "mesh_one transform name: " << scene->mesh_one->transform->name << std::endl;
        return false;
    }
    if (!scene->mesh_one->bsdf)
    {
        std::cout << "mesh_one bsdf not initialized" << std::endl;
        return false;
    }
    if (scene->mesh_one->bsdf->type != "diffuse")
    {
        std::cout << "mesh_one bsdf type" << std::endl;
        return false;
    }
    if (!scene->mesh_two)
    {
        std::cout << "mesh_two not initialized" << std::endl;
        return false;
    }
    if (scene->mesh_two->filename != "../scenes/meshes/ajax.obj")
    {
        std::cout << "mesh_two filename" << std::endl;
        return false;
    }
    if (!scene->mesh_two->bsdf)
    {
        std::cout << "mesh_two not initialized" << std::endl;
        return false;
    }
    if (scene->mesh_two->bsdf->type != "diffuse")
    {
        std::cout << "mesh_two bsdf type" << std::endl;
        return false;
    }
    if (!scene->integrator)
    {
        std::cout << "integrator not initialized" << std::endl;
        return false;
    }
    if (scene->integrator->type != "normals")
    {
        std::cout << "integrator type" << std::endl;
        return false;
    }
    if (!scene->camera)
    {
        std::cout << "camera not initialized" << std::endl;
        return false;
    }
    if (!scene->camera->transform)
    {
        std::cout << "camera transform not initialized" << std::endl;
        return false;
    }
    if ((scene->camera->transform->four - -64.8161) > 1e-4)
    {
        std::cout << "camera one: " << scene->camera->transform->one << std::endl;
        return false;
    }
    if ((scene->camera->transform->five - 47.2211) > 1e-4)
    {
        std::cout << "camera two" << std::endl;
        return false;
    }
    if ((scene->camera->transform->six - 23.8576) > 1e-4)
    {
        std::cout << "camera three" << std::endl;
        return false;
    }
    if ((scene->camera->transform->one - -65.6055) > 1e-4)
    {
        std::cout << "camera four" << std::endl;
        return false;
    }
    if ((scene->camera->transform->two - 47.5762) > 1e-4)
    {
        std::cout << "camera five" << std::endl;
        return false;
    }
    if ((scene->camera->transform->three - 24.3583) > 1e-4)
    {
        std::cout << "camera six" << std::endl;
        return false;
    }
    if ((scene->camera->transform->seven - 0.299858) > 1e-4)
    {
        std::cout << "camera seven" << std::endl;
        return false;
    }
    if ((scene->camera->transform->eight - 0.934836) > 1e-4)
    {
        std::cout << "camera eight" << std::endl;
        return false;
    }
    if ((scene->camera->transform->nine - -0.190177) > 1e-4)
    {
        std::cout << "camera nine" << std::endl;
        return false;
    }
    // if (scene->camera->transform->name != "toWorld")
    // {
    //     std::cout << "camera transform name" << std::endl;
    //     return false;
    // }
    if (!scene->sampler)
    {
        std::cout << "sampler not initialized" << std::endl;
        return false;
    }
    if (scene->sampler->type != "independent")
    {
        std::cout << "sampler type: " << scene->sampler->type << std::endl;
        return false;
    }
    if (scene->sampler->sample_count != 256)
    {
        std::cout << "sampler sample_count" << std::endl;
        return false;
    }

    // TODO: create scripts to automatically generate all scenes

    delete scene;

    return true;
}
