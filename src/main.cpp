/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

#include <iostream>
#include <feign/parser/json_parser.h>
#include <feign/core/scene.h>
#include <feign/misc/scene_generator.h>
#include <tests/tester.h>

#include "../scenes/adrien/ajax_cosine.h"

void run_scene(std::string scene_name)
{
    FeignRenderer::initialize();

    JsonParser::parse(SCENES_PATH + scene_name + ".json");

    FeignRenderer::clean_up();
}

// these are all the base debug scenes
void run_all_scenes()
{
    run_scene("generated/ajax-normals");
    run_scene("generated/ajax-diffuse");
    run_scene("generated/ajax-amb_occ");

    run_scene("tests/ajax_normals");
    run_scene("tests/ajax_diffuse");
}

int main(int argc, char* argv[])
{
    std::cout << "UNDER CONSTRUCTION" << std::endl;

    // //////// TEMPORARY ////////
    //
    // ajax_cosine();
    // return 0;
    //
    // /////\\\ TEMPORARY \\\/////

    // std::string scene = "tests/ajax_normals";
    std::string scene = "tests/ajax_diffuse";
    // std::string scene = "generated/ajax-normals-new-format";
    // scene = "generated/ajax-diffuse";
    // scene = "generated/ajax-amb_occ";

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-u") == 0)
        {
            std::cout << "Beginning Unit Tests" << std::endl;
            UnitTestManager* unitTests = new UnitTestManager();

            if (!unitTests->runUnitTests())
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
        }
        if (strcmp(argv[i], "-g") == 0)
        {
            std::cout << "Generating all scenes" << std::endl;

            SceneGenerator::create_all_scenes();

            std::cout << "Finished generating all scenes" << std::endl;
        }
        if (strcmp(argv[i], "-s") == 0)
        {
            scene = std::string(argv[++i]);
        }
        if (strcmp(argv[i], "-a") == 0)
        {
            run_all_scenes();
            return 0;
        }
    }

    std::cout << SCENES_PATH << scene << ".json" << std::endl;

    run_scene(scene);

    return 0;
}
