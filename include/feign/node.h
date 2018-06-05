#pragma once

#include <feign/common.h>

enum NodeType {
    NT_Emitter,
    NT_Mesh,
    NT_BSDF,
    NT_PhaseFunction,
    NT_Integrator,
    NT_Media,
    NT_Sampler,
    NT_Texture
};

class SceneNode {
public:
    // TODO
};
