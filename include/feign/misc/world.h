/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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
