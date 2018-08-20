#pragma once

#include <feign/node.h>

class Parser
{
public:
    static Node* parse(string filename);
    static Node* generateScene(const vector<string>& tokens);
    static bool getNextToken(const vector<string>& tokens, string& token, int index);
    static void possiblyAddChild(vector<Node*>& nodes, Node* node);

    static void checkBalance(string filename);
};
