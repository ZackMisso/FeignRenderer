#include <feign/parser/parser.h>
#include <feign/exceptions.h>

SceneNode* Parser::parse(string filename) {
    // TODO

    return nullptr;
}

Token* Parser::tokenize(const string& data, int startPos, int endPos, Token* currentToken) {
    // bool foundNextToken = false;
    // int pos = startPos;
    //
    // while (!foundNextToken) {
    //     if (data[pos] == '<') foundNextToken = true;
    // }
    //
    // bool foundPrevToken = false;
    // int prevPos = endPos;
    //
    // while(!foundPrevToken) {
    //     if (data[prevPos == ">"])
    // }

    bool foundNextToken = false;
    int pos = startPos;

    while(!foundNextToken) {
        if (data[pos++] == '<') {
            foundNextToken = true;
            break;
        }

        if (pos > endPos) break;
    }

    if (data[pos] == '/') {
        pos++;
        string tokObject = "";

        while(data[pos] != '>') {
            tokObject = tokObject + data[pos++];
        }

        if (tokObject != currentToken->tokenName) {
            throw MissingTokenEndException(tokObject);
        }

        currentToken->closed = true;
        return currentToken;
    } else {
        string tokObject = "";

        while (data[pos] != ' ' || data[pos] != '>' || data[pos] != '/') {
            tokObject = tokObject + data[pos++];
        }

        if (data[pos] == ' ') {
            // TODO
        }

        else if (data[pos] == '/') {
            // TODO
        }

        else if (data[pos] == '>') {
            // TODO
        }

        else {
            // TODO
        }


    }

    // TODO

    return nullptr;
}
