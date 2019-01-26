#pragma once

#include <feign/node.h>

class Parser
{
public:
    static Node* parse(std::string filename);
    static Node* generateWorld(const std::vector<std::string>& tokens);
    static bool getNextToken(const std::vector<std::string>& tokens,
                             std::string& token,
                             int index);
    static void possiblyAddChild(std::vector<Node*>& nodes,
                                 Node* node);

    static void checkBalance(std::string filename);
};
