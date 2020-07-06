/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/shader.h>
#include <feign/shapes/objmesh.h>
#include <feign/stats/clocker.h>

FEIGN_BEGIN()

WaveDisplacementShader::WaveDisplacementShader(std::vector<Point2f> points,
                                               std::vector<Functor> functors,
                                               std::vector<Float> dists,
                                               std::vector<Float> start_proxies,
                                               std::vector<Float> end_proxies)
    : points(points),
      functors(functors),
      dists(dists),
      start_proxies(start_proxies),
      end_proxies(end_proxies) { }


bool WaveDisplacementShader::isValid(MeshType mesh_type) const
{
    return mesh_type == OBJ_MESH;
}

// this shader will move the mesh's verts in the direction of their normals
// this shader also assumes the obj mesh has a 1-1 correspondence between
// points and normals - NOTE: this should not be the case
void WaveDisplacementShader::evaluate(void* mesh)
{
    #if CLOCKING
        Clocker::startClock("shader eval");
    #endif

    ObjMesh* obj_mesh = (ObjMesh*)mesh;
    const std::vector<Point3f>& old_verts = obj_mesh->getVerts();
    const std::vector<Normal3f>& old_norms = obj_mesh->getNorms();

    std::vector<Point3f> new_verts = std::vector<Point3f>(old_verts.size());

    // process verts
    for (int i = 0; i < old_verts.size(); ++i)
    {
        for (int j = 0; j < points.size(); ++j)
        {

            Point3f start_pos = old_verts[i];
            Point3f end_pos = old_verts[i] + old_norms[i] * functors[i];
        }
    }

    std::vector<Normal3f> new_norms = std::vector<Normal3f>(old_norms.size());
    // process norms

    throw new FeignRendererException("wave displacement shader evaluate");
    // TODO

    #if CLOCKING
        Clocker::endClock("shader eval");
    #endif
}

FEIGN_END()
