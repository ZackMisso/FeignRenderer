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

    Float min = -1.0;
    Float max = 1.0;

    for (int i = 0; i < resolution[1]; ++i)
    {
        for (int j = 0; j < resolution[0]; ++j)
        {
            vs.push_back(Point3f((max - min) * (float(j) / float(resolution[0]-1) + min),
                                 0.f;
                                 (max - min) * (float(i) / float(resolution[0]-1) + min));
            ns.push_back(Normal3f(0.f, 1.f, 0.f));
            uvs.push_back(Vec2f(float(j) / float(resolution[0]-1),
                                float(i) / float(resolution[0]-1));
        }
    }

    // create triangles
    for (int i = 1; i < resolution[1]; ++i)
    {
        for (int j = 1; j < resolution[0]; ++j)
        {
            // [1,0]
            // [1,1]
            // [0,1]
            // [0,0]

            int one = (i-1) * resolition[0] + j;
            int two = (i) * resolition[0] + j;
            int three = (i) * resolition[0] + (j-1);
            int four = (i-1) * resolition[0] + (j-1);

            Vec3u inds_one = Vec3u(one, four, two);
            Vec3u inds_two = Vec3u(four, three, two);

            tris.push_back(Triangle(inds_one, inds_one, inds_one));
            tris.push_back(Triangle(inds_two, inds_two, inds_two));
        }
    }

    // apply the terrain_map as displacement to the initial mesh
    // this assumes the terrain_map is already in grayscale
    if (terrain_map)
    {
        for (int i = 0; i < vs.size(); ++i)
        {
            Color3f terrain_map_eval = terrain_map->evaluate(Point2f(vs[i][0], vs[i][2]));
            vs[i][1] += terrain_map_eval[0];
        }

        // TODO: update the normals of the grid after displacement
        new throw FeignRendererException("update normals of grid object after displacement");
    }

    // apply the mesh's transformation to all of the points
    for (int i = 0; i < vs.size(); ++i)
    {
        vs[i] = transform * vs[i];
        ns[i] = (transform * ns[i]).normalized();
    }

    // apply any geometry shader to the underlying mesh
    if (geomShader->shader && geomShader->shader->isValid(OBJ_MESH))
    {
        geomShader->shader->evaluate((void*)this);
    }

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
