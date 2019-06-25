#include <feign/parser/json_parser.h>

#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/filereadstream.h>

#include <cstdio>
#include <fstream>

// void JsonParser::possiblyAddChild(std::vector<Node*>& nodes,
//                                   Node* node)
// {
//     if (nodes.size() > 0)
//     {
//         nodes[nodes.size() - 1]->addChild(node);
//     }
//
//     nodes.push_back(node);
// }

Transform JsonParser::parseTransform(const rapidjson::Value& value)
{
    // TODO
    return Transform();
}

void JsonParser::parseBSDF(const rapidjson::Value& value,
                           Node* parent)
{
    // TODO
}

void JsonParser::parseSampler(const rapidjson::Value& value,
                              Node* parent)
{
    // TODO
}

void JsonParser::parseIntegrator(const rapidjson::Value& value,
                                 Node* parent)
{
    // TODO
}

void JsonParser::parseEmitter(const rapidjson::Value& value,
                              std::vector<Transform>& transform_stack,
                              Node* parent)
{
    // TODO
}

void JsonParser::parseCamera(const rapidjson::Value& value,
                             std::vector<Transform>& transform_stack,
                             Node* parent)
{
    // TODO
}

void JsonParser::parseSceneObject(const rapidjson::Value& value,
                                  std::vector<Transform>& transform_stack,
                                  Node* parent)
{
    // TODO
}

void JsonParser::parseScene(const rapidjson::Value& value,
                            std::vector<Transform>& transform_stack,
                            Node* parent)
{
    Scene* scene = new Scene();

    int pre_depth = transform_stack.size();

    for (rapidjson::Value::ConstMemberIterator itr = value.MemberBegin(),
         itr != value.MemberEnd(); ++itr)
    {
        // sampler
        if (itr.GetString() == "sampler")
        {
            parseSampler(itr->value,
                         scene);
        }
        // integrator
        else if (itr.GetString() == "integrator")
        {
            parseIntegrator(itr->value,
                            scene);
        }
        // camera
        else if (itr.GetString() == "camera")
        {
            parseCamera(itr->value,
                        transform_stack,
                        scene);
        }
        // object
        else if (itr.GetString() == "object")
        {
            parseSceneObject(itr->value,
                             transform_stack,
                             scene);
        }
        // emitter
        else if (itr.GetString() == "emitter")
        {
            parseEmitter(itr->value,
                         transform_stack,
                         scene);
        }
        // transform
        else if (itr.GetString() == "transform")
        {
            Transform transform = parseTransform(itr->value);
            transform_stack.push_back(transform);
        }
        else
        {
            throw new UnrecognizedTokenException(std::string(itr.GetString()));
        }
    }

    while (transform_stack.size() > pre_depth) transform_stack.pop_back();

    parent->addChild(scene);
}

Node* JsonParser::parse(std::string filename)
{
    FILE* file = fopen(filename, "r");
    char read_buffer[65536];
    rapidjson::FileReadStream input_stream(file, read_buffer, sizeof(read_buffer));
    rapidjson::Document document;
    document.parseStream(input_stream);
    fclose(input_stream);

    std::vector<Node*> nodes = std::vector<Node*>();
    std::vector<Transform> transform_stack = std::vector<Transform>();

    WorldNode* world = new WorldNode();
    transform_stack.push_back(Transform());

    for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
         itr != document.MemberEnd(); ++itr)
    {
        static const char kTypeNames[] = {"Null", "False", "True", "Object", "Array", "String", "Number"};

        if (itr->name.GetString() == "scene")
        {
            parseScene(itr->value,
                       transform_stack,
                       world);
        }
        else
        {
            throw new UnrecognizedTokenException(std::string(itr->name.GetString()));
        }
    }

    return world;
}
