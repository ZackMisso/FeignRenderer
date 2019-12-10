/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/bsdf.h>

// TODO: eventually make this more complicated
// idk how to structure this, need to research some more on how other renderers
// do this

class Material
{
public:
    Material();
    ~Material();

    BSDF* getBSDF() const { return bsdf; }
    void setBSDF(BSDF* param) { bsdf = param; }

protected:
    BSDF* bsdf;
};

/////////////////////////////////////////////////
// Material Node structure
/////////////////////////////////////////////////
struct MaterialNode : public Node
{
public:
    MaterialNode() : material(nullptr) { }
    MaterialNode(std::string name) : Node(name) { }
    MaterialNode(Material* material) : material(material) { }

    Material* material;
};
/////////////////////////////////////////////////
