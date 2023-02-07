/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()

struct Node
{
public:
    Node() {}
    Node(std::string name) : name(name) {}
    virtual ~Node() {}

    // modification methods to easily implement custom behavior
    virtual void modify_color(std::string var, Color3f val) {}
    virtual void modify_float(std::string var, Float val) {}
    virtual void modify_int(std::string var, int val) {}
    virtual void modify_string(std::string var, std::string val) {}

    virtual void print() const {} // TODO: remove this

    std::string name;
};

// a struct containing the render wide globals which might be used to
// effect the nodes
struct NodeUpdateGlobals
{
    NodeUpdateGlobals();

    Float screen_x;
    Float screen_y;
};

// modifiers which can be used to modify nodes
struct Modifier
{
public:
    Modifier(std::string var) : var(var) {}

    // TODO: need to think this design through to account for multithreading
    // TODO: once actual time is accounted for, change integer frame, to float time
    virtual void modify(Node *node, NodeUpdateGlobals &globes, int frame) const = 0;

    std::string var;
};

FEIGN_END()
