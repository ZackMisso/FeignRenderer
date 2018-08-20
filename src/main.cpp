#include <iostream>
#include <feign/parser/parser.h>

int main(int argc, char* argv[]) {
    std::cout << "UNDER CONSTRUCTION" << std::endl;

    SceneNode* worldNode = Parser::parse("/Users/fortuna/Documents/Hobbies/FeignRenderer/scenes/ajax-normals.firal");

    if (!worldNode)
    {
        // TODO: debug
        cout << "MAJOR ERROR" << endl;
    }

    // TODO: render

    return 0;
}
