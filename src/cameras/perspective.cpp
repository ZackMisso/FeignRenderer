/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/camera.h>
#include <feign/core/recon_filter.h>
#include <feign/math/warp.h>

FEIGN_BEGIN()

Perspective::Perspective(Float aperature_radius,
                         Float focal_distance,
                         Float fov,
                         Float near,
                         Float far,
                         uint32_t width,
                         uint32_t height)
    : Camera(),
      aperature_radius(aperature_radius),
      focal_distance(focal_distance),
      fov(fov),
      near(near),
      far(far)
{
    film_size = Vec2i(width, height);
}

void Perspective::pre_process()
{
    float aspect = float(film_size[0]) / float(film_size[1]);

    float recip = 1.0 / (far - near);
    float cot = 1.0 / std::tan(deg_to_rad(fov / 2.0));

    Matrix4f perspective = Matrix4f();

    perspective[0] = cot;
    perspective[5] = cot;
    perspective[10] = far * recip;
    perspective[11] = -near * far * recip;
    perspective[14] = 1.0;

    Matrix4f aspect_scale = Matrix4f::scale(Vec3f(-0.5, -0.5 * aspect, 1.0));
    Matrix4f aspect_trans = Matrix4f::translate(Vec3f(-1.0, -1.0 / aspect, 0.0));

    Matrix4f mat = aspect_scale * aspect_trans * perspective;

    sample_to_camera = Transform(mat).inverse();
}

Color3f Perspective::sample_ray(Ray3f &ray,
                                const Point2f &film_samp,
                                const Point2f &app_samp) const
{
    Point3f near_p = sample_to_camera * Point3f(
                                            film_samp(0) * 1.0 / film_size(0),
                                            film_samp(1) * 1.0 / film_size(1),
                                            0.f);

    Vector3f d = near_p.normalized();
    float inv_z = 1.f / d(2);

    Point2f ap_pos = WarpSpace::sqr_to_uni_disk(app_samp) * aperature_radius;
    float t = focal_distance / d(2);

    Vector3f od = d * t;
    od = od - Vector3f(ap_pos(0), ap_pos(1), 0);
    od = od.normalized();

    ray.origin = camera_to_world * Point3f(ap_pos(0), ap_pos(1), 0);
    ray.dir = camera_to_world * od;

    ray.near = near * inv_z;
    ray.far = far * inv_z;

    // maybe make rays keep track of recipricals
    // ray.update();

    return Color3f(1.f);
}

void Perspective::print() const
{
    std::cout << "camera transform:" << std::endl;
    camera_to_world.print();
    std::cout << "sample to camera transform:" << std::endl;
    sample_to_camera.print();
    std::cout << "aperature radius: " << aperature_radius << std::endl;
    std::cout << "focal distance: " << focal_distance << std::endl;
    std::cout << "fov: " << fov << std::endl;
    std::cout << "near: " << near << std::endl;
    std::cout << "far: " << far << std::endl;
    std::cout << std::endl;
}

FEIGN_END()
