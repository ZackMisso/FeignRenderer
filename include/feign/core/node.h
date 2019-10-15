#pragma once


#include <feign/common.h>

enum NodeType
{
    NT_Emitter,
    NT_Mesh,
    NT_BSDF,
    NT_PhaseFunction,
    NT_Integrator,
    NT_Media,
    NT_Sampler,
    NT_Texture,
    NT_Scene,
    NT_Transform,
    NT_Camera,
    NT_Primitive,
    NT_World,
    NT_Recon_Filter,
    NT_Accel
};

class Node
{
public:
    Node();
    Node(Node* parent);
    virtual ~Node();

    virtual NodeType getNodeType() const = 0;

    virtual void addChild(Node* node);
    virtual void setParent(Node* node);
    virtual bool hasChild(Node* node);
    virtual void removeChild(Node* node);

    virtual void print() const { }

    int numChildren() const;

    virtual void preProcess() = 0;
    void preProcessChildren();

    void printGraph(std::string indent = "") const;

    virtual std::string getName() const { return ""; }

    std::string getNodeName() const { return node_name; }

    Node* getParent() const { return parent; }

protected:
    Node* parent;                // parent node
    std::vector<Node*> children; // children of this node
    std::string node_name;       // user/scene defined name
};
