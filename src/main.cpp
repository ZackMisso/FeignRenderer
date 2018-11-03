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
        cout << "Unit Tests Failed -> terminating early" << endl;
        return -1;
    }
    delete unitTests;
    std::cout << "All Unit Tests Passed" << std::endl;

    std::cout << "Parsing: " << SCENES_PATH "ajax-normals.firal" << std::endl;
    Node* parsedNode = Parser::parse(SCENES_PATH "ajax-normals.firal");

    if (!parsedNode || parsedNode->getNodeType() != NT_World)
    {
        cout << "Parse failed" << endl;
        return -1;
    }

    WorldNode* worldNode = (WorldNode*)parsedNode;

    // NOTE: calling it world graph because i want to support multiple scenes
    //       from one scene file -> actually render animations
    cout << endl;
    cout << "World Graph Description:" << endl;
    cout << endl;
    worldNode->printGraph();
    cout << endl;

    // preprocess the world
    cout << "Pre Processing" << endl;
    worldNode->preProcess();

    // render
    worldNode->renderAllScenes();

    return 0;
}
