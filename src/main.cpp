/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/parser/json_parser.h>
#include <feign/core/scene.h>
#include <tests/tester.h>
#include <iostream>

#include "../scenes/adrien/ajax_cosine.h"
#include "../scenes/adrien/neon_intro.h"
#include "../scenes/adrien/neon_take_2.h"
// #include "../scenes/adrien/sphere_ground.h"
#include "../scenes/adrien/sandbox.h"
#include "../scenes/month_of_shaders/jan_1_2020.h"
#include "../scenes/month_of_shaders/jan_2_2020.h"
#include "../scenes/month_of_shaders/jan_3_2020.h"
#include "../scenes/month_of_shaders/jan_4_2020.h"
#include "../scenes/month_of_shaders/jan_5_2020.h"
#include "../scenes/month_of_shaders/jan_6_2020.h"
#include "../scenes/month_of_shaders/jan_7_2020.h"
#include "../scenes/month_of_shaders/jan_8_2020.h"
#include "../scenes/month_of_shaders/jan_9_2020.h"
#include "../scenes/cool_renders/hall_of_tiles/hall_of_tiles.h"

void run_scene(std::string scene_name)
{
    JsonParser::parse(SCENES_PATH + scene_name + ".json");

    FeignRenderer::clean_up();
}

// these are all the base debug scenes
// void run_all_scenes()
// {
//     // run_scene("tests/ajax_normals");
//     // run_scene("tests/ajax_diffuse");
//     // run_scene("tests/ajax_mirror");
//     // // run_scene("tests/box_distant");
//     // run_scene("tests/box_rotations");
// }

int main(int argc, char* argv[])
{
    // //////// TEMPORARY ////////
    //
    // // ajax_cosine();
    // neon_intro();
    // return 0;
    // neon_intro_2();
    // return 0;

    // sandbox();
    // return 0;

    // HallOfTiles();
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
    // jan_9_2020();
    // sphere_ground();
    // return 0;

    ///\\\ Month of shaders \\\/////

    // std::string scene = "tests/ajax_normals";
    // std::string scene = "tests/ajax_diffuse";
    // std::string scene = "tests/ajax_mirror";
    // std::string scene = "tests/box_rotations";
    // std::string scene = "tests/ajax_bary";
    // std::string scene = "tests/cornell_box_diffuse";
    // std::string scene = "tests/cornell_box_diffuse_area_light";
    // std::string scene = "tests/cornell_box_dielectrics";
    // std::string scene = "tests/cornell_box_phong";
    std::string scene = "tests/spot_light";

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-u") == 0)
        {
            std::cout << "Beginning Unit Tests" << std::endl;
            UnitTestManager* unitTests = new UnitTestManager();

            if (!unitTests->run_all_tests())
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-ui") == 0)
        {
            std::cout << "Beginning Unit Tests" << std::endl;
            UnitTestManager* unitTests = new UnitTestManager();

            int index = std::stoi(argv[i+1]);

            if (!unitTests->run_test(index))
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-ur") == 0)
        {
            UnitTestManager* unitTests = new UnitTestManager();
            unitTests->reference_run = true;

            if (!unitTests->run_all_tests())
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-uri") == 0)
        {
            UnitTestManager* unitTests = new UnitTestManager();
            unitTests->reference_run = true;

            int index = std::stoi(argv[i+1]);

            if (!unitTests->run_test(index))
            {
                delete unitTests;
                std::cout << "Unit Tests Failed -> terminating early" << std::endl;
                return -1;
            }
            delete unitTests;

            std::cout << "All Unit Tests Passed" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "-s") == 0)
        {
            scene = std::string(argv[++i]);
        }
        // if (strcmp(argv[i], "-a") == 0)
        // {
        //     // run_all_scenes();
        //     // return 0;
        // }
    }

    std::cout << SCENES_PATH << scene << ".json" << std::endl;

    run_scene(scene);

    return 0;
}
