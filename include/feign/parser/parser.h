#pragma once

#include <feign/node.h>
#include <feign/parser/token.h>

class Parser
{
public:
    static SceneNode* parse(string filename);
    static SceneNode* generateScene(const vector<string>& tokens);
    static bool getNextToken(const vector<string>& tokens, string& token, int index);
    static void possiblyAddChild(vector<SceneNode*>& nodes, SceneNode* node);

    static bool checkBalance(string filename);
};
