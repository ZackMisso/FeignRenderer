/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/scene.h>
#include <rapidjson/document.h>

FEIGN_BEGIN()

// the idea behind the parsers is to keep them from having any information
// related to the current state of the renderer. All the information related to
// the state of the render state and scene graph should be set and updated by
// the api. The parser should only make api calls.

class JsonParser
{
public:
    static void parse(std::string filename);
    static void parse(std::string filename, Imagef* image);

protected:
    static void actually_parse(rapidjson::Document& document);
};

FEIGN_END()
