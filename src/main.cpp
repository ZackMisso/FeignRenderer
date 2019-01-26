#include <iostream>
#include <feign/parser/parser.h>
#include <feign/misc/world.h>
#include <tests/tester.h>

int main(int argc, char* argv[]) {
    std::cout << "UNDER CONSTRUCTION" << std::endl;

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

    std::cout << "Parsing: " << SCENES_PATH "ajax-normals.firal" << std::endl;
    Node* parsedNode = Parser::parse(SCENES_PATH "ajax-normals.firal");

    if (!parsedNode || parsedNode->getNodeType() != NT_World)
    {
        std::cout << "Parse failed" << std::endl;
        return -1;
    }

    WorldNode* worldNode = (WorldNode*)parsedNode;

    // NOTE: calling it world graph because i want to support multiple scenes
    //       from one scene file -> actually render animations
    std::cout << std::endl;
    std::cout << "World Graph Description:" << std::endl;
    std::cout << std::endl;
    worldNode->printGraph();
    std::cout << std::endl;

    // preprocess the world
    std::cout << "Pre Processing" << std::endl;
    worldNode->preProcess();

    // render
    worldNode->renderAllScenes();

    return 0;
}
