/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/camera.h>
#include <feign/core/recon_filter.h>
#include <feign/math/warp.h>

FEIGN_BEGIN()

Perspective::Perspective(Float aperatureRadius,
                         Float focalDistance,
                         Float fov,
                         Float near,
                         Float far,
                         uint32_t width,
                         uint32_t height)
    : Camera(),
      aperatureRadius(aperatureRadius),
      focalDistance(focalDistance),
      fov(fov),
      near(near),
      far(far)
{
    filmSize = Vec2i(width, height);
}

void Perspective::preProcess()
{
    float aspect = float(filmSize[0]) / float(filmSize[1]);

    float recip = 1.0 / (far - near);
    float cot = 1.0 / std::tan(degToRad(fov / 2.0));

    Matrix4f perspective = Matrix4f();

    perspective[0] = cot;
    perspective[5] = cot;
    perspective[10] = far * recip;
    perspective[11] = -near * far * recip;
    perspective[14] = 1.0;

    Matrix4f aspectScale = Matrix4f::scale(Vec3f(-0.5, -0.5 * aspect, 1.0));
    Matrix4f aspectTrans = Matrix4f::translate(Vec3f(-1.0, -1.0 / aspect, 0.0));

    Matrix4f mat = aspectScale * aspectTrans * perspective;

    sampleToCamera = Transform(mat).inverse();
}

Color3f Perspective::sampleRay(Ray3f& ray,
                               const Point2f& filmSamp,
                               const Point2f& appSamp) const
{
    Point3f nearP = sampleToCamera * Point3f(
                            filmSamp(0) * 1.0 / filmSize(0),
                            filmSamp(1) * 1.0 / filmSize(1),
                            0.f);

    Vector3f d = nearP.normalized();
    float invZ = 1.f / d(2);

    Point2f apPos = WarpSpace::sqrToUniDisk(appSamp) * aperatureRadius;
    float t = focalDistance / d(2);

    Vector3f od = d * t;
    od = od - Vector3f(apPos(0), apPos(1), 0);
    od = od.normalized();

    ray.origin = cameraToWorld * Point3f(apPos(0), apPos(1), 0);
    ray.dir = cameraToWorld * od;

    ray.near = near * invZ;
    ray.far = far * invZ;

    // maybe make rays keep track of recipricals
    // ray.update();

    return Color3f(1.f);
}

void Perspective::print() const
{
    std::cout << "camera transform:" << std::endl;
    cameraToWorld.print();
    std::cout << "sample to camera transform:" << std::endl;
    sampleToCamera.print();
    std::cout << "aperature radius: " << aperatureRadius << std::endl;
    std::cout << "focal distance: " << focalDistance << std::endl;
    std::cout << "fov: " << fov << std::endl;
    std::cout << "near: " << near << std::endl;
    std::cout << "far: " << far << std::endl;
    std::cout << std::endl;
}

FEIGN_END()
