#include <feign/cameras/perspective.h>
#include <feign/filters/gauss_filter.h>
#include <feign/math/warp.h>

Perspective::Perspective() : Camera() { }

Perspective::Perspective(Node* parent) : Camera(parent) { }

void Perspective::preProcess()
{
    preProcessChildren();

    primitives->findTransform("toWorld", cameraToWorld, Transform());
    primitives->findInt("width", filmSize[0], 1280);
    primitives->findInt("height", filmSize[1], 720);
    primitives->findFloat("aperatureRadius", aperatureRadius, 0.0);
    primitives->findFloat("focalDistance", focalDistance, 10.0);
    primitives->findFloat("fov", fov, 30.0);
    primitives->findFloat("nearClip", near, 1e-4);
    primitives->findFloat("farClip", far, 1e4);

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

    // TODO: add ability to choose different filters.

    filter = new GaussFilter();
    filter->preProcess();
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

    Point2f apPos = WarpSpace::squareToUniformDisk(appSamp) * aperatureRadius;
    float t = focalDistance / d(2);

    Vector3f od = d * t;
    // cout << "OD 1: " << od[0] << " " << od[1] << " " << od[2] << endl;
    od = od - Vector3f(apPos(0), apPos(1), 0);
    // cout << "OD 2: " << od[0] << " " << od[1] << " " << od[2] << endl;
    od = od.normalized();
    // cout << "OD 3: " << od[0] << " " << od[1] << " " << od[2] << endl;


    ray.origin = cameraToWorld * Point3f(apPos(0), apPos(1), 0);
    ray.dir = cameraToWorld * od;
    // cout << "ray.dir: " << ray.dir[0] << " " << ray.dir[1] << " " << ray.dir[2] << endl;

    ray.mint = near * invZ;
    ray.maxt = far * invZ;

    // maybe make rays keep track of recipricals
    // ray.update();

    return Color3f(1.f);
}

string Perspective::getName() const
{
    return Camera::getName() + "perspective";
}
