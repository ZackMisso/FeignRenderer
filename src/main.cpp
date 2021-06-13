/**
* Author:    Zackary Misso
* Version:   0.2.0
*
* Anyone has permission to use the following code as long as proper
* acknowledgement is provided to the original author(s).
**/

#include <feign/parsers/json_parser.h>
#include <feign/core/scene.h>
#include <feign/test/tester.h>
#include <iostream>

#include <openvdb/openvdb.h>

#include "../scenes/cool_renders/one_way_dice/one_way_dice.h"

using namespace feign;

void run_scene(std::string scene_name)
{
    JsonParser::parse(scene_name);

    FeignRenderer::clean_up();
}

int main(int argc, char* argv[])
{
    #if OPENVDB
        openvdb::initialize();
    #endif
    // //////// TEMPORARY ////////

    // OneWayDice();
    // return 0;

    // /////\\\ TEMPORARY \\\////

    std::string scene = "";

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
    }

    std::cout << std::endl;
    std::cout << scene << std::endl;
    std::cout << std::endl;

    run_scene(scene);

    return 0;
}
