#include <feign/core/shader.h>
#include <feign/shapes/objmesh.h>

InterpVertsToSphereShader::InterpVertsToSphereShader(float prop_of_shortest_axis, float interp)
    : prop_of_shortest_axis(prop_of_shortest_axis), interp(interp)
{ }


bool InterpVertsToSphereShader::isValid(MeshType mesh_type) const
{
    return mesh_type == OBJ_MESH;
}

void InterpVertsToSphereShader::evaluate(void* mesh)
{
    ObjMesh* obj_mesh = (ObjMesh*)mesh;

    Point3f centroid = obj_mesh->centroid();
    BBox3f bbox = obj_mesh->boundingBox();
    int minor_axis = bbox.minorAxis();
    Float radius = bbox.extents()[minor_axis] * prop_of_shortest_axis;

    const std::vector<Triangle>& triangles = obj_mesh->getTris();
    const std::vector<Point3f>& old_verts = obj_mesh->getVerts();
    const std::vector<Normal3f>& old_norms = obj_mesh->getNorms();

    std::vector<Point3f> new_verts = std::vector<Point3f>(old_verts.size());
    std::vector<Normal3f> new_norms = std::vector<Normal3f>(old_norms.size());

    // compute new positions of all vertices by interpolation
    for (int i = 0; i < old_verts.size(); ++i)
    {
        Point3f start_pos = old_verts[i];
        Point3f end_pos = (start_pos - centroid).normalized() * radius + centroid;

        new_verts[i] = start_pos * (1.f - interp) + end_pos * interp;
    }

    // Since the magnitude of the crossproduct of two vectors is proportional to the area
    // of the triangles they create, we can compute smooth-ish vertex normals by
    // accumulating the unnormalized normals of all triangles attached to each vertex
    // and then normalizing the vertex normals.

    // accumulate the contributions to each vertex normals from each triangle they
    // make up
    for (int i = 0; i < triangles.size(); ++i)
    {
        uint32_t i0_vs = triangles[i].vsInds(0);
        uint32_t i1_vs = triangles[i].vsInds(1);
        uint32_t i2_vs = triangles[i].vsInds(2);

        uint32_t i0_ns = triangles[i].nsInds(0);
        uint32_t i1_ns = triangles[i].nsInds(1);
        uint32_t i2_ns = triangles[i].nsInds(2);

        Point3f p0 = new_verts[i0_vs];
        Point3f p1 = new_verts[i1_vs];
        Point3f p2 = new_verts[i2_vs];

        Normal3f tri_norm = ((p1-p0)^(p2-p0));

        if (tri_norm.sqrNorm() == 0.f)
        {
            tri_norm = Normal3f(0.f, 1.f, 0.f);
        }

        tri_norm = tri_norm.normalized();

        new_norms[i0_ns] += tri_norm;
        new_norms[i1_ns] += tri_norm;
        new_norms[i2_ns] += tri_norm;
    }

    // normalize the normals
    for (int i = 0; i < new_norms.size(); ++i)
    {
        // do this just in case
        if (new_norms[i].sqrNorm() == 0.f)
        {
            new_norms[i] = Normal3f(0.f, 1.f, 0.f);
        }

        new_norms[i] = new_norms[i].normalized();
    }

    obj_mesh->setVerts(new_verts);
    obj_mesh->setNorms(new_norms);
}
