/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/scene.h>
#include <feign/stats/clocker.h>
#include <rapidjson/document.h>

FEIGN_BEGIN()

// the idea behind the parsers is to keep them from having any information
// related to the current state of the renderer. All the information related to
// the state of the render state and scene graph should be set and updated by
// the api. The parser should only make api calls.

class JsonParser
{
public:
#if CLOCKING
    static void parse_and_run(std::string filename,
                              ClockerResults* clockings = nullptr);
    static void parse_and_run(std::string filename,
                              Imagef *image,
                              ClockerResults* clockings = nullptr);
#elif
    static void parse_and_run(std::string filename);
    static void parse_and_run(std::string filename, Imagef *image); 
#endif

protected:
#if CLOCKING
    static void actually_parse(rapidjson::Document &document,
                               ClockerResults* clockings = nullptr);
#elif
    static void actually_parse(rapidjson::Document &document);
#endif
};

FEIGN_END()
