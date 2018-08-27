#include <iostream>
#include <feign/parser/parser.h>

int main(int argc, char* argv[]) {
    std::cout << "UNDER CONSTRUCTION" << std::endl;

    Node* worldNode = Parser::parse("/Users/corneria/Documents/Projects/FeignRenderer/scenes/ajax-normals.firal");

    if (!worldNode)
    {
        // TODO: debug
        cout << "MAJOR ERROR" << endl;
    }

    // NOTE: calling it world graph because i want to support multiple scenes
    //       from one scene file -> actually render animations
    cout << endl;
    cout << "World Graph:" << endl;
    cout << endl;
    worldNode->printGraph();
    cout << endl;

    // TODO: render

    return 0;
}
