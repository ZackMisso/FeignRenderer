/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <iostream>
#include <feign/parser/json_parser.h>
#include <feign/core/scene.h>
#include <tests/tester.h>

#include "../scenes/adrien/ajax_cosine.h"
#include "../scenes/adrien/neon_intro.h"
#include "../scenes/adrien/sphere_ground.h"
#include "../scenes/month_of_shaders/jan_1_2020.h"
#include "../scenes/month_of_shaders/jan_2_2020.h"
#include "../scenes/month_of_shaders/jan_3_2020.h"
#include "../scenes/month_of_shaders/jan_4_2020.h"
#include "../scenes/month_of_shaders/jan_5_2020.h"
#include "../scenes/month_of_shaders/jan_6_2020.h"
#include "../scenes/month_of_shaders/jan_7_2020.h"
#include "../scenes/month_of_shaders/jan_8_2020.h"
#include "../scenes/month_of_shaders/jan_9_2020.h"

void run_scene(std::string scene_name)
{
    FeignRenderer::initialize();

    JsonParser::parse(SCENES_PATH + scene_name + ".json");

    FeignRenderer::clean_up();
}

// these are all the base debug scenes
void run_all_scenes()
{
    run_scene("tests/ajax_normals");
    run_scene("tests/ajax_diffuse");
    run_scene("tests/ajax_mirror");
    // run_scene("tests/box_distant");
    run_scene("tests/box_rotations");
}

int main(int argc, char* argv[])
{
    std::cout << "UNDER CONSTRUCTION" << std::endl;

    // //////// TEMPORARY ////////
    //
    // // ajax_cosine();
    // neon_intro();
    // return 0;
    //
    // /////\\\ TEMPORARY \\\/////

    ////// Month of shaders ////////

    // jan_1_2020();          // done
    // jan_2_2020();          // done
    // jan_3_2020();          // done
    // jan_4_2020();         // done
    // jan_5_2020();          // done
    // jan_6_2020();
    // jan_7_2020();
    // jan_8_2020();
    jan_9_2020();
    // sphere_ground();
    return 0;

    ///\\\ Month of shaders \\\/////

    // std::string scene = "tests/ajax_normals";
    // std::string scene = "tests/ajax_diffuse";
    // std::string scene = "tests/ajax_mirror";
    // std::string scene = "tests/box_rotations";
    // std::string scene = "tests/ajax_bary";
    std::string scene = "tests/cornell_box_diffuse";

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
