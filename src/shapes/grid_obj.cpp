/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/shapes/grid_obj.h>

FEIGN_BEGIN()

GridObj::GridObj()
    : ObjMesh(),
      resolution(Vec2i(2, 2)), terrain_map(nullptr) {}

GridObj::GridObj(Vec2i res, TextureNode *terrain_map)
    : ObjMesh(),
      resolution(res), terrain_map(terrain_map) {}

void GridObj::preProcess(bool requires_processing)
{
    // TODO: incorporate preprocessing check
    // create the underlying mesh
    LOG("hello");
    int num_verts = resolution[0] * resolution[1];
    tris = std::vector<Triangle>();

    vs = std::vector<Point3f>(num_verts);
    ns = std::vector<Normal3f>(num_verts);
    uvs = std::vector<Vec2f>(num_verts);

    Float min = -1.0;
    Float max = 1.0;

    int index = 0;

    for (int i = 0; i < resolution[1]; ++i)
    {
        for (int j = 0; j < resolution[0]; ++j)
        {
            vs[index] = Point3f((max - min) * float(j) / float(resolution[0] - 1) + min,
                                0.f,
                                (max - min) * float(i) / float(resolution[1] - 1) + min);

            ns[index] = Normal3f(0.f, 1.f, 0.f);
            uvs[index] = Vec2f(float(j) / float(resolution[0] - 1),
                               float(i) / float(resolution[1] - 1));

            index++;
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

            int one = (i - 1) * resolution[0] + j;
            int two = (i)*resolution[0] + j;
            int three = (i)*resolution[0] + (j - 1);
            int four = (i - 1) * resolution[0] + (j - 1);

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
        terrain_map->texture->preProcess();

        for (int i = 0; i < vs.size(); ++i)
        {
            Color3f terrain_map_eval = terrain_map->texture->evaluate(Point2f(0.5f * (vs[i][0] + 1.f),
                                                                              0.5f * (vs[i][2] + 1.f)));
            vs[i][1] += terrain_map_eval[0];
        }

        // TODO: make this its own function
        std::vector<Normal3f> new_norms = std::vector<Normal3f>(ns.size());

        for (int i = 0; i < tris.size(); ++i)
        {
            uint32_t i0_vs = tris[i].vsInds(0);
            uint32_t i1_vs = tris[i].vsInds(1);
            uint32_t i2_vs = tris[i].vsInds(2);

            uint32_t i0_ns = tris[i].nsInds(0);
            uint32_t i1_ns = tris[i].nsInds(1);
            uint32_t i2_ns = tris[i].nsInds(2);

            Point3f p0 = vs[i0_vs];
            Point3f p1 = vs[i1_vs];
            Point3f p2 = vs[i2_vs];

            Normal3f tri_norm = ((p1 - p0) ^ (p2 - p0));

            if (tri_norm.sqrNorm() == 0.f)
            {
                tri_norm = Normal3f(0.f, 1.f, 0.f);
            }

            tri_norm = tri_norm.normalized();

            new_norms[i0_ns] += tri_norm;
            new_norms[i1_ns] += tri_norm;
            new_norms[i2_ns] += tri_norm;
        }

        ns = new_norms;
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
        geomShader->shader->evaluate((void *)this);
    }

    // surface area information is typically only required for
    // importance sampling mesh emitters
    if (requires_processing)
    {
        // TODO: is this really ever needed
        // // precompute the total surface area and cache it
        // sa = 0.0;
        // for (uint32_t i = 0; i < tris.size(); ++i)
        // {
        //     sa += surface_area(i);
        // }
        //
        // // calculate the centroid
        // center = centroid();

        // calculate the bounding box
        bbox = BBox3f(vs[0], vs[0]);
        for (uint32_t i = 1; i < vs.size(); ++i)
        {
            bbox.expand(vs[i]);
        }
    }
}

FEIGN_END()
