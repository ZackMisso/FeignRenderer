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

    virtual void preProcess(bool use_prims) = 0;
    void preProcessChildren(bool use_prims);

    void printGraph(std::string indent = "") const;

    virtual std::string getName() const = 0;

    int getTag() const;

    PrimitiveList* getPrimList() const;

    Node* getParent() const { return parent; }

protected:
    Node* parent;
    PrimitiveList* primitives;
    std::vector<Node*> children;
    int tag;
};
