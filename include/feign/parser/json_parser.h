/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/misc/world.h>
#include <rapidjson/document.h>

// the idea behind the parsers is to keep them from having any information
// related to the current state of the renderer. All the information related to
// the state of the render state and scene graph should be set and updated by
// the api. The parser should only make api calls.

class JsonParser
{
public:
    static void parseTransform(const rapidjson::Value& value);

    static void parseBSDF(const rapidjson::Value& value);

    static void parseSampler(const rapidjson::Value& value);

    static void parseIntegrator(const rapidjson::Value& value);

    static void parseEmitter(const rapidjson::Value& value);

    static void parseCamera(const rapidjson::Value& value);

    static void parseSceneObject(const rapidjson::Value& value);

    static void parseScene(const rapidjson::Value& value);

    static WorldNode* parse(std::string filename);
};
