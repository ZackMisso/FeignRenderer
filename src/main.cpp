#include <iostream>
#include <feign/parser/json_parser.h>
#include <feign/misc/world.h>
#include <tests/tester.h>

int main(int argc, char* argv[]) {
    std::cout << "UNDER CONSTRUCTION" << std::endl;

    // std::cout << "Beginning Unit Tests" << std::endl;
    // UnitTestManager* unitTests = new UnitTestManager();
    //
    // if (!unitTests->runUnitTests())
    // {
    //     delete unitTests;
    //     std::cout << "Unit Tests Failed -> terminating early" << std::endl;
    //     return -1;
    // }
    // delete unitTests;
    //
    // std::cout << "All Unit Tests Passed" << std::endl;

    WorldNode* world = JsonParser::parse(SCENES_PATH "ajax-normals-generated.json");
    std::cout << "post world" << std::endl;
    // std::cout << std::endl;
    // std::cout << "New World Graph Description:" << std::endl;
    // std::cout << std::endl;
    // world->printGraph();
    // std::cout << std::endl;

    world->renderAllScenes();

    return 0;
}
