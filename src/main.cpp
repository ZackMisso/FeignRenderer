#include <iostream>
#include <feign/parser/parser.h>
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

    ////////// OLD PARSER TEST //////////
    // std::cout << "Parsing: " << SCENES_PATH "ajax-normals.firal" << std::endl;
    // Node* parsedNode = Parser::parse(SCENES_PATH "ajax-normals.firal");
    // // Node* parsedNode = Parser::parse(SCENES_PATH "ajax-normals.firal");
    //
    // if (!parsedNode || parsedNode->getNodeType() != NT_World)
    // {
    //     std::cout << "Parse failed" << std::endl;
    //     return -1;
    // }
    //
    // WorldNode* worldNode = (WorldNode*)parsedNode;
    //
    // // NOTE: calling it world graph because i want to support multiple scenes
    // //       from one scene file -> actually render animations
    // std::cout << std::endl;
    // std::cout << "Old World Graph Description:" << std::endl;
    // std::cout << std::endl;
    // worldNode->printGraph();
    // std::cout << std::endl;
    //
    // // preprocess the world
    // std::cout << "Pre Processing" << std::endl;
    // worldNode->preProcess(true);
    // // // render
    // // worldNode->renderAllScenes();
    /////////////////////////////////////

    std::cout << "parsing new world" << std::endl;

    WorldNode* newWorld = JsonParser::parse(SCENES_PATH "ajax-normals-generated.json");

    // std::cout << std::endl;
    // std::cout << "New World Graph Description:" << std::endl;
    // std::cout << std::endl;
    // newWorld->printGraph();
    // std::cout << std::endl;
    //
    // std::cout << "Rendering" << std::endl;
    //
    // std::vector<Scene*> new_scenes = newWorld->collectScenes();
    // std::vector<Scene*> old_scenes = worldNode->collectScenes();
    //
    // std::cout << "new world camera: " << std::endl;
    // new_scenes[0]->getCamera()->print();
    // std::cout << "old world camera: " << std::endl;
    // old_scenes[0]->getCamera()->print();

    newWorld->renderAllScenes();

    return 0;
}
