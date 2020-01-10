/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/shapes/objmesh.h>
#include <feign/core/texture.h>

class GridObj : public ObjMesh
{
public:
    struct Params
    {
        Params(std::string texture,
               std::string shader,
               Vec2i resolution)
            : texture(texture),
              shader(shader),
              resolution(resolution) { }

        std::string shader;
        std::string texture;
        Vec2i resolution;
    };

    GridObj();
    GridObj(Vec2i res, TextureNode* terrain_map);

    ~GridObj() { }

    virtual void preProcess();

    Vec2i resolution;
    TextureNode* terrain_map;
};
