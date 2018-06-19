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
        while (data[pos] != '>') {
            string tokObject = "";
            Token* newToken = nullptr;

            while (data[pos] != ' ' || data[pos] != '>' || data[pos] != '/' || data[pos] != "\n") {
                tokObject = tokObject + data[pos++];
            }

            if (data[pos] == ' ') {

                if (tokObject == "emitter") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "media") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "sampler") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "integrator") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "int") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "float") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "long") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "double") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "texture") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "string") {
                    if (!newToken) {
                        // TODO
                    } else {
                        throw MultipleObjectTokenException(tokObject);
                    }
                } else if (tokObject == "name") {
                    if (!newToken) {
                        throw FirstTokenException(tokObject);
                    } else {
                        // TODO
                    }
                } else if (tokObject == "type") {
                    if (!newToken) {
                        throw FirstTokenException(tokObject);
                    } else {
                        // TODO
                    }
                } else if (tokObject == "name") {
                    if (!newToken) {
                        throw FirstTokenException(tokObject);
                    } else {
                        // TODO
                    }
                } else {
                    throw InvalidTokenException(tokObject);
                }
            }

            else if (data[pos] == '/') {
                // TODO
            }

            else if (data[pos] == '>') {
                // TODO
            }

            else if (data[pos] == "=") {
                // TODO
            }

            else {
                // TODO
            }
        }
    }

    // TODO

    return nullptr;
}

// maybe use this
vector<TokenValue> Parser::getTokens() {
    vector<TokenValue> tokens = vector<TokenValue>()

    tokens.push_back(TokenValue("emitter", TT_EMITTER));
    tokens.push_back(TokenValue("sampler", TT_SAMPLER));
    tokens.push_back(TokenValue("int", TT_INT));
    tokens.push_back(TokenValue("long", TT_LONG));
    tokens.push_back(TokenValue("float", TT_FLOAT));
    tokens.push_back(TokenValue("double", TT_DOUBLE));
    tokens.push_back(TokenValue("string", TT_STRING));
    tokens.push_back(TokenValue("name", TT_NAME));
    tokens.push_back(TokenValue("type", TT_TYPE));
    tokens.push_back(TokenValue("value", TT_VALUE));
    tokens.push_back(TokenValue("up", TT_UP));
    tokens.push_back(TokenValue("target", TT_TARGET));
    tokens.push_back(TokenValue("origin", TT_ORIGIN));

    return tokens;
}
