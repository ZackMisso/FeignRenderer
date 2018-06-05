#pragma once

#include <feign/common.h>
#include <feign/node.h>

class Scene {
public:
    Scene();
    ~Scene();

protected:
    vector<SceneNode*> sceneObjects;
};
