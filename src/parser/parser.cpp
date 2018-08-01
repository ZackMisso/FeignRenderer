#include <feign/parser/parser.h>
#include <feign/bsdfs/brdf.h>
#include <feign/emitters/emitter.h>
#include <feign/integrators/integrator.h>
#include <feign/samplers/sampler.h>
#include <feign/exceptions.h>
#include <feign/scene.h>
#include <fstream>
#include <sstream>
#include <iostream>

SceneNode* Parser::parse(string filename)
{
    // check to make sure all required nodes have an end tag
    bool validFile = checkBalance(filename);

    // TODO: create an exceptions system for this
    if (!validFile)
    {
        cout << "Invalid File format. Make sure all nodes have end tags" << endl;
        return nullptr;
    }

    // tokenize the file
    ifstream file;
    file.open(filename);
    stringstream fileStream;
    fileStream << file.rdbuf();
    vector<string> tokens = vector<string>();
    string str;

    while (fileStream >> str)
        tokens.push_back(str);

    // generate the scene using the tokens
    return generateScene(tokens);
}

bool Parser::getNextToken(const vector<string>& tokens, string& token, int index) {
    // check if the token has reached the end
    if (index >= tokens.size())
    {
        token = "";
        return false;
    }

    // if not return the next token
    token = tokens[index];
    return true;
}

void Parser::possiblyAddChild(vector<SceneNode*>& nodes, SceneNode* node)
{
    if (nodes.size() > 0)
    {
        nodes[nodes.size() - 1]->addChild(node);
    }

    if (!node->isPrimitive())
    {
        nodes.push_back(node);
    }
}

SceneNode* Parser::generateScene(const vector<string>& tokens)
{
    // initializes counters, and node stack
    vector<SceneNode*> nodes = vector<SceneNode*>();
    vector<string> nodeTokens = vector<string>();

    string token;
    int index = 0;

    // main parse loop
    while (getNextToken(tokens, token, index++))
    {
        if (token == "scene")
        {
            nodeTokens.push_back(token);

            Scene* scene = new Scene();

            possiblyAddChild(nodes, scene);

            // TODO - add scene specific variables
        }
        else if (token == "sampler")
        {
            nodeTokens.push_back(token);

            Sampler* sampler = new Sampler();

            possiblyAddChild(nodes, scene);

            // TODO
        }
        else if (token == "integrator")
        {
            nodeTokens.push_back(token);

            Integrator* integrator = new Integrator();

            possiblyAddChild(nodes, integrator);

            // TODO
        }
        else if (token == "camera")
        {
            nodeTokens.push_back(token);

            Camera* camera = new Camera();

            possiblyAddChild(nodes, camera);

            // TODO
        }
        else if (token == "transform")
        {
            nodeTokens.push_back(token);

            Transform* transform = new Transform();

            possiblyAddChild(nodes, transform);

            // TODO
        }
        else if (token == "mesh")
        {
            nodeTokens.push_back(token);

            Mesh* mesh = new Mest(mesh);

            possiblyAddChild(nodes, mesh);

            // TODO
        }
        else if (token == "bsdf")
        {
            nodeTokens.push_back(token);

            BRDF* brdf = new BRDF();

            possiblyAddChild(nodes, brdf);

            // TODO
        }
    }



    // TODO
    return nullptr;
}

bool Parser::checkBalance(string filename)
{
    ifstream file;
    file.open(filename);
    stringstream ss;
    ss << file.rdbuf();
    string buf;

    vector<string> majorNodes = vector<string>();

    while (ss >> buf)
    {
        if (buf == "scene") majorNodes.push_back(buf);
        else if (buf == "sampler") majorNodes.push_back(buf);
        else if (buf == "integrator") majorNodes.push_back(buf);
        else if (buf == "camera") majorNodes.push_back(buf);
        else if (buf == "transform") majorNodes.push_back(buf);
        else if (buf == "mesh") majorNodes.push_back(buf);
        else if (buf == "bsdf") majorNodes.push_back(buf);

        else if (buf == "/scene")
        {
            if (majorNodes[majorNodes.size() - 1] == "scene")
                majorNodes.pop_back();
            else
            {
                file.close();
                return false;
            }
        }
        else if (buf == "/sampler")
        {
            if (majorNodes[majorNodes.size() - 1] == "sampler")
                majorNodes.pop_back();
            else
            {
                file.close();
                return false;
            }
        }
        else if (buf == "/integrator")
        {
            if (majorNodes[majorNodes.size() - 1] == "integrator")
                majorNodes.pop_back();
            else
            {
                file.close();
                return false;
            }
        }
        else if (buf == "/camera")
        {
            if (majorNodes[majorNodes.size() - 1] == "camera")
                majorNodes.pop_back();
            else
            {
                file.close();
                return false;
            }
        }
        else if (buf == "/transform")
        {
            if (majorNodes[majorNodes.size() - 1] == "transform")
                majorNodes.pop_back();
            else
            {
                file.close();
                return false;
            }
        }
        else if (buf == "/mesh")
        {
            if (majorNodes[majorNodes.size() - 1] == "mesh")
                majorNodes.pop_back();
            else
            {
                file.close();
                return false;
            }
        }
        else if (buf == "/bsdf")
        {
            if (majorNodes[majorNodes.size() - 1] == "bsdf")
                majorNodes.pop_back();
            else
            {
                file.close();
                return false;
            }
        }
    }

    return majorNodes.empty();
}

// Token* Parser::tokenize(const string& data, int startPos, int endPos, Token* currentToken)
// {
//     bool foundNextToken = false;
//     int pos = startPos;
//
//     while(!foundNextToken) {
//         if (data[pos++] == '<') {
//             foundNextToken = true;
//             break;
//         }
//
//         if (pos > endPos) break;
//     }
//
//     if (data[pos] == '/') {
//         pos++;
//         string tokObject = "";
//
//         while(data[pos] != '>') {
//             tokObject = tokObject + data[pos++];
//         }
//
//         if (tokObject != currentToken->tokenName) {
//             throw MissingTokenEndException(tokObject);
//         }
//
//         currentToken->closed = true;
//         return currentToken;
//     } else {
//         while (data[pos] != '>') {
//             string tokObject = "";
//             Token* newToken = nullptr;
//
//             while (data[pos] != ' ' || data[pos] != '>' || data[pos] != '/' || data[pos] != '\n') {
//                 tokObject = tokObject + data[pos++];
//             }
//
//             if (data[pos] == ' ') {
//
//                 if (tokObject == "emitter") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "media") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "sampler") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "integrator") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "int") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "float") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "long") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "double") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "texture") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "string") {
//                     if (!newToken) {
//                         // TODO
//                     } else {
//                         throw MultipleObjectTokenException(tokObject);
//                     }
//                 } else if (tokObject == "name") {
//                     if (!newToken) {
//                         throw FirstTokenException(tokObject);
//                     } else {
//                         // TODO
//                     }
//                 } else if (tokObject == "type") {
//                     if (!newToken) {
//                         throw FirstTokenException(tokObject);
//                     } else {
//                         // TODO
//                     }
//                 } else if (tokObject == "name") {
//                     if (!newToken) {
//                         throw FirstTokenException(tokObject);
//                     } else {
//                         // TODO
//                     }
//                 } else if (tokObject == "scene") {
//                     if (!newToken) {
//                         throw FirstTokenException(tokObject);
//                     } else {
//                         // TODO
//                     }
//                 } else {
//                     throw InvalidTokenException(tokObject);
//                 }
//             }
//
//             else if (data[pos] == '/') {
//                 // TODO
//             }
//
//             else if (data[pos] == '>') {
//                 // TODO
//             }
//
//             else if (data[pos] == '=') {
//                 // TODO
//             }
//
//             else {
//                 // TODO
//             }
//         }
//     }
//
//     // TODO
//
//     return nullptr;
// }

// maybe use this
// vector<TokenValue> Parser::getTokens() {
//     vector<TokenValue> tokens = vector<TokenValue>();
//
//     tokens.push_back(TokenValue("emitter", TT_EMITTER));
//     tokens.push_back(TokenValue("sampler", TT_SAMPLER));
//     tokens.push_back(TokenValue("int", TT_INT));
//     tokens.push_back(TokenValue("long", TT_LONG));
//     tokens.push_back(TokenValue("float", TT_FLOAT));
//     tokens.push_back(TokenValue("double", TT_DOUBLE));
//     tokens.push_back(TokenValue("string", TT_STRING));
//     tokens.push_back(TokenValue("name", TT_NAME));
//     tokens.push_back(TokenValue("type", TT_TYPE));
//     tokens.push_back(TokenValue("value", TT_VALUE));
//     tokens.push_back(TokenValue("up", TT_UP));
//     tokens.push_back(TokenValue("target", TT_TARGET));
//     tokens.push_back(TokenValue("origin", TT_ORIGIN));
//
//     return tokens;
// }
