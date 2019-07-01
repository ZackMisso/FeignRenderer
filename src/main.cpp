#include <iostream>
#include <feign/parser/json_parser.h>
#include <feign/misc/world.h>
#include <feign/misc/scene_generator.h>
#include <tests/tester.h>

int main(int argc, char* argv[]) {
    // std::cout << 0.002*0.002+0.010*0.010+0.991*0.991 << std::endl;
    // std::cout << 0.004*0.004+0.014*0.014+0.991*0.991 << std::endl;
    
    std::cout << "UNDER CONSTRUCTION" << std::endl;

    std::string scene = "ajax-normals-generated";

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
    }

    std::cout << SCENES_PATH << scene << ".json" << std::endl;

    WorldNode* world = JsonParser::parse(SCENES_PATH + scene + ".json");

    // std::cout << std::endl;
    // std::cout << "New World Graph Description:" << std::endl;
    // std::cout << std::endl;
    // world->printGraph();
    // std::cout << std::endl;

    world->renderAllScenes();

    delete world;

    return 0;
}
