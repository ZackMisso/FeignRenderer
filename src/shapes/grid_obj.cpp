/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/shapes/grid_obj.h>

GridObj::GridObj()
    : ObjMesh(),
      resolution(Vec2i(2, 2)), terrain_map(nullptr) { }

GridObj::GridObj(Vec2i res, TextureNode* terrain_map)
    : ObjMesh(),
      resolution(res), terrain_map(terrain_map) { }

void GridObj::preProcess()
{
    // create the underlying mesh
    int num_verts = resolution[0] * resolution[1];
    tris = std::vector<Triangle>();

    vs = std::vector<Point3f>(num_verts);
    ns = std::vector<Normal3f>(num_verts);
    uvs = std::vector<Vec2f>(num_verts);

    for (int i = 0; i < resolution[1]; ++i)
    {
        for (int j = 0; j < resolution[0]; ++j)
        {
            // create vertex data
        }
    }

    // create triangles
    throw new FeignRendererException("gridobj create triangles");

    // apply the terrain_map as displacement to the initial mesh
    throw new FeignRendererException("gridobj terrain_map");

    // apply the mesh's transformation to all of the points
    throw new FeignRendererException("gridobj transform verts");

    // apply any geometry shader to the underlying mesh
    throw new FeignRendererException("gridobj geom_shader");

    // precompute the total surface area and cache it
    sa = 0.0;
    for (uint32_t i = 0; i < tris.size(); ++i)
    {
        sa += surfaceArea(i);
    }

    // calculate the centroid
    Point3f center = centroid();

    // calculate the bounding box
    bbox = BBox3f(vs[0], vs[0]);
    for (uint32_t i = 1; i < vs.size(); ++i)
    {
        bbox.expand(vs[i]);
    }
}
