// #pragma once
//
// // This implementation is based off of Nori's implementation
//
// #include <feign/accel/accel.h>
//
// #define BVH_SERIAL_THRESHOLD 32
// #define BVH_GRAIN_SIZE 1000 // for parallelization later
// #define BVH_TRAVERSAL_COST 1 // heuristic operator
// #define BVH_INTERSECTION_COST 1 // heuristic operator
//
// struct BVH_Node
// {
//     union
//     {
//         struct
//         {
//             unsigned flag : 1;
//             uint32_t size : 31;
//             uint32_t start;
//         } leaf;
//
//         struct
//         {
//             unsigned flag : 1;
//             uint32_t axis : 31;
//             uint32_t rightChild;
//         } inner;
//
//         uint64_t data;
//     };
//
//     BBox3f bbox;
//
//     bool isLeaf() const
//     {
//         return leaf.flag == 1;
//     }
//
//     bool isInner() const
//     {
//         return leaf.flag == 0;
//     }
//
//     bool empty() const
//     {
//         return data == 0;
//     }
//
//     uint32_t start() const
//     {
//         return leaf.start;
//     }
//
//     uint32_t end() const
//     {
//         return leaf.start + leaf.size;
//     }
// };
//
// class BVH : public Accel
// {
//     friend class BVHBuildTask;
// public:
//     BVH();
//     ~BVH();
//
//     virtual void clear();
//     virtual void build();
//     virtual void addShape(Shape* mesh);
//     virtual void preProcess();
//
//     virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const;
//
//     std::pair<float, uint32_t> statistics(uint32_t node_idx = 0) const;
//
//     virtual std::string getName() const;
//
// protected:
//     uint32_t findMesh(uint32_t& idx) const
//     {
//         auto it = std::lower_bound(mesh_offsets.begin(), mesh_offsets.end(), idx+1) - 1;
//         // std::cout << "Old IDX: " << idx;
//         idx -= *it;
//         // std::cout << " New IDX: " << idx << std::endl;
//         return (uint32_t) (it - mesh_offsets.begin());
//     }
//
//     BBox3f getBoundingBox(uint32_t idx) const
//     {
//         uint32_t meshIdx = findMesh(idx);
//         return meshes[meshIdx]->boundingBox(idx);
//     }
//
//     Point3f getCentroid(uint32_t idx) const
//     {
//         uint32_t meshIdx = findMesh(idx);
//         return meshes[meshIdx]->centroid(idx);
//     }
//
//     std::vector<uint32_t> mesh_offsets;
//     std::vector<uint32_t> indices;
//     std::vector<BVH_Node> nodes;
//     BBox3f bbox;
// };
