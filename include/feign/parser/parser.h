#pragma once

#include <feign/node.h>
#include <feign/parser/token.h>

class Parser {
    static SceneNode* parse(string filename);
    static Token* tokenize(const string& data, int startPos, int endPos, Token* currentToken);
};
