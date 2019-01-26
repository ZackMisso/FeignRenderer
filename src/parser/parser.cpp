#include <feign/parser/parser.h>
#include <feign/bsdfs/bsdf.h>
#include <feign/bsdfs/diffuse.h>
#include <feign/cameras/camera.h>
#include <feign/cameras/perspective.h>
#include <feign/emitters/emitter.h>
#include <feign/integrators/integrator.h>
#include <feign/integrators/normal.h>
#include <feign/math/transform.h>
#include <feign/misc/prim.h>
#include <feign/misc/world.h>
#include <feign/misc/transformnode.h>
#include <feign/samplers/sampler.h>
#include <feign/samplers/independent.h>
#include <feign/shapes/objmesh.h>
#include <feign/exceptions.h>
#include <feign/scene.h>
#include <fstream>
#include <sstream>
#include <iostream>

Node* Parser::parse(string filename)
{
    // check to make sure all required nodes have an end tag
    checkBalance(filename);

    // tokenize the file
    // TODO: this process of parsing is extremely slow... fix this
    ifstream file;
    file.open(filename);
    stringstream fileStream;
    fileStream << file.rdbuf();
    vector<string> tokens = vector<string>();
    cout << tokens.size() << endl;
    string str;

    while (fileStream >> str)
        tokens.push_back(str);

    // generate the scene using the tokens
    return generateWorld(tokens);
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

void Parser::possiblyAddChild(vector<Node*>& nodes,
                              Node* node)
{
    if (nodes.size() > 0)
    {
        nodes[nodes.size() - 1]->addChild(node);
    }

    nodes.push_back(node);
}

Node* Parser::generateWorld(const vector<string>& tokens)
{
    // initializes counters, and node stack
    vector<Node*> nodes = vector<Node*>();
    vector<string> nodeTokens = vector<string>();
    vector<Transform> transformStack = vector<Transform>();

    string token;
    int index = 0;

    WorldNode* world = new WorldNode();
    nodes.push_back(world);
    transformStack.push_back(Transform());

    // main parse loop
    while (getNextToken(tokens, token, index++))
    {
        if (token == "scene")
        {
            nodeTokens.push_back(token);
            transformStack.push_back(Transform());

            Scene* scene = new Scene();

            possiblyAddChild(nodes, scene);
        }
        else if (token == "/scene")
        {
            if (nodes[nodes.size() - 1]->getNodeType() == NT_Scene)
            {
                Node* node = nodes.back();

                Transform lastTransform = transformStack.back();
                Primitive<Transform>* transformPrim = new Primitive<Transform>("toWorld", lastTransform);
                node->getPrimList()->addTransformPrimitive(transformPrim);
                cout << node->getName() << endl;
                lastTransform.print();

                transformStack.pop_back();

                nodes.pop_back();
            }
            else
            {
                throw new InvalidEndTokenException();
            }
        }
        else if (token == "sampler")
        {
            nodeTokens.push_back(token);

            Sampler* sampler = nullptr;

            string typeToken;
            getNextToken(tokens, typeToken, index++);

            if (typeToken.empty())
            {
                throw MissingExpectedTokenException("sampType");
            }

            if (typeToken == "independent")
            {
                sampler = new Independent();
            }
            else
            {
                throw UnsupportedTypeException("sampType: " + typeToken);
            }

            // TODO - more ???

            possiblyAddChild(nodes, sampler);
        }
        else if (token == "/sampler")
        {
            if (nodes[nodes.size() - 1]->getNodeType() == NT_Sampler)
            {
                nodes.pop_back();
            }
            else
            {
                throw new InvalidEndTokenException();
            }
        }
        else if (token == "integrator")
        {
            nodeTokens.push_back(token);

            Integrator* integrator = nullptr;

            string typeToken;
            getNextToken(tokens, typeToken, index++);

            if (typeToken.empty())
            {
                throw MissingExpectedTokenException("sampType");
            }

            if (typeToken == "normals")
            {
                integrator = new NormalIntegrator();
            }
            else
            {
                throw UnsupportedTypeException("sampType: " + typeToken);
            }

            // TODO - more ???

            possiblyAddChild(nodes, integrator);
        }
        else if (token == "/integrator")
        {
            if (nodes[nodes.size() - 1]->getNodeType() == NT_Integrator)
            {
                nodes.pop_back();
            }
            else
            {
                throw new InvalidEndTokenException();
            }
        }
        else if (token == "camera")
        {
            nodeTokens.push_back(token);

            Transform lastTransform = transformStack.back();
            transformStack.push_back(lastTransform);

            Camera* camera = nullptr;

            string typeToken;
            getNextToken(tokens, typeToken, index++);

            if (typeToken.empty())
            {
                throw MissingExpectedTokenException("camType");
            }

            if (typeToken == "perspective")
            {
                camera = new Perspective();
            }
            else
            {
                throw UnsupportedTypeException("camType: " + typeToken);
            }

            // TODO - more ???

            possiblyAddChild(nodes, camera);
        }
        else if (token == "/camera")
        {
            if (nodes[nodes.size() - 1]->getNodeType() == NT_Camera)
            {
                Node* node = nodes.back();

                Transform lastTransform = transformStack.back();
                Primitive<Transform>* transformPrim = new Primitive<Transform>("toWorld", lastTransform);
                node->getPrimList()->addTransformPrimitive(transformPrim);
                lastTransform.print();

                transformStack.pop_back();

                nodes.pop_back();
            }
            else
            {
                throw new InvalidEndTokenException();
            }
        }
        else if (token == "transform")
        {
            nodeTokens.push_back(token);

            string nameToken;
            getNextToken(tokens, nameToken, index++);

            if (nameToken != "name")
            {
                throw MissingExpectedTokenException("transform name");
            }

            string transformName;
            getNextToken(tokens, transformName, index++);

            Transform transform = Transform();

            while(tokens[index] != "/transform")
            {
                string matType;
                getNextToken(tokens, matType, index++);

                if (matType == "lookat")
                {
                    string targetText;
                    getNextToken(tokens, targetText, index++);

                    if (targetText != "target")
                    {
                        throw MissingExpectedTokenException("lookat target");
                    }

                    float targetX;
                    float targetY;
                    float targetZ;

                    string valText;
                    getNextToken(tokens, valText, index++);
                    targetX = stof(valText);

                    getNextToken(tokens, valText, index++);
                    targetY = stof(valText);

                    getNextToken(tokens, valText, index++);
                    targetZ = stof(valText);

                    string originText;
                    getNextToken(tokens, originText, index++);

                    if (originText != "origin")
                    {
                        throw MissingExpectedTokenException("lookat origin");
                    }

                    float originX;
                    float originY;
                    float originZ;

                    getNextToken(tokens, valText, index++);
                    originX = stof(valText);

                    getNextToken(tokens, valText, index++);
                    originY = stof(valText);

                    getNextToken(tokens, valText, index++);
                    originZ = stof(valText);

                    string upText;
                    getNextToken(tokens, upText, index++);

                    if (upText != "up")
                    {
                        throw MissingExpectedTokenException("lookat up");
                    }

                    float upX;
                    float upY;
                    float upZ;

                    getNextToken(tokens, valText, index++);
                    upX = stof(valText);

                    getNextToken(tokens, valText, index++);
                    upY = stof(valText);

                    getNextToken(tokens, valText, index++);
                    upZ = stof(valText);

                    Vec3f origin = Vec3f(originX, originY, originZ);
                    Vec3f target = Vec3f(targetX, targetY, targetZ);
                    Vec3f up = Vec3f(upX, upY, upZ);

                    Vec3f zaxis = (target - origin).normalized();
                    Vec3f xaxis = ((up.normalized()) ^ zaxis).normalized();
                    Vec3f yaxis = (zaxis ^ xaxis).normalized();

                    Matrix4f lookAtMatrix = Matrix4f();

                    lookAtMatrix.setRow(0, Vec4f(xaxis, 0.f));
                    lookAtMatrix.setRow(1, Vec4f(yaxis, 0.f));
                    lookAtMatrix.setRow(2, Vec4f(zaxis, 0.f));
                    lookAtMatrix.setRow(3, Vec4f(origin, 1.f));

                    Transform trans = Transform(lookAtMatrix);

                    transform = transform * trans;
                }
                else if (matType == "translate")
                {
                    Vec3f translate;

                    string valText;
                    getNextToken(tokens, valText, index++);
                    translate[0] = stof(valText);

                    getNextToken(tokens, valText, index++);
                    translate[1] = stof(valText);

                    getNextToken(tokens, valText, index++);
                    translate[2] = stof(valText);

                    Matrix4f translateMatrix = Matrix4f::translate(translate);

                    Transform trans = Transform(translateMatrix);

                    transform = transform * trans;
                }
                else if (matType == "rotate")
                {
                    throw new NotImplementedException("rotate parse");
                }
                else if (matType == "scale")
                {
                    Vec3f scale;

                    string valText;
                    getNextToken(tokens, valText, index++);
                    scale[0] = stof(valText);

                    getNextToken(tokens, valText, index++);
                    scale[1] = stof(valText);

                    getNextToken(tokens, valText, index++);
                    scale[2] = stof(valText);

                    Matrix4f scaleMatrix = Matrix4f::scale(scale);

                    Transform trans = Transform(scaleMatrix);

                    transform = transform * trans;
                }
                else
                {
                    throw new InvalidEndTokenException("transform");
                }
            }

            // cout << "Transform Name: " << transformName << endl;

            if (transformName == "toWorld")
            {
                Transform lastTransform = transformStack.back();
                transformStack.pop_back();
                transform = lastTransform * transform;

                transformStack.push_back(transform);
            }
            else
            {
                throw new NotImplementedException("non to world transforms");
            }
        }
        else if (token == "/transform")
        {
            // does nothing

            // cout << "Woop" << endl;
            // if (nodes[nodes.size() - 1]->getNodeType() == NT_Transform)
            // {
            //     nodes.pop_back();
            // }
            // else
            // {
            //     throw new InvalidEndTokenException();
            // }
            // cout << "Waap" << endl;
        }
        else if (token == "mesh")
        {
            nodeTokens.push_back(token);

            Transform lastTransform = transformStack.back();
            transformStack.push_back(lastTransform);

            Shape* mesh = nullptr;

            string typeToken;
            getNextToken(tokens, typeToken, index++);

            if (typeToken.empty())
            {
                throw MissingExpectedTokenException("meshType");
            }

            if (typeToken == "obj")
            {
                // cout << "Pre Contructor" << endl;
                mesh = new ObjMesh();
                // cout << "Post Contructor" << endl;

                // TODO
            }
            else
            {
                throw UnsupportedTypeException("meshType: " + typeToken);
            }

            // TODO - more ???

            possiblyAddChild(nodes, mesh);
        }
        else if (token == "/mesh")
        {
            if (nodes[nodes.size() - 1]->getNodeType() == NT_Mesh)
            {
                Node* node = nodes.back();

                Transform lastTransform = transformStack.back();
                Primitive<Transform>* transformPrim = new Primitive<Transform>("toWorld", lastTransform);
                node->getPrimList()->addTransformPrimitive(transformPrim);
                cout << node->getName() << endl;
                lastTransform.print();

                transformStack.pop_back();

                nodes.pop_back();
            }
            else
            {
                throw new InvalidEndTokenException();
            }
        }
        else if (token == "bsdf")
        {
            nodeTokens.push_back(token);

            BSDF* bsdf = nullptr;

            string typeToken;
            getNextToken(tokens, typeToken, index++);

            if (typeToken.empty())
            {
                throw MissingExpectedTokenException("bsdfType");
            }

            if (typeToken == "diffuse")
            {
                bsdf = new Diffuse();
            }
            else
            {
                throw UnsupportedTypeException("bsdfType: " + typeToken);
            }

            // TODO - more ???

            possiblyAddChild(nodes, bsdf);
        }
        else if (token == "/bsdf")
        {
            if (nodes[nodes.size() - 1]->getNodeType() == NT_BSDF)
            {
                nodes.pop_back();
            }
            else
            {
                throw new InvalidEndTokenException();
            }
        }
        else if (token == "int")
        {
            string nameToken;
            getNextToken(tokens, nameToken, index++);

            if (nameToken != "name")
            {
                throw MissingExpectedTokenException("name");
            }

            string nameValue;
            getNextToken(tokens, nameValue, index++);

            if (nameValue.empty())
            {
                throw MissingExpectedTokenException("name");
            }

            string valueToken;
            getNextToken(tokens, valueToken, index++);

            if (valueToken != "value")
            {
                throw MissingExpectedTokenException("value");
            }

            string valueValue;
            getNextToken(tokens, valueValue, index++);

            if (valueValue.empty())
            {
                throw MissingExpectedTokenException("value");
            }

            int val = stoi(valueValue);
            Primitive<int>* intPrim = new Primitive<int>(nameValue, val);

            nodes[nodes.size() - 1]->getPrimList()->addIntPrimitive(intPrim);
        }
        else if (token == "float")
        {
            string nameToken;
            getNextToken(tokens, nameToken, index++);

            if (nameToken != "name")
            {
                throw MissingExpectedTokenException("name");
            }

            string nameValue;
            getNextToken(tokens, nameValue, index++);

            if (nameValue.empty())
            {
                throw MissingExpectedTokenException("name");
            }

            string valueToken;
            getNextToken(tokens, valueToken, index++);

            if (valueToken != "value")
            {
                throw MissingExpectedTokenException("value");
            }

            string valueValue;
            getNextToken(tokens, valueValue, index++);

            if (valueValue.empty())
            {
                throw MissingExpectedTokenException("value");
            }

            Float val = stof(valueValue);
            Primitive<Float>* floatPrim = new Primitive<Float>(nameValue, val);

            nodes[nodes.size() - 1]->getPrimList()->addFloatPrimitive(floatPrim);
        }
        else if (token == "string")
        {
            string nameToken;
            getNextToken(tokens, nameToken, index++);

            if (nameToken != "name")
            {
                throw MissingExpectedTokenException("name");
            }

            string nameValue;
            getNextToken(tokens, nameValue, index++);

            if (nameValue.empty())
            {
                throw MissingExpectedTokenException("name");
            }

            string valueToken;
            getNextToken(tokens, valueToken, index++);

            if (valueToken != "value")
            {
                throw MissingExpectedTokenException("value");
            }

            string valueValue;
            getNextToken(tokens, valueValue, index++);

            if (valueValue.empty())
            {
                throw MissingExpectedTokenException("value");
            }

            Primitive<string>* stringPrim = new Primitive<string>(nameValue, valueValue);

            nodes[nodes.size() - 1]->getPrimList()->addStringPrimitive(stringPrim);
        }
    }

    if (nodes.size() != 1)
    {
        throw new GlobalTagMisMatch();
    }

    return nodes[0];
}

void Parser::checkBalance(string filename)
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
                throw new TagMisMatch(buf);
            }
        }
        else if (buf == "/sampler")
        {
            if (majorNodes[majorNodes.size() - 1] == "sampler")
                majorNodes.pop_back();
            else
            {
                file.close();
                throw new TagMisMatch(buf);
            }
        }
        else if (buf == "/integrator")
        {
            if (majorNodes[majorNodes.size() - 1] == "integrator")
                majorNodes.pop_back();
            else
            {
                file.close();
                throw new TagMisMatch(buf);
            }
        }
        else if (buf == "/camera")
        {
            if (majorNodes[majorNodes.size() - 1] == "camera")
                majorNodes.pop_back();
            else
            {
                file.close();
                throw new TagMisMatch(buf);
            }
        }
        else if (buf == "/transform")
        {
            if (majorNodes[majorNodes.size() - 1] == "transform")
                majorNodes.pop_back();
            else
            {
                file.close();
                throw new TagMisMatch(buf);
            }
        }
        else if (buf == "/mesh")
        {
            if (majorNodes[majorNodes.size() - 1] == "mesh")
                majorNodes.pop_back();
            else
            {
                file.close();
                throw new TagMisMatch(buf);
            }
        }
        else if (buf == "/bsdf")
        {
            if (majorNodes[majorNodes.size() - 1] == "bsdf")
                majorNodes.pop_back();
            else
            {
                file.close();
                throw new TagMisMatch(buf);
            }
        }
    }

    if (!majorNodes.empty()) throw new GlobalTagMisMatch();
}
