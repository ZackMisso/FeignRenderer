/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/objmesh.h>
#include <feign/core/texture.h>

class GridObj : public ObjMesh
{
public:
    GridObj();
    GridObj(Vec2i res, TextureNode* terrain_map);

    ~GridObj() { }

    virtual void preProcess();

    Vec2i resolution;
    TextureNode* terrain_map;
};
