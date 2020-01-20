/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/accel.h>

void SDFAccel::preProcess()
{
    sdfs = std::vector<SDFShape*>();
}

void SDFAccel::clear()
{
    sdfs.clear();
}

void SDFAccel::addSDFShape(SDFShape* mesh)
{
    sdfs.push_back(mesh);
}

void SDFAccel::build()
{
    scene_box = BBox3f();

    for (int i = 0; i < sdfs.size(); ++i)
    {
        scene_box.expand(sdfs[i]->boundingBox());
    }
}

// normals using finite difference
Normal3f SDFAccel::sd_normal(Point3f pt) const
{
    int dum;
    Float dx = sd_evaluate(pt + EPS_VEC_X, dum) - sd_evaluate(pt - EPS_VEC_X, dum);
    Float dy = sd_evaluate(pt + EPS_VEC_Y, dum) - sd_evaluate(pt - EPS_VEC_Y, dum);
    Float dz = sd_evaluate(pt + EPS_VEC_Z, dum) - sd_evaluate(pt - EPS_VEC_Z, dum);

    return Normal3f(dx, dy, dz).normalized();
}

Float SDFAccel::sd_evaluate(Point3f point, int& shape_index) const
{
    Float min = std::numeric_limits<Float>::infinity();

    for (int i = 0; i < sdfs.size(); ++i)
    {
        Float shape_val = sdfs[i]->evaluate(point);
        Float d_val = shape_val;

        if (i != 0)
        {
            Float smooth_interp = std::min(sdfs[i]->interp,
                                           sdfs[shape_index]->interp);

            d_val = sd_smooth_union(shape_val, min, smooth_interp);
        }

        if (min > d_val)
        {
            min = d_val;
            shape_index = i;
        }
    }

    return min;
}

Float SDFAccel::sd_smooth_union(Float d1, Float d2, Float k) const
{
    Float h = clamp(0.5 + 0.5*(d2 - d1) / k, 0.0, 1.0 );
    return interp(d2, d1, h) - k * h * (1.0 - h);
}

// TODO: this intersection method is extremely inefficient for the teacup in
//       a stadium case
bool SDFAccel::intersect(const Ray3f& scene_ray, Intersection& its) const
{
    // assert(false);
    Float near;
    Float far;

    scene_box.intersect(scene_ray, near, far);

    bool hit = false;
    int i = 0;

    Float dist = near;
    int max_iter = 100;

    for (; i < max_iter && !hit && dist <= far; ++i)
    {
        // assert(false);
        Point3f p = scene_ray(dist);
        int shape_index;

        Float sd = sd_evaluate(p, shape_index);
        // LOG("signed dist:",sd);

        if (sd <= Epsilon)
        {
            hit = true;
            its.t = dist;
            its.uv = Vec2f(0.f, 0.f); // how to incorporate uv coordinates?

            its.intersected_mesh = sdfs[shape_index];
            its.p = p;

            Normal3f norm = sd_normal(p);
            its.g_frame = CoordinateFrame(norm);
            its.s_frame = its.g_frame;

            return true;
        }

        dist += sd;
    }

    return false;
}
