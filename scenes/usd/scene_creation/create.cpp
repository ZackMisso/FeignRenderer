#include "create.h"

#include <pxr/usd/usd/stage.h>

FEIGN_BEGIN()

USDUnitTest_CreationTool::USDUnitTest_CreationTool()
{
    // TODO
}

void USDUnitTest_CreationTool::create_all_scenes()
{
    create_scene_000();
}

void USDUnitTest_CreationTool::create_scene_000()
{
    // TODO
}

FEIGN_END()

// scene json file:

// {
//     "scene": {
//         "name": "test_000",
//         "integrator": "integrator",
//         "sampler": "sampler",
//         "camera": "camera"
//     },
//     "integrator": {
//         "name": "integrator",
//         "type": "normal",
//         "filter": "default"
//     },
//     "sampler": {
//         "name": "sampler",
//         "type": "independent",
//         "sample_count": 4
//     },
//     "camera": {
//         "name": "camera",
//         "type": "perspective",
//         "lookat": [
//             -65.6055,
//             47.5762,
//             24.3583,
//             -64.8161,
//             47.2211,
//             23.8576,
//             0.299858,
//             0.934836,
//             -0.190177
//         ],
//         "fov": 30.0,
//         "width": 256,
//         "height": 256
//     },
//     "object": {
//         "name": "floor",
//         "mesh": "plane",
//         "scale": [
//             100.0,
//             1.0,
//             100.0
//         ]
//     },
//     "mesh": {
//         "name": "plane",
//         "type": "triangle_mesh",
//         "filename": "../scenes/meshes/plane.obj"
//     },
//     "object": {
//         "name": "ajax",
//         "mesh": "ajax_obj"
//     },
//     "mesh": {
//         "name": "ajax_obj",
//         "type": "triangle_mesh",
//         "filename": "../scenes/meshes/ajax.obj"
//     }
// }
