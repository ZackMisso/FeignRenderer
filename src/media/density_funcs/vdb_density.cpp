/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/density_func.h>

#if OPENVDB

#include <openvdb/tools/Interpolation.h>

FEIGN_BEGIN()

OpenVDBDensity::OpenVDBDensity(std::string openvdb_file)
{
    LOG("loading openvdb file: " + openvdb_file);

    openvdb::io::File file(openvdb_file);
    file.open();

    openvdb::GridBase::Ptr base_grid;

    for (openvdb::io::File::NameIterator name_iter = file.beginName();
         name_iter != file.endName(); ++name_iter)
    {
        if (name_iter.gridName() == "density")
        {
            base_grid = file.readGrid(name_iter.gridName());
        }
        else
        {
            std::cout << "Ignoring Grid: " << name_iter.gridName() << std::endl;
        }
    }

    grid = openvdb::gridPtrCast<openvdb::FloatGrid>(base_grid);

    file.close();
    LOG("finished loading openvdb file");
}

OpenVDBDensity::~OpenVDBDensity()
{
    grid->clear();
}

void OpenVDBDensity::pre_process()
{
    LOG("pre-processing openvdb medium");
    openvdb::CoordBBox bbox = grid->evalActiveVoxelBoundingBox();

    openvdb::Coord min = bbox.min();
    openvdb::Coord max = bbox.max();

    openvdb::tools::GridSampler<openvdb::FloatGrid, openvdb::tools::PointSampler> sampler(*grid);

    // TODO: maybe use this later, but for now the majorant will be hardcoded
    // max_density = 0.f;
    //
    // for (int k = min.z(); k <= max.z(); ++k)
    // {
    //     for (int i = min.y(); i <= max.y(); ++i)
    //     {
    //         for (int j = min.x(); j <= max.x(); ++j)
    //         {
    //             Float value = sampler.isSample(openvdb::Vec3R(Float(j), Float(i), Float(k)));
    //             if (value > max_density)
    //             {
    //                 max_density = value;
    //             }
    //         }
    //     }
    // }
    //
    // LOG("VDB Max Density: " + std::to_string(max_density));
    LOG("finished pre-processing openvdb medium");
}

Color3f OpenVDBDensity::eval(const Point3f &p) const
{
    // TODO: need to make box vs. point a configurable parameter
    openvdb::tools::GridSampler<openvdb::FloatGrid, openvdb::tools::BoxSampler> sampler(*grid);
    // openvdb::tools::GridSampler<openvdb::FloatGrid, openvdb::tools::PointSampler> sampler(*grid);

    return Color3f(sampler.wsSample(openvdb::Vec3R(p(0), p(1), p(2))));
}

// Color3f OpenVDBDensity::SpectralD(const Point3f& p) const
// {
//     // TODO: need to make support for colored media
//     return Color3f(D(p));
// }

Float OpenVDBDensity::max_density() const
{
    return max_d;
}

FEIGN_END()

#endif
