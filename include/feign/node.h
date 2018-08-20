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

class SceneNode
{
public:
    SceneNode();
    SceneNode(SceneNode* parent);
    virtual ~SceneNode();

    virtual NodeType getNodeType() const = 0;
    virtual bool isPrimitive() const = 0;

    virtual void addChild(SceneNode* node);
    virtual void setParent(SceneNode* node);
    virtual bool hasChild(SceneNode* node);
    virtual void removeChild(SceneNode* node);

    int getTag() const;

    PrimitiveList* getPrimList() const;

protected:
    SceneNode* parent;
    PrimitiveList* primitives;
    vector<SceneNode*> children;
    int tag;
};
