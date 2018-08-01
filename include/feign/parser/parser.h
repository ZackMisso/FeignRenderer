#pragma once

#include <feign/node.h>
#include <feign/parser/token.h>

class Parser
{
public:
    static SceneNode* parse(string filename);
    static SceneNode* generateScene(const vector<string>& tokens);

    static bool checkBalance(string filename);

    // static Token* tokenize(const string& data, int startPos, int endPos, Token* currentToken);

    // static vector<TokenValue> getTokens();
};
