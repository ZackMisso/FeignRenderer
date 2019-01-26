#pragma once

// this is a hacky sort of thing to make a global first node

#include <feign/node.h>
#include <feign/scene.h>

class WorldNode : public Node
{
public:
    WorldNode();
    WorldNode(Node* parent);
    virtual ~WorldNode();

    void renderAllScenes();

    virtual void preProcess();

    std::vector<Scene*> collectScenes() const;

    virtual std::string getName() const;

    virtual NodeType getNodeType() const;
};
