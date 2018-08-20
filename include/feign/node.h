#pragma once

// TODO: rename this class - this node can be used for more than one
// scene

#include <feign/common.h>
#include <feign/misc/primlist.h>

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
    NT_Primitive
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

    void printGraph(string indent = "") const;

    virtual string getName() const = 0;

    int getTag() const;

    PrimitiveList* getPrimList() const;

protected:
    Node* parent;
    PrimitiveList* primitives;
    vector<Node*> children;
    int tag;
};
