#pragma once

#include <feign/node.h>

class JsonParser
{
public:
    // static void possiblyAddChild(std::vector<Node*>& nodes,
    //                              Node* node);

    static Transform parseTransform(const rapidjson::Value& value);
    static void parseBSDF(const rapidjson::Value& value,
                          Node* parent);
    static void parseSampler(const rapidjson::Value& value,
                             Node* parent);
    static void parseIntegrator(const rapidjson::Value& value,
                                Node* parent);
    static void parseEmitter(const rapidjson::Value& value,
                             std::vector<Transform>& transform_stack,
                             Node* parent);
    static void parseCamera(const rapidjson::Value& value,
                            std::vector<Transform>& transform_stack,
                            Node* parent);
    static void parseSceneObject(const rapidjson::Value& value,
                                 std::vector<Transform>& transform_stack,
                                 Node* parent);
    static void parseScene(const rapidjson::Value& value,
                           std::vector<Transform>& transform_stack,
                           Node* parent);

    static Node* parse(std::string filename);
};
